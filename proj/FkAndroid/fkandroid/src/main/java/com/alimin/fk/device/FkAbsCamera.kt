package com.alimin.fk.device

import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import com.alimin.fk.entity.FkResult

abstract class FkAbsCamera {
    private val infoListeners = ArrayList<OnInfoListener>()

    abstract fun create(): Int
    abstract fun destroy(): Int
    abstract fun start(settings: FkCameraSettings): Int
    abstract fun stop(): Int
    abstract fun getImageSource(): FkAbsImageSource
    abstract fun takePicture(listener: OnCaptureListener): Int
    abstract fun getFeatures(): List<FkCameraFeatures>
    abstract fun updateExposure(curExpValue: Int): Int

    open fun addOnInfoListener(listener: OnInfoListener) {
        synchronized(infoListeners) {
            infoListeners.add(listener)
        }
    }

    open fun removeOnInfoListener(listener: OnInfoListener) {
        synchronized(infoListeners) {
            infoListeners.remove(listener)
        }
    }

    open fun dispatchInfo(result: FkResult, arg0: Int, arg1: Any?) {
        val listeners = ArrayList<OnInfoListener>()
        synchronized(infoListeners) {
            infoListeners.forEach { listeners.add(it) }
        }
        listeners.forEach {
            it.onInfo(result.code, arg0, arg1, result.msg)
        }
    }
}

interface OnInfoListener {
    fun onInfo(what: Int, arg0: Int, arg1: Any?, msg: String)
}

interface OnCaptureListener {
    fun onResult(source: FkAbsImageSource?)
}

enum class FkCameraState {
    IDLE,
    CREATING,
    CREATED,
    STARTING,
    STARTED,
    STOPPING,
    STOPPED,
    DESTROYING,
    DESTROYED
}