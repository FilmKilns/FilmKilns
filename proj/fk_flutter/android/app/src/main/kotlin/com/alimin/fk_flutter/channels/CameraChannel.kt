package com.alimin.fk_flutter.channels

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import com.alimin.fk.entity.FkCameraFeatureKey
import com.alimin.fk.entity.FkResult
import com.alimin.fk.source.FkBitmapSource
import com.alimin.fk.utils.FkLogcat
import com.alimin.fk_flutter.module.image.ImageContract
import io.flutter.plugin.common.BinaryMessenger
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import java.io.FileOutputStream
import java.nio.ByteBuffer

class CameraChannel(
    messenger: BinaryMessenger,
    private val applicationContext: Context,
    private val presenter: ImageContract.Presenter
) : MethodChannel.MethodCallHandler {
    companion object {
        const val TAG = "CameraChannel"
    }
    private val methodChannel = MethodChannel(messenger, "com.alimin.flutter/media_editor")

    init {
        Log.i(TAG, "Create media_editor channel")
        methodChannel.setMethodCallHandler(this@CameraChannel)
    }

    fun deliveryInfo(result: FkResult) {
        methodChannel.invokeMethod(
            "onDeliveryInfo",
            mapOf("Code" to result.code, "Msg" to result.msg)
        )
    }

    fun deliveryTip(result: FkResult, isError: Boolean) {
        methodChannel.invokeMethod(
            "onDeliveryTip",
            mapOf("Code" to result.code, "Msg" to result.msg, "isError" to isError)
        )
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

            "savePicture" -> {
                presenter.savePicture()
                result.success(FkResult.OK.code)
                return
            }

            "openCamera" -> {
                presenter.openCamera()
                result.success(FkResult.OK.code)
                return
            }

            "closeCamera" -> {
                val expValue = arguments["exposure_value"] as Int?
                presenter.closeCamera()
                result.success(FkResult.OK.code)
                return
            }

            "switchCamera" -> {
                presenter.switchCamera()
                result.success(FkResult.OK.code)
                return
            }

            "capture" -> {
                presenter.takePicture()
                result.success(FkResult.OK.code)
                return
            }

            "onCameraExposureUpdate" -> {
                val expValue = arguments["exposure_value"] as Int?
//                runOnUiThread { expValue?.let { camera?.updateExposure(expValue) } }
                return
            }
            "setFlashMode" -> {
                val mode = arguments["mode"] as Int?
                mode?.let {
                    when (mode) {
                        1 -> presenter.activateFeatures(arrayOf(FkCameraFeatureKey.FLASH_AUTO))
                        2 -> presenter.activateFeatures(arrayOf(FkCameraFeatureKey.FLASH_ON))
                        else -> presenter.activateFeatures(arrayOf(FkCameraFeatureKey.FLASH_OFF))
                    }
                }
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