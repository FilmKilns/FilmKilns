package com.alimin.fk.engine

import android.graphics.Point
import android.graphics.Rect
import android.util.Size
import android.view.Choreographer
import android.view.Surface
import androidx.annotation.Keep
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.core.FkPaint
import com.alimin.fk.define.kScaleType
import com.alimin.fk.entity.FkResult

@Keep
class FkImage(handle: Long) : FkEngine(handle) {
    private lateinit var workspace: String
    private var mSyncLock = Object()

    constructor(workspace: String) : this(0) {
        this.workspace = workspace
    }

    override fun onCreateInstance(): Long = nativeCreateInstance(workspace)
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

    fun attachToSurface(surface: Surface?, scaleType: kScaleType): FkResult {
        if (!isNull()) {
            return FkResult(nativeSetSurface(getHandle(), surface, scaleType.ordinal))
        }
        return FkResult.FAIL
    }

    fun detachFromSurface(surface: Surface?) {
        if (!isNull()) {
            nativeSetSurface(getHandle(), null, kScaleType.CENTER_INSIDE.ordinal)
        }
    }

    /**
     * @param path Picture file path
     * @return Layer id.
     */
    fun newLayerWithFile(path: String): Int {
        if (!isNull()) {
            return nativeNewLayerWithFile(getHandle(), path)
        }
        return -1
    }

    /**
     * @return Layer id.
     */
    fun newLayerWithColor(size: Size, red: Int, green: Int, blue: Int, alpha: Int): Int {
        if (!isNull()) {
            return nativeNewLayerWithColor(getHandle(), size.width, size.height, red, green, blue, alpha)
        }
        return -1
    }

    /**
     * @return Layer id.
     */
    fun newLayerWithSource(source: FkAbsImageSource): Int {
        if (!isNull()) {
            return source.with {
                val layerId = nativeNewLayerWithSource(getHandle(), it)
                return@with FkResult(layerId, "layerId")
            }.code
        }
        return -1
    }

    /**
     * Copy srcLayer to layerId
     * @param layerId
     * @param srcLayerId
     * @return Result code.
     */
    fun setProjectionLayer(layerId : Int, srcLayerId : Int): Int {
        if (!isNull()) {
            return nativeSetProjectionLayer(getHandle(), layerId, srcLayerId)
        }
        return -1
    }

    /**
     * @return Result code.
     */
    fun removeLayer(layerId: Int): FkResult {
        if (!isNull()) {
            return FkResult(nativeRemoveLayer(getHandle(), layerId))
        }
        return FkResult.FAIL
    }

//    private var lastTime = 0L
    fun notifyRender(): FkResult {
        if (isNull()) {
            return FkResult.FAIL
        }
        Choreographer.getInstance().postFrameCallback(object : Choreographer.FrameCallback{
            override fun doFrame(frameTimeNanos: Long) {
                synchronized(mSyncLock) {
                    if (!isNull()) {
                        nativeNotifyRender(getHandle())
                    }
                }
            }
        })
    return FkResult.OK
    }

    fun setCanvasSize(width: Int, height: Int): FkResult {
        if (!isNull()) {
            return FkResult(nativeSetCanvasSize(getHandle(), width, height))
        }
        return FkResult.FAIL
    }

    /**
     * @param layerId Layer ID, ID 0 is Canvas.
     * @param dx Delta x position of view
     * @param dy Delta y position of view
     * @return Result code.
     */
    fun postTranslate(layerId: Int, dx: Int, dy: Int): FkResult {
        if (!isNull()) {
            return FkResult(nativePostTranslate(getHandle(), layerId, dx, dy))
        }
        return FkResult.FAIL
    }

    fun postScale(layerId: Int, dx: Float, dy: Float): FkResult {
        if (!isNull()) {
            return FkResult(nativePostScale(getHandle(), layerId, dx, dy))
        }
        return FkResult.FAIL
    }

    fun postRotation(layerId: Int, num: Int, den: Int): FkResult {
        if (!isNull()) {
            return FkResult(nativePostRotation(getHandle(), layerId, num, den))
        }
        return FkResult.FAIL
    }

    /**
     * @param layerId Layer ID, ID 0 is Canvas.
     * @param size  Size of point
     * @param point Point of view
     * @return Result code.
     */
    fun drawPoint(layerId: Int, color: Long, size: Int, point: Point): FkResult {
        if (!isNull()) {
            return FkResult(nativeDrawPoint(getHandle(), layerId, color, size, point.x, point.y))
        }
        return FkResult.FAIL
    }

    fun crop(rect: Rect): FkResult {
        if (!isNull()) {
            return FkResult(nativeCrop(getHandle(), rect.left, rect.top, rect.right, rect.bottom))
        }
        return FkResult.FAIL
    }

    fun cropLayer(layerId: Int, leftTop: Point, rightBottom: Point): FkResult {
        if (!isNull()) {
            return FkResult(nativeCropLayer(getHandle(), layerId, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y))
        }
        return FkResult.FAIL
    }

    fun save(file: String, listener: FkNativeMsgListener): FkResult {
        if (!isNull()) {
            return FkResult(nativeSave(getHandle(), file, listener))
        }
        return FkResult.FAIL
    }

    fun drawPath(layerId: Int, x: Int, y: Int, paint: FkPaint): FkResult {
        if (!isNull()) {
            return FkResult(nativeDrawPath(getHandle(), layerId, x, y, paint.toByteArray()))
        }
        return FkResult.FAIL
    }

    fun drawPathFinish(layerId: Int): FkResult {
        if (!isNull()) {
            return FkResult(nativeDrawPathFinish(getHandle(), layerId))
        }
        return FkResult.FAIL
    }

    private external fun nativeCreateInstance(workspace: String): Long
    private external fun nativeCreate(handle: Long)
    private external fun nativeDestroy(handle: Long)
    private external fun nativeStart(handle: Long)
    private external fun nativeStop(handle: Long)
    private external fun nativeSetSurface(handle: Long, surface: Surface?, scaleType: Int): Int
    private external fun nativeNewLayerWithFile(handle: Long, path: String): Int
    private external fun nativeNewLayerWithColor(
        handle: Long,
        width: Int,
        height: Int,
        red: Int,
        green: Int,
        blue: Int,
        alpha: Int
    ): Int

    private external fun nativeNewLayerWithSource(handle: Long, sourceHandle: Long): Int
    private external fun nativeSetProjectionLayer(handle: Long, layerId: Int, srcLayerId: Int): Int
    private external fun nativeRemoveLayer(handle: Long, layerId: Int): Int
    private external fun nativeSetCanvasSize(handle: Long, width: Int, height: Int): Int

    private external fun nativeNotifyRender(handle: Long): Int
    private external fun nativePostTranslate(handle: Long, layerId: Int, dx: Int, dy: Int): Int
    private external fun nativePostScale(handle: Long, layerId: Int, dx: Float, dy: Float): Int
    private external fun nativePostRotation(handle: Long, layerId: Int, num: Int, den: Int): Int
    private external fun nativeDrawPoint(handle: Long, layerId: Int, color: Long, size: Int, x: Int, y: Int): Int
    private external fun nativeCrop(handle: Long, left: Int, top: Int, right: Int, bottom: Int): Int
    private external fun nativeCropLayer(handle: Long, layerId: Int, left: Int, top: Int, right: Int, bottom: Int): Int
    private external fun nativeSave(handle: Long, file: String, listener: FkNativeMsgListener): Int
    private external fun nativeDrawPath(handle: Long, layerId: Int, x: Int, y: Int, paintData: ByteArray): Int
    private external fun nativeDrawPathFinish(handle: Long, layerId: Int): Int
}