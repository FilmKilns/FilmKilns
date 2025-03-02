package com.alimin.fk_flutter.module.image

import android.content.Context
import android.graphics.Color
import android.graphics.Rect
import android.hardware.camera2.CameraManager
import android.util.Log
import android.util.Size
import android.view.Surface
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.core.FkPaint
import com.alimin.fk.define.kScaleType
import com.alimin.fk.device.FkAbsCamera
import com.alimin.fk.device.FkCamera2
import com.alimin.fk.device.OnCaptureListener
import com.alimin.fk.device.OnInfoListener
import com.alimin.fk.engine.*
import com.alimin.fk.entity.FkCameraFeatureKey
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import com.alimin.fk.entity.FkRational
import com.alimin.fk.entity.FkResult
import com.alimin.fk.listener.FkDoStatusListener
import com.alimin.fk.listener.FkGetLayersListener
import com.alimin.fk.listener.FkNativeMsgListener
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.alimin.pic.model.ImageEngineModel
import com.alimin.pic.model.impl.ImageEngineModelImpl
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import java.io.File
import java.nio.ByteBuffer

class ImagePresenter(
    private val view: ImageContract.View,
    workspace: String,
    private val model: ImageEngineModel = ImageEngineModelImpl()
) : ImageContract.Presenter(), OnInfoListener {
    private val cacheFile: File
    private val engine: FkImage
    private val modelEngine: FkImageModel
    private var curLayer = -1
    private var paint = FkPaint(10, Color.WHITE)
    private var layerUpdateListeners = ArrayList<OnLayerUpdateListener>()
    private var loadStatusListeners = ArrayList<FkDoStatusListener>()
    private var reqRestoreCamera = false
    private var cameraLayer = -1
    private var camera: FkAbsCamera? = null
    private var cameraManager: CameraManager? = null
    private val cameraSettings = FkCameraSettings(FkCameraFeatures.kFacing.Front, Size(1080, 1440), Size(3072, 4096)).apply {
        reqFeatures.add(FkCameraFeatureKey.SCENE_AUTO_EXT)
    }

    companion object {
        const val TAG = "ImagePresenter"
    }

    init {
        view.presenter = this
        cacheFile = File(File(workspace), "/${System.currentTimeMillis()}.fkp.dir")
        engine = FkImage(cacheFile.absolutePath)
        modelEngine = FkImageModel(engine)
    }

    private fun notifyLayers() {
        getLayers(object : FkGetLayersListener {
            override fun onGetLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>) {
                synchronized(this) {
                    layerUpdateListeners.forEach {
                        GlobalScope.launch(Dispatchers.Main) {
                            it.onLayers(layers)
                        }
                    }
                }
            }
        })
    }

    override fun create() {
        engine.create()
        modelEngine.create()
    }

    override fun start() {
        engine.start()
        modelEngine.start()
        modelEngine.load(cacheFile.absolutePath,
            object : FkDoStatusListener {
                override fun onDone() {
                    synchronized(this) {
                        loadStatusListeners.forEach {
                            GlobalScope.launch(Dispatchers.Main) {
                                it.onDone()
                            }
                        }
                    }

                    GlobalScope.launch(Dispatchers.Main) { notifyLayers() }
                }
            })
        if (reqRestoreCamera) {
            reqRestoreCamera = false
            openCamera()
        }
    }

    override fun stop() {
        reqRestoreCamera = cameraLayer > 0
        closeCamera()
        modelEngine.save(cacheFile.absolutePath)
        modelEngine.stop()
        engine.stop()
    }

    override fun destroy() {
        modelEngine.destroy()
        engine.destroy()
        synchronized(this) {
            layerUpdateListeners.clear()
        }
        cacheFile.deleteRecursively()
    }

    override fun notifyRender() {
        engine.notifyRender()
    }

    override fun selectLayer(layerId: Int) {
        if (layerId >= 0) {
            curLayer = layerId
        }
    }

    override fun isSelectedLayer(layerId: Int) : Boolean = curLayer == layerId

    override fun newLayerWithFile(path: String) {
        val layer = engine.newLayerWithFile(path)
        if (curLayer < 0) {
            selectLayer(layer)
        }
        engine.notifyRender()
        if (layer > 0) {
            notifyLayers()
        }
    }

    override fun newLayerWithColor(size: Size, red: Int, green: Int, blue: Int, alpha: Int) {
        val layer = engine.newLayerWithColor(size, red, green, blue, alpha)
        if (curLayer < 0) {
            selectLayer(layer)
        }
        engine.notifyRender()
        if (layer > 0) {
            notifyLayers()
        }
    }

    override fun newLayerWithSource(source: FkAbsImageSource) {
        val layer = engine.newLayerWithSource(source)
        if (curLayer < 0) {
            selectLayer(layer)
        }
        engine.notifyRender()
        if (layer > 0) {
            notifyLayers()
        }
    }

    override fun removeLayer(layerId: Int) {
        val ret = engine.removeLayer(layerId)
        if (FkResult.OK == ret) {
            if (curLayer == layerId) {
                curLayer = -1
            }
            engine.notifyRender()
            if (layerId > 0) {
                notifyLayers()
            }
        }
    }

    override fun detachFromSurface(surface: Surface) {
        engine.detachFromSurface(surface)
    }

    override fun attachToSurface(surface: Surface) {
        engine.attachToSurface(surface, kScaleType.CENTER_INSIDE)
    }

    override fun postTranslate(dx: Int, dy: Int) {
        if (checkCurLayer()) {
            engine.postTranslate(curLayer, dx, dy)
        }
    }

    override fun postRotate(dr: FkRational) {
        if (checkCurLayer()) {
            engine.postRotation(curLayer, dr.num, dr.den)
        }
    }

    override fun postScale(ds: FkRational) {
        if (checkCurLayer()) {
            engine.postScale(curLayer, ds.num.toFloat() / ds.den, ds.num.toFloat() / ds.den)
        }
    }

    override fun getLayers(listener: FkGetLayersListener): Int {
        return modelEngine.getLayers(listener).code
    }

    override fun crop(rect: Rect) {
        if (checkCurLayer()) {
//            drawPath(rect.left, rect.top)
//            drawPath(rect.right, rect.bottom)
//            drawPath(rect.left, rect.top)
//            drawPathFinish()
            engine.crop(rect)
        }
    }

    override fun save(file: String) {
        view.onImageSaving()
        engine.save(file, object : FkNativeMsgListener {
            override fun onNativeMsgReceived(
                what: Int,
                arg: Int,
                msg: String?,
                pbObject: ByteBuffer?
            ): Boolean {
                GlobalScope.launch(Dispatchers.Main) {
                    if (arg == 0) {
                        view.onImageSaved(file)
                    } else {
                        view.showError(arg, "Save picture error: $arg")
                    }
                }
                return true
            }
        })
    }

    override fun save() {
        modelEngine.save(cacheFile.absolutePath)
    }

    override fun load(listener: FkDoStatusListener) {
        modelEngine.load(cacheFile.absolutePath, listener)
    }

    private fun checkCurLayer(): Boolean {
        return curLayer >= 0
    }

    @Synchronized
    override fun addLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.add(l)
    }

    @Synchronized
    override fun removeLayerUpdateListener(l: OnLayerUpdateListener) {
        layerUpdateListeners.remove(l)
    }

    @Synchronized
    override fun addLoadStatusListener(l: FkDoStatusListener) {
        loadStatusListeners.add(l)
    }

    @Synchronized
    override fun removeLoadStatusListener(l: FkDoStatusListener) {
        loadStatusListeners.remove(l)
    }

    override fun drawPath(x: Int, y: Int) {
        engine.drawPath(curLayer, x, y, paint)
    }

    override fun drawPathFinish() {
        engine.drawPathFinish(curLayer)
    }

    override fun getPaint(): FkPaint = paint

    override fun openCamera() {
        if (camera != null) {
            return
        }
        cameraManager = view.getContext().getSystemService(Context.CAMERA_SERVICE) as CameraManager
        camera = FkCamera2(cameraManager!!)
        camera?.addOnInfoListener(this)
        camera?.let {
            it.getImageSource().addOnRenderListener(object : FkAbsImageSource.OnRenderListener{
                override fun onCreate() {
                }

                override fun onDestroy() {
                }

                override fun onRender() {
                    notifyRender()
//                    runOnUiThread { methodChannel?.invokeMethod("onCameraFrameUpdated", null) }
                }
            })
            it.create()
            it.start(cameraSettings)
            cameraLayer = engine.newLayerWithSource(it.getImageSource())
        }
    }

    override fun closeCamera() {
        if (cameraLayer < 0) {
            return
        }
        removeLayer(cameraLayer)
        notifyRender()
        camera?.stop()
        camera?.destroy()
        camera?.removeOnInfoListener(this)
        camera = null
        cameraLayer = -1
        cameraManager = null
    }

    override fun switchCamera() {
        camera?.let {
            val listener = object : OnInfoListener {
                override fun onInfo(what: Int, arg0: Int, arg1: Any?, msg: String) {
                    it.removeOnInfoListener(this)
                    cameraSettings.apply {
                        facing =
                            if (facing == FkCameraFeatures.kFacing.Back) FkCameraFeatures.kFacing.Front else FkCameraFeatures.kFacing.Back
                    }
                    val ret = it.start(cameraSettings)
                    if (ret == FkResult.OK.code) {
                        view.onCameraInfo(FkResult.INFO_CAMERA_SWITCH_DONE)
                    } else {
                        view.showError(FkResult.FAIL.code, "Switch camera fail")
                    }
                }
            }
            it.addOnInfoListener(listener)
            it.stop()
        }
    }

    override fun takePicture() {
        camera?.takePicture(object : OnCaptureListener {
            override fun onResult(source: FkAbsImageSource?) {
                if (source == null) {
                    view.showError(FkResult.FAIL.code, "Capture fail")
                } else {
                    newLayerWithSource(source)
                    view.onCameraInfo(FkResult.INFO_CAMERA_TAKE_PICTURE_SUCCESS)
                }
            }
        })
    }

    override fun onInfo(what: Int, arg0: Int, arg1: Any?, msg: String) {
        when (what) {
            FkResult.INFO_CAMERA_PREVIEW_SIZE_SELECTED.code -> arg1?.let {
                val previewSize  = it as Size
                engine.setCanvasSize(previewSize.width, previewSize.height)
            }
        }
    }

    override fun testAction() {
        view.onCameraInfo(FkResult.INFO_CAMERA_TEST_ACTION)
    }
}