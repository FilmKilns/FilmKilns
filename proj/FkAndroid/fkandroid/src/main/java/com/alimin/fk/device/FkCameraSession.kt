package com.alimin.fk.device

import android.hardware.camera2.CameraAccessException
import android.hardware.camera2.CameraCaptureSession
import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CaptureRequest
import android.hardware.camera2.TotalCaptureResult
import android.os.Handler
import android.util.Log
import android.view.Surface
import com.alimin.fk.utils.FkLogcat

class FkCameraSession(
    cameraDevice: CameraDevice?,
    surfaces: MutableList<Surface>,
    handler: Handler,
    callback: FkAbsCameraSession.StateCallback): FkAbsCameraSession {
    private var session: CameraCaptureSession? = null

    companion object {
        private val TAG = "FkCameraSession"
    }

    init {
        try {
            cameraDevice?.createCaptureSession(surfaces, object: CameraCaptureSession.StateCallback() {
                override fun onConfigured(session: CameraCaptureSession) {
                    FkLogcat.i(TAG, "onConfigured")
                    this@FkCameraSession.session = session
                    callback.onConfigured(this@FkCameraSession)
                }

                override fun onConfigureFailed(session: CameraCaptureSession) {
                    FkLogcat.e(TAG, "onConfigureFailed")
                    callback.onConfigureFailed(this@FkCameraSession)
                }

                override fun onSurfacePrepared(session: CameraCaptureSession, surface: Surface) {
                    FkLogcat.e(TAG, "onSurfacePrepared")
                    super.onSurfacePrepared(session, surface)
                }

                override fun onClosed(session: CameraCaptureSession) {
                    super.onClosed(session)
                    this@FkCameraSession.session = null
                }
            }, handler)
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    override fun abortCaptures() {
        session?.abortCaptures()
    }

    override fun stopRepeating() {
        session?.stopRepeating()
    }

    override fun setRepeatingRequest(request: CaptureRequest, callback: FkAbsCameraSession.CaptureCallback, handler: Handler) {
        session?.setRepeatingRequest(request, object : CameraCaptureSession.CaptureCallback() {
            override fun onCaptureCompleted(
                session: CameraCaptureSession,
                request: CaptureRequest,
                result: TotalCaptureResult
            ) {
                super.onCaptureCompleted(session, request, result)
                callback.onCaptureCompleted(this@FkCameraSession, request, result)
            }
        }, handler)
    }

    override fun close() {
        session?.close()
    }

    override fun capture(request: CaptureRequest, callback: FkAbsCameraSession.CaptureCallback, handler: Handler) {
        session?.capture(request, object : CameraCaptureSession.CaptureCallback(){
            override fun onCaptureCompleted(
                session: CameraCaptureSession,
                request: CaptureRequest,
                result: TotalCaptureResult
            ) {
                super.onCaptureCompleted(session, request, result)
                callback.onCaptureCompleted(this@FkCameraSession, request, result)
            }
        }, handler)
    }
}