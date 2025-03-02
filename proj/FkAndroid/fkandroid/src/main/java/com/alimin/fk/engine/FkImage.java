package com.alimin.fk.engine;

import android.graphics.Point;
import android.graphics.Rect;
import android.util.Size;
import android.view.Choreographer;
import android.view.Surface;

import androidx.annotation.Keep;
import androidx.annotation.Nullable;

import com.alimin.fk.core.FkAbsImageSource;
import com.alimin.fk.core.FkPaint;
import com.alimin.fk.define.kScaleType;
import com.alimin.fk.entity.FkResult;
import com.alimin.fk.listener.FkNativeMsgListener;
import com.filmkilns.annotation.FkNativeAuto;

import kotlin.jvm.functions.Function1;

@Keep
@FkNativeAuto(path = "cpp/native/engine")
public class FkImage extends FkEngine {
    private String workspace;

    public FkImage(String workspace) {
        this.workspace = workspace;
    }

    public FkImage(long _handle) {
    }

    @Override
    public long onNativeCreate() {
        long handle = nativeCreateInstance(workspace);
        if (handle != 0) {
            nativeCreate(handle);
        }
        return handle;
    }

    @Override
    public boolean onNativeDestroy(long handle) {
        nativeDestroy(handle);
        return true;
    }

    @Override
    public FkResult start() {
        super.start();
        return with(handle -> {
            nativeStart(handle);
            return FkResult.Companion.getOK();
        });
    }

    @Override
    public FkResult stop() {
        super.stop();
        return with(handle -> {
            nativeStop(handle);
            return FkResult.Companion.getOK();
        });
    }

    public FkResult attachToSurface(@Nullable Surface surface, kScaleType scaleType) {
        return withInt(handle -> nativeSetSurface(handle, surface, scaleType.ordinal()));
    }

    public FkResult detachFromSurface(@Nullable Surface surface) {
        return withInt(handle -> nativeSetSurface(handle, surface, kScaleType.CENTER_INSIDE.ordinal()));
    }

    /**
     * @param path Picture file path
     * @return Layer id.
     */
    public int newLayerWithFile(String path) {
        return withInt(handle -> nativeNewLayerWithFile(handle, path)).getCode();
    }

    /**
     * @return Layer id.
     */
    public int newLayerWithColor(Size size, int red, int green, int blue, int alpha) {
        return withInt(handle -> nativeNewLayerWithColor(handle, size.getWidth(), size.getHeight(), red, green, blue, alpha)).getCode();
    }

    /**
     * @return Layer id.
     */
    public int newLayerWithSource(FkAbsImageSource source) {
        return withInt(handle -> source.withInt(new Function1<Long, Integer>() {
            @Override
            public Integer invoke(Long sourceHandle) {
                return nativeNewLayerWithSource(handle, sourceHandle);
            }
        }).getCode()).getCode();
    }

    /**
     * Copy srcLayer to layerId
     *
     * @param layerId
     * @param srcLayerId
     * @return Result code.
     */
    public FkResult setProjectionLayer(int layerId, int srcLayerId) {
        return withInt(handle -> nativeSetProjectionLayer(handle, layerId, srcLayerId));
    }

    /**
     * @return Result code.
     */
    public FkResult removeLayer(int layerId) {
        return withInt(handle -> nativeRemoveLayer(handle, layerId));
    }

    //    private var lastTime = 0L
    public FkResult notifyRender() {
        return withInt(new Function1<Long, Integer>() {
            @Override
            public Integer invoke(Long handle) {
                Choreographer.getInstance().postFrameCallback(new Choreographer.FrameCallback() {
                    @Override
                    public void doFrame(long frameTimeNanos) {
                        synchronized (FkImage.this) {
                            if (!isNull()) {
                                nativeNotifyRender(handle);
                            }
                        }
                    }
                });
                return FkResult.Companion.getOK().getCode();
            }
        });
    }

    public FkResult setCanvasSize(int width, int height) {
        return withInt(handle -> nativeSetCanvasSize(handle, width, height));
    }

    /**
     * @param layerId Layer ID, ID 0 is Canvas.
     * @param dx      Delta x position of view
     * @param dy      Delta y position of view
     * @return Result code.
     */
    public FkResult postTranslate(int layerId, int dx, int dy) {
        return withInt(handle -> nativePostTranslate(handle, layerId, dx, dy));
    }

    public FkResult postScale(int layerId, float dx, float dy) {
        return withInt(handle -> nativePostScale(handle, layerId, dx, dy));
    }

    public FkResult postRotation(int layerId, int num, int den) {
        return withInt(handle -> nativePostRotation(handle, layerId, num, den));
    }

    /**
     * @param layerId Layer ID, ID 0 is Canvas.
     * @param size    Size of point
     * @param point   Point of view
     * @return Result code.
     */
    public FkResult drawPoint(int layerId, long color, int size, Point point) {
        return withInt(handle -> nativeDrawPoint(handle, layerId, color, size, point.x, point.y));
    }

    public FkResult crop(Rect rect) {
        return withInt(handle -> nativeCrop(handle, rect.left, rect.top, rect.right, rect.bottom));
    }

    public FkResult cropLayer(int layerId, Point leftTop, Point rightBottom) {
        return withInt(handle -> nativeCropLayer(handle, layerId, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y));
    }

    public FkResult save(String file, FkNativeMsgListener listener) {
        return withInt(handle -> nativeSave(handle, file, listener));
    }

    public FkResult drawPath(int layerId, int x, int y, FkPaint paint) {
        return withInt(handle -> nativeDrawPath(handle, layerId, x, y, paint.toByteArray()));
    }

    public FkResult drawPathFinish(int layerId) {
        return withInt(handle -> nativeDrawPathFinish(handle, layerId));
    }

    /**
     * +----------------------------------------+
     * |          START native methods          |
     * +----------------------------------------+
     */
    private native long nativeCreateInstance(String workspace);

    private native void nativeCreate(long handle);

    private native void nativeDestroy(long handle);

    private native void nativeStart(long handle);

    private native void nativeStop(long handle);

    private native int nativeSetSurface(long handle, Surface surface, int scaleType);

    private native int nativeNewLayerWithFile(long handle, String path);

    private native int nativeNewLayerWithColor(long handle, int width, int height,
                                               int red, int green, int blue, int alpha);

    private native int nativeNewLayerWithSource(long handle, long sourceHandle);

    private native int nativeSetProjectionLayer(long handle, int layerId, int srcLayerId);

    private native int nativeRemoveLayer(long handle, int layerId);

    private native int nativeSetCanvasSize(long handle, int width, int height);

    private native int nativeNotifyRender(long handle);

    private native int nativePostTranslate(long handle, int layerId, int dx, int dy);

    private native int nativePostScale(long handle, int layerId, float dx, float dy);

    private native int nativePostRotation(long handle, int layerId, int num, int den);

    private native int nativeDrawPoint(long handle, int layerId, long color, int size, int x, int y);

    private native int nativeCrop(long handle, int left, int top, int right, int bottom);

    private native int nativeCropLayer(long handle, int layerId, int left, int top, int right, int bottom);

    private native int nativeSave(long handle, String file, FkNativeMsgListener listener);

    private native int nativeDrawPath(long handle, int layerId, int x, int y, byte[] paintData);

    private native int nativeDrawPathFinish(long handle, int layerId);
    /**
     * +----------------------------------------+
     * |           END native methods           |
     * +----------------------------------------+
     */
}
