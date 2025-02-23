package com.alimin.fk.core

abstract class FkAbsImageSource : FkNativeObject() {
    private val infoListeners = ArrayList<OnRenderListener>()

    abstract fun onCreate(arg0: Int, arg1: Long, arg2: String?): Int
    abstract fun onDestroy(): Int
    abstract fun onRender(arg0: Int, arg1: Long, arg2: String?): Int
    abstract fun getSize(): IntArray
    abstract fun getTimestampInNS(): Long

    open fun addOnRenderListener(l: OnRenderListener) {
        synchronized(infoListeners) {
            infoListeners.add(l)
        }
    }

    open fun removeOnRenderListener(l: OnRenderListener) {
        synchronized(infoListeners) {
            infoListeners.remove(l)
        }
    }

    open fun copyInfoListeners(): List<OnRenderListener> {
        val listeners = ArrayList<OnRenderListener>()
        synchronized(infoListeners) {
            infoListeners.forEach { listeners.add(it) }
        }
        return listeners
    }

    interface OnRenderListener {
        fun onCreate()
        fun onDestroy()
        fun onRender()
    }
}