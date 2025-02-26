package com.alimin.fk.device

import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CameraExtensionCharacteristics
import android.hardware.camera2.CameraExtensionSession
import android.hardware.camera2.CaptureRequest
import android.hardware.camera2.TotalCaptureResult
import android.hardware.camera2.params.ExtensionSessionConfiguration
import android.hardware.camera2.params.OutputConfiguration
import android.os.Build
import android.os.Handler
import android.util.Log
import android.view.Surface
import com.alimin.fk.utils.FkLogcat
import java.util.concurrent.Executor

class FkCameraExtSession(
    cameraDevice: CameraDevice?,
    surfaces: MutableList<Surface>,
    handler: Handler,
    callback: FkAbsCameraSession.StateCallback
) : FkAbsCameraSession {
    private var session: CameraExtensionSession? = null

    companion object {
        private val TAG = "FkCameraExtSession"
    }

    init {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val stateCallback = object : CameraExtensionSession.StateCallback() {
                override fun onConfigured(session: CameraExtensionSession) {
                    FkLogcat.i(TAG, "onConfigured")
                    this@FkCameraExtSession.session = session
                    callback.onConfigured(this@FkCameraExtSession)
                }

                override fun onConfigureFailed(session: CameraExtensionSession) {
                    FkLogcat.i(TAG, "onConfigureFailed")
                    callback.onConfigureFailed(this@FkCameraExtSession)
                }

                override fun onClosed(session: CameraExtensionSession) {
                    super.onClosed(session)
                    FkLogcat.i(TAG, "onClosed")
                    this@FkCameraExtSession.session = null
                }
            }
            val executor = Executor { command -> handler.post(command) }
            val outputs = surfaces.map { OutputConfiguration(it) }
            cameraDevice?.createExtensionSession(
                ExtensionSessionConfiguration(
                    CameraExtensionCharacteristics.EXTENSION_AUTOMATIC,
                    outputs,
                    executor,
                    stateCallback
                )
            )
        }
    }


    override fun abortCaptures() {
        FkLogcat.i(TAG, "abortCaptures not support.")
    }

    override fun stopRepeating() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            session?.stopRepeating()
        }
    }

    override fun setRepeatingRequest(
        request: CaptureRequest,
        callback: FkAbsCameraSession.CaptureCallback,
        handler: Handler
    ) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val executor = Executor { command -> handler.post(command) }
            session?.setRepeatingRequest(
                request,
                executor,
                object : CameraExtensionSession.ExtensionCaptureCallback() {
                    override fun onCaptureResultAvailable(
                        session: CameraExtensionSession,
                        request: CaptureRequest,
                        result: TotalCaptureResult
                    ) {
                        super.onCaptureResultAvailable(session, request, result)
                        callback.onCaptureCompleted(this@FkCameraExtSession, request, result)
                    }
                })
        }
    }

    override fun close() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            session?.close()
        }
    }

    override fun capture(
        request: CaptureRequest,
        callback: FkAbsCameraSession.CaptureCallback,
        handler: Handler
    ) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE) {
            session?.realtimeStillCaptureLatency?.let {
                FkLogcat.i(TAG, "Capture latency ${it.captureLatency}, ${it.processingLatency}")
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val executor = Executor { command -> handler.post(command) }
            session?.capture(request, executor, object : CameraExtensionSession.ExtensionCaptureCallback(){
                override fun onCaptureResultAvailable(
                    session: CameraExtensionSession,
                    request: CaptureRequest,
                    result: TotalCaptureResult
                ) {
                    super.onCaptureResultAvailable(session, request, result)
                    callback.onCaptureCompleted(this@FkCameraExtSession, request, result)
                }
            })
        }
    }
}