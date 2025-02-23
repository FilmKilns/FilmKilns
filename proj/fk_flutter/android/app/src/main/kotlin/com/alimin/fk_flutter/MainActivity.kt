package com.alimin.fk_flutter

import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Color
import android.graphics.SurfaceTexture
import android.hardware.camera2.CameraManager
import android.os.Bundle
import android.text.TextUtils
import android.util.Log
import android.util.Size
import android.view.Surface
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import com.alimin.fk.FilmKilns
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.core.FkAbsImageSource2
import com.alimin.fk.core.FkNativeObject
import com.alimin.fk.define.kScaleType
import com.alimin.fk.device.FkAbsCamera
import com.alimin.fk.device.FkCamera2
import com.alimin.fk.device.OnCaptureListener
import com.alimin.fk.device.OnInfoListener
import com.alimin.fk.engine.FkImage
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import com.alimin.fk.entity.FkResult
import com.alimin.fk.source.FkBitmapSource
import com.alimin.fk.source.FkNativeTestObject
import com.alimin.fk.utils.FkLogcat
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.android.FlutterActivityLaunchConfigs
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import java.io.FileOutputStream
import java.nio.ByteBuffer

class MainActivity : FlutterActivity(), MethodChannel.MethodCallHandler, TextureView.SurfaceTextureListener {
    private lateinit var container: FrameLayout
    private lateinit var surfaceContainer: FrameLayout
    private var renderView: TextureView? = null
    private var surface: Surface? = null
    private var methodChannel: MethodChannel? = null
    private var cameraLayer = -1
    private var camera: FkAbsCamera? = null
    private var cameraManager: CameraManager? = null
    private val cameraSettings = FkCameraSettings(FkCameraFeatures.kFacing.Front, Size(1080, 1440), Size(3072, 4096)).apply {
//        reqCameraKeys.add(FkCameraAvailableKey.SCENE_AUTO_EXT)
    }

    companion object {
        const val TAG = "FkMainActivity"
    }

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)
    }

    override fun getIntent(): Intent {
        return super.getIntent().apply {
            putExtra("background_mode", FlutterActivityLaunchConfigs.BackgroundMode.transparent.toString())
        }
    }

    private fun testNativeObject() {
        val obj = FkNativeTestObject()
        obj.create()
        if (!obj.getBool(true)) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getByte(1) != 1.toByte()) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getChar('2') != '2') {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getShort(30) != 30.toShort()) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getInt(40) != 40) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getLong(50) != 50L) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getFloat(6.0f) != 6.0f) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (obj.getDouble(7.0) != 7.0) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        if (!TextUtils.equals(obj.getString("80"), "80")) {
            throw RuntimeException("[FAIL] testNativeObject error.")
        }
        obj.destroy()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        FilmKilns.init(applicationContext)
        testNativeObject()
        Log.i(TAG, "transparencyMode=${transparencyMode}")
        flutterEngine?.apply {
            Log.i(TAG, "Create media_editor channel")
            methodChannel = MethodChannel(dartExecutor, "com.alimin.flutter/media_editor")
            methodChannel?.setMethodCallHandler(this@MainActivity)
        }
    }

    override fun setContentView(view: View?) {
        surfaceContainer = FrameLayout(this).apply {
            layoutParams = ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
            )
        }
        container = FrameLayout(this).apply {
            layoutParams = ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
            )
            setBackgroundColor(Color.BLACK)
            addView(surfaceContainer)
            addView(view)
        }
        super.setContentView(container)
    }

    override fun onMethodCall(call: MethodCall, result: MethodChannel.Result) {
        Log.i(TAG, "onMethodCall ${call.method}")
        val arguments = call.arguments as Map<*, *>
        when (call.method) {
            "attachWindow" -> {
                val handle = arguments["handle"] as Int
                attachWindow(FkImage(handle.toLong()))
                result.success(0)
                return
            }

            "detachWindow" -> {
                val handle = arguments["handle"] as Int
                detachWindow(FkImage(handle.toLong()))
                result.success(0)
                return
            }
            "convertImage" -> {
                val path = arguments["path"] as String
                if (path.lastIndexOf(".heic") != -1) {
                    val opt = BitmapFactory.Options().apply {
                    }
                    val pathTmp = "${path}.png"
                    val bitmap = BitmapFactory.decodeFile(path, opt)
                    FileOutputStream(pathTmp).use { fos ->
                        try {
                            bitmap.compress(Bitmap.CompressFormat.PNG, 100, fos)
                            result.success(pathTmp)
                        } catch (e: Exception) {
                            e.printStackTrace()
                            result.error("-1", "Create temp file error.", e.toString())
                        }
                    }
                    bitmap.recycle()
                } else {
                    result.success(path)
                }
                return
            }
            "openCamera" -> {
                val handle = arguments["handle"] as Int
                val ret = openCamera(handle)
                if (ret.isSuccess()) {
                    result.success(ret.code)
                } else {
                    result.error(ret.code.toString(), ret.msg, null)
                }
                return
            }
            "closeCamera" -> {
                val handle = arguments["handle"] as Int
                val ret = closeCamera(handle)
                if (ret >= 0) {
                    result.success(ret)
                } else {
                    result.error(ret.toString(), "Close camera fail", null)
                }
                return
            }
            "switchCamera" -> {
                camera?.let {
                    val listener = object :OnInfoListener{
                        override fun onInfo(what: Int, arg0: Int, arg1: Any?, msg: String) {
                            it.removeOnInfoListener(this)
                            cameraSettings.apply {
                                facing =
                                    if (facing == FkCameraFeatures.kFacing.Back) FkCameraFeatures.kFacing.Front else FkCameraFeatures.kFacing.Back
                            }
                            val ret = it.start(cameraSettings)
                            result.success(ret)
                        }
                    }
                    it.addOnInfoListener(listener)
                    it.stop()
                }
                return
            }
            "capture" -> {
                val handle = arguments["handle"] as Int
                camera?.takePicture(object : OnCaptureListener {
                    override fun onResult(source: FkAbsImageSource2?) {
                        if (source == null) {
                            result.error("-1", "Capture fail", null)
                        } else {
                            val engine = FkImage(handle.toLong())
                            val layerId = engine.newLayerWithSource(source)
                            result.success(layerId)
                        }
                    }
                })
                return
            }
            "onCameraExposureUpdate" -> {
                val expValue = arguments["exposure_value"] as Int?
                runOnUiThread { expValue?.let { camera?.updateExposure(expValue) } }
                return
            }
            "reqTest" -> {
                val handle = arguments["handle"] as Int
                val path = "/sdcard/Android/data/com.alimin.fk_flutter/files/wide-gamut-tests-master/P3-sRGB-red.png"
//                val path = "/sdcard/Android/data/com.alimin.fk_flutter/files/P3_1714017400378.png"
                BitmapFactory.decodeFile(path)
                    ?.let {
                        FkLogcat.i(
                            "FkBitmap",
                            "colorSpace=${it.byteCount}, colorSpace=${it.colorSpace?.name}"
                        )
                        val buffer = ByteBuffer.allocateDirect(it.byteCount)
                        it.copyPixelsToBuffer(buffer)
                        val source = FkBitmapSource(buffer, it.width, it.height)
                        val engine = FkImage(handle.toLong())
//                        val layerId = engine.newLayerWithSource(source)
//                        FkLogcat.i("FkBitmap", "FkBitmapSource layerId=$layerId")
//                        result.success(layerId)
                    }
                return
            }
        }
        result.error("-1", "Not support!", null)
    }

    private fun attachWindow(engine: FkImage) {
        renderView = TextureView(this).apply {
            tag = engine
            surfaceTextureListener = this@MainActivity
        }
        surfaceContainer.addView(renderView)
    }

    private fun detachWindow(engine: FkImage) {
        surfaceContainer.removeView(renderView)
        renderView = null
    }

    private fun openCamera(handle: Int): FkResult {
        if (cameraLayer > 0) {
            return FkResult(cameraLayer, "Skip open camera")
        }
        cameraManager = context.getSystemService(Context.CAMERA_SERVICE) as CameraManager
        camera = FkCamera2(cameraManager!!)
        camera?.let {
            val engine = FkImage(handle.toLong())
            it.getImageSource().addOnRenderListener(object : FkAbsImageSource2.OnRenderListener{
                override fun onCreate() {
                }

                override fun onDestroy() {
                }

                override fun onRender() {
                    engine.notifyRender()
                    runOnUiThread { methodChannel?.invokeMethod("onCameraFrameUpdated", null) }
                }
            })
            it.create()
            it.start(cameraSettings)
            cameraLayer = engine.newLayerWithSource(it.getImageSource())
            return FkResult(cameraLayer, "Open camera success")
        }
        return FkResult.NPE
    }

    private fun closeCamera(handle: Int): Int {
        removeLayer(handle, cameraLayer)
        return 0
    }

    private fun removeLayer(handle: Int, layerId: Int) {
        if (layerId < 0) {
            return
        }
        val engine = FkImage(handle.toLong())
        val ret = engine.removeLayer(layerId)
        if (FkResult.OK == ret) {
            engine.notifyRender()
        }
        if (cameraLayer == layerId) {
            camera?.stop()
            camera?.destroy()
            camera = null
            cameraLayer = -1
            cameraManager = null
        }
    }

    override fun onSurfaceTextureAvailable(p0: SurfaceTexture, width: Int, height: Int) {
        renderView?.apply {
            Log.i(TAG, "attachToSurface")
            surface = Surface(p0)
            val engine = tag as FkImage
            engine.attachToSurface(surface, kScaleType.CENTER_INSIDE)
            engine.setCanvasSize(width, height)
            methodChannel?.invokeMethod("attachToSurfaceDone", null)
        }
    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        renderView?.apply {
            Log.i(TAG, "detachFromSurface")
            val engine = tag as FkImage
            engine.detachFromSurface(surface)
            methodChannel?.invokeMethod("detachFromSurfaceDone", null)
            surface?.release()
            surface = null
            return true
        }
        return false
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }
}
