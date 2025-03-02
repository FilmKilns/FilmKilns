package com.alimin.fk_flutter

import android.content.Intent
import android.graphics.Color
import android.graphics.Rect
import android.graphics.SurfaceTexture
import android.os.Build
import android.os.Bundle
import android.util.DisplayMetrics
import android.util.Log
import android.util.Size
import android.view.Surface
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.view.WindowInsets
import android.view.WindowMetrics
import android.widget.FrameLayout
import android.widget.Toast
import com.alimin.fk.FilmKilns
import com.alimin.fk.entity.FkResult
import com.alimin.fk.utils.FkLogcat
import com.alimin.fk_flutter.channels.CameraChannel
import com.alimin.fk_flutter.module.image.ImageContract
import com.alimin.fk_flutter.module.image.ImagePresenter
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.android.FlutterActivityLaunchConfigs
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel
import java.io.File

class MainActivity : FlutterActivity(), ImageContract.View, TextureView.SurfaceTextureListener {
    override val isActive: Boolean
        get() = true
    override lateinit var presenter: ImageContract.Presenter
    lateinit var cameraChannel: CameraChannel
    private lateinit var container: FrameLayout
    private lateinit var surfaceContainer: FrameLayout
    private var renderView: TextureView? = null
    private var surface: Surface? = null
    private var methodChannel: MethodChannel? = null

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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        FilmKilns.init(applicationContext)
        val workspace = File(externalCacheDir, "workspace")
        if (!workspace.exists()) {
            workspace.mkdirs()
        }
        ImagePresenter(this, workspace.absolutePath, getScreenResolution())
        presenter.create()
        flutterEngine?.apply {
            cameraChannel = CameraChannel(dartExecutor, applicationContext, presenter)
        }
        Log.i(TAG, "transparencyMode=${transparencyMode}")
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
        renderView = TextureView(this).apply {
            surfaceTextureListener = this@MainActivity
        }
        surfaceContainer.addView(renderView)
        super.setContentView(container)
    }

    private fun getScreenResolution(): Size {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val windowMetrics: WindowMetrics = windowManager.currentWindowMetrics
            val windowInsets: WindowInsets = windowMetrics.getWindowInsets()
            val bounds: Rect = windowMetrics.getBounds()
            val width = bounds.width()
            val height = bounds.height()
            return Size(width, height)
        } else {
            val displayMetrics = DisplayMetrics()
            windowManager.defaultDisplay.getMetrics(displayMetrics)
            val width = displayMetrics.widthPixels
            val height = displayMetrics.heightPixels
            return Size(width, height)
        }
    }

    override fun onSurfaceTextureAvailable(p0: SurfaceTexture, width: Int, height: Int) {
        renderView?.apply {
            Log.i(TAG, "attachToSurface")
            surface = Surface(p0)
            presenter.attachToSurface(surface!!)
//            presenter.setCanvasSize(width, height)
            presenter.notifyRender()
            methodChannel?.invokeMethod("attachToSurfaceDone", null)
        }
    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
        Log.i(TAG, "onSurfaceTextureSizeChanged")
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        renderView?.apply {
            Log.i(TAG, "detachFromSurface")
            surface?.let { presenter.detachFromSurface(it) }
            methodChannel?.invokeMethod("detachFromSurfaceDone", null)
            surface?.release()
            surface = null
            return true
        }
        return false
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }

    private fun attachWindow() {
        if (renderView == null) {
            renderView = TextureView(this).apply {
                surfaceTextureListener = this@MainActivity
            }
            surfaceContainer.addView(renderView)
        }
    }

    private fun detachWindow() {
        surfaceContainer.removeView(renderView)
        renderView = null
    }

    override fun onStart() {
        super.onStart()
        Log.i(TAG, "onStart")
        presenter.start()
        attachWindow()
//        if (pickImagePath?.isNotEmpty() == true) {
//            presenter.newLayerWithFile(pickImagePath!!)
//            pickImagePath = null
//        }
    }

    override fun onStop() {
        Log.i(TAG, "onStop")
        detachWindow()
        presenter.stop()
        super.onStop()
    }

    override fun onDestroy() {
        presenter.destroy()
        super.onDestroy()
    }

    override fun onImageSaved(file: String) {
        Toast.makeText(this, "Saved to $file", Toast.LENGTH_LONG).show()
    }

    override fun onImageSaving() {
    }

    override fun showError(error: Int, msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_LONG).show()
    }

    override fun onPresenterInfo(result: FkResult, arg0 : Any?) {
        FkLogcat.i(TAG, "${result.code}: ${result.msg}")
        when (result.code) {
            FkResult.INFO_CAMERA_TAKE_PICTURE_SUCCESS.code -> {
                cameraChannel.deliveryInfo(result)
            }
            FkResult.INFO_LAYER_CNT.code -> {
                FkLogcat.i(TAG, "Layer cnt=$arg0")
            }
            FkResult.INFO_CAMERA_TEST_ACTION.code -> {

            }
        }
    }
}
