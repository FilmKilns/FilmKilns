package com.alimin.fk

import android.app.Application
import android.content.Context
import android.hardware.camera2.CameraManager
import android.util.Size
import androidx.test.core.app.ApplicationProvider
import androidx.test.runner.AndroidJUnit4
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.device.FkCamera2
import com.alimin.fk.engine.FkImage
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import org.junit.Assert
import org.junit.Test
import org.junit.runner.RunWith
import java.io.File

@RunWith(AndroidJUnit4::class)
class FkCameraTest {
    @Test
    fun mainTest() {
        val context = ApplicationProvider.getApplicationContext<Application>()
        FilmKilns.init(context)

        val cacheDir = File(context.externalCacheDir, "/test.fkp.dir")
        val engine = FkImage(cacheDir.absolutePath)
        engine.create()
        engine.start()
        engine.setCanvasSize(1080, 1920)

        val cameraSettings = FkCameraSettings(FkCameraFeatures.kFacing.Front, Size(1080, 1440), Size(3072, 4096)).apply {
//        reqCameraKeys.add(FkCameraAvailableKey.SCENE_AUTO_EXT)
        }
        val cameraManager = context.getSystemService(Context.CAMERA_SERVICE) as CameraManager
        val camera = FkCamera2(cameraManager)
        var cameraLayer = -1
        var cntOfFrame = 0
        camera.let {
            it.getImageSource().addOnRenderListener(object : FkAbsImageSource.OnRenderListener{
                override fun onCreate() {
                }

                override fun onDestroy() {
                }

                override fun onRender() {
                    engine.notifyRender()
                    ++cntOfFrame
                }
            })
            it.create()
            it.start(cameraSettings)
            cameraLayer = engine.newLayerWithSource(it.getImageSource())
        }
        Thread.sleep(10000)

        if (cameraLayer > 0) {
            engine.removeLayer(cameraLayer)
            cameraLayer = -1
        }
        camera.stop()
        engine.stop()

        camera.destroy()
        engine.destroy()
        cacheDir.deleteRecursively()
        Assert.assertTrue("cntOfFrame too less.", cntOfFrame >= 100)
    }
}