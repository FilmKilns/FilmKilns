package com.alimin.fk_flutter

import android.content.Intent
import android.graphics.Color
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.Toast
import com.alimin.fk.FilmKilns
import com.alimin.fk.engine.FkImage
import com.alimin.fk.entity.FkResult
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
        ImagePresenter(this, workspace.absolutePath)
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

    private fun detachWindow(engine: FkImage) {
        surfaceContainer.removeView(renderView)
        renderView = null
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

    override fun onStart() {
        super.onStart()
        presenter.start()
//        if (pickImagePath?.isNotEmpty() == true) {
//            presenter.newLayerWithFile(pickImagePath!!)
//            pickImagePath = null
//        }
    }

    override fun onStop() {
        super.onStop()
        presenter.stop()
    }

    override fun onDestroy() {
        super.onDestroy()
        presenter.destroy()
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }

    override fun onImageSaved(file: String) {
    }

    override fun onImageSaving() {
    }

    override fun showError(error: Int, msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_LONG).show()
    }

    override fun onCameraInfo(result: FkResult) {
        Toast.makeText(this, "${result.code}: ${result.msg}", Toast.LENGTH_LONG).show()
    }
}
