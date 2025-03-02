package com.alimin.fk_flutter.channels

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import com.alimin.fk.source.FkBitmapSource
import com.alimin.fk.utils.FkLogcat
import com.alimin.fk_flutter.MainActivity.Companion.TAG
import com.alimin.fk_flutter.module.image.ImageContract
import io.flutter.plugin.common.BinaryMessenger
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import java.io.FileOutputStream
import java.nio.ByteBuffer

class CameraChannel(
    messenger: BinaryMessenger,
    val applicationContext: Context,
    val presenter: ImageContract.Presenter
) : MethodChannel.MethodCallHandler {
    init {
        Log.i(TAG, "Create media_editor channel")
        val methodChannel = MethodChannel(messenger, "com.alimin.flutter/media_editor")
        methodChannel.setMethodCallHandler(this@CameraChannel)
    }

    override fun onMethodCall(call: MethodCall, result: MethodChannel.Result) {
        Log.i(TAG, "onMethodCall ${call.method}")
        val arguments = call.arguments as Map<*, *>
        when (call.method) {
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
                presenter.openCamera(applicationContext)
                return
            }

            "closeCamera" -> {
                presenter.closeCamera()
                return
            }

            "switchCamera" -> {
                presenter.switchCamera()
                return
            }

            "capture" -> {
                presenter.takePicture()
                return
            }

            "onCameraExposureUpdate" -> {
                val expValue = arguments["exposure_value"] as Int?
//                runOnUiThread { expValue?.let { camera?.updateExposure(expValue) } }
                return
            }

            "reqTest" -> {
                val handle = arguments["handle"] as Int
                val path =
                    "/sdcard/Android/data/com.alimin.fk_flutter/files/wide-gamut-tests-master/P3-sRGB-red.png"
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
//                        val layerId = presenter.newLayerWithSource(source)
//                        FkLogcat.i("FkBitmap", "FkBitmapSource layerId=$layerId")
//                        result.success(layerId)
                    }
                return
            }
        }
        result.error("-1", "Not support!", null)
    }
}