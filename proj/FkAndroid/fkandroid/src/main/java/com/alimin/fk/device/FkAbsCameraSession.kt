package com.alimin.fk.device

import android.hardware.camera2.CaptureRequest
import android.hardware.camera2.TotalCaptureResult
import android.os.Handler

interface FkAbsCameraSession {
    interface StateCallback {
        fun onConfigured(session: FkAbsCameraSession)
        fun onConfigureFailed(session: FkAbsCameraSession)
    }

    interface CaptureCallback {
        fun onCaptureCompleted(
            session: FkAbsCameraSession,
            request: CaptureRequest,
            result: TotalCaptureResult
        )
    }
    fun abortCaptures()
    fun stopRepeating()
    fun setRepeatingRequest(request: CaptureRequest, callback: CaptureCallback, handler: Handler)
    fun close()
    fun capture(request: CaptureRequest, callback: CaptureCallback, handler: Handler)
}