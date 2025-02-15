package com.alimin.fk.engine

import androidx.annotation.Keep
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.alimin.fk.pb.FkPictureModelOuterClass
import java.nio.ByteBuffer

interface FkNativeMsgListener {
    fun onNativeMsgReceived(what: Int, arg: Int, msg: String?, pbObject: ByteBuffer?): Boolean
}

interface FkGetLayersListener {
    fun onGetLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>)
}

interface OnDoStatusListener {
    fun onDone()
}

@Keep
class FkImageModel(val engine: FkImage) : FkEngine(0) {
    private var mSyncLock = Object()
    override fun onCreateInstance(): Long = nativeCreateInstance(engine.getHandle())
    override fun create() {
        super.create()
        if (!isNull()) {
            nativeCreate(getHandle())
        }
    }

    override fun destroy() {
        super.destroy()
        synchronized(mSyncLock) {
            if (!isNull()) {
                nativeDestroy(getHandle())
                releaseHandle()
            }
        }
    }

    override fun start() {
        super.start()
        if (!isNull()) {
            nativeStart(getHandle())
        }
    }

    override fun stop() {
        super.stop()
        if (!isNull()) {
            nativeStop(getHandle())
        }
    }

    fun save(file: String): Int {
        if (!isNull()) {
            return nativeSave(getHandle(), file)
        }
        return -1
    }

    fun load(file: String, listener: OnDoStatusListener): Int {
        if (!isNull()) {
            return nativeLoad(getHandle(), file, object : FkNativeMsgListener {
                override fun onNativeMsgReceived(
                    what: Int,
                    arg: Int,
                    msg: String?,
                    pbObject: ByteBuffer?
                ): Boolean {
//                    if (FkResult(what) == FkResult.FK_OK) {
                        listener.onDone()
//                    }
                    return true
                }
            })
        }
        return -1
    }

    fun countOfLayer(): Int {
        return 0
    }

    fun getLayers(listener: FkGetLayersListener): Int {
        if (!isNull()) {
            return nativeGetLayers(getHandle(), object : FkNativeMsgListener {
                override fun onNativeMsgReceived(what: Int, arg: Int, msg: String?, pbObject: ByteBuffer?): Boolean {
                    if (null != pbObject) {
                        val model =
                            FkPictureModelOuterClass.FkPictureModel.parseFrom(pbObject)
                        listener.onGetLayers(model.layersList)
                    }
                    return pbObject != null
                }
            })
        }
        return -1
    }

    fun getLayer(layer: Int) {

    }

    private external fun nativeCreateInstance(imageEngineHandle: Long): Long
    private external fun nativeCreate(handle: Long)
    private external fun nativeDestroy(handle: Long)
    private external fun nativeStart(handle: Long)
    private external fun nativeStop(handle: Long)

    private external fun nativeSave(handle: Long, file: String): Int
    private external fun nativeLoad(handle: Long, file: String, listener: FkNativeMsgListener): Int
    private external fun nativeGetLayers(handle: Long, listener: FkNativeMsgListener): Int
}