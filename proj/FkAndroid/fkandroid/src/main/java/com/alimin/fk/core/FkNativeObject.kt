package com.alimin.fk.core

import com.alimin.fk.entity.FkResult

abstract class FkNativeObject {
    private var handle = 0L
    fun isNull(): Boolean = handle == 0L
    abstract fun onNativeCreate(): Long
    abstract fun onNativeDestroy(handle: Long): Boolean
    open fun create(): FkResult {
        val _handle = onNativeCreate()
        if (_handle != 0L) {
            synchronized(this) {
                handle = _handle
            }
            return FkResult.OK
        }
        return FkResult.INVALID_DATA
    }

    open fun destroy(): FkResult {
        synchronized(this) {
            if (isNull()) {
                return FkResult.INVALID_STATE
            }
            val _handle = handle
            handle = 0L
            val state = onNativeDestroy(_handle)
            if (state) {
                return FkResult.OK
            }
        }
        return FkResult.FAIL
    }

    fun with(task: (handle: Long) -> FkResult): FkResult {
        synchronized(this) {
            if (isNull()) {
                return FkResult.INVALID_DATA
            }
            return task(handle)
        }
    }

    protected fun finalize() {
        destroy()
    }
}