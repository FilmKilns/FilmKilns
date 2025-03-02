package com.alimin.fk.source;

import android.graphics.Point;
import android.graphics.PointF;
import android.graphics.SurfaceTexture;
import android.view.Surface;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.alimin.fk.core.FkAbsImageSource;
import com.alimin.fk.entity.FkResult;
import com.alimin.fk.utils.FkLogcat;
import com.filmkilns.annotation.FkNativeAuto;

import java.util.ArrayList;
import java.util.List;

@Keep
@FkNativeAuto(path = "cpp/native/source")
public class FkSurfaceTextureSource extends FkAbsImageSource {
    private interface DoneCallback {
        void done(int arg0, long arg1, String arg2);
    }

    private final static String TAG = "FkSurfaceTextureSource";
    private Surface surface;
    private SurfaceTexture surfaceTexture;
    private Point size = new Point(0, 0);
    private final float[] matrix = new float[16];
    private final ArrayList<OnRenderListener> infoListeners = new ArrayList<>();
    private boolean isDestroy = false;
    private boolean reqDestroy = false;
    private final PointF scale = new PointF(1f, 1f);
    private DoneCallback createFunc;
    private long timestampInNS = 0L;

    public FkSurfaceTextureSource() {
        super();
    }

    @NonNull
    @Override
    public FkResult create() {
        FkResult ret = super.create();
        reqDestroy = false;
        surfaceTexture = new SurfaceTexture(0);
        surface = new Surface(surfaceTexture);
        surfaceTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                if (isRendering()) {
                    List<OnRenderListener> listeners = copyInfoListeners();
                    for (OnRenderListener l : listeners) {
                        if (l != null) {
                            l.onRender();
                        }
                    }
                }
            }
        });
        return ret;
    }

    @NonNull
    @Override
    public FkResult destroy() {
        FkResult ret = super.destroy();
        FkLogcat.i(TAG, "destroy");
        reqDestroy = true;
        _destroy();
        return ret;
    }

    private synchronized void _destroy() {
        if (reqDestroy && isDestroy) {
            FkLogcat.i(TAG, "_destroy");
            if (surfaceTexture != null) {
                surfaceTexture.release();
            }
            surfaceTexture = null;
            if (surface != null) {
                surface.release();
            }
            surface = null;
        }
    }

    public synchronized void setSize(int w, int h) {
        size.x = w;
        size.y = h;
    }

    public synchronized void allocBuffer() {
        if (surfaceTexture != null) {
            surfaceTexture.setDefaultBufferSize(size.x, size.y);
        }
        createFunc = this::onCreate;
        FkLogcat.i(TAG, "allocBuffer");
    }

    public synchronized void setScale(float scaleX, float scaleY) {
        scale.x = scaleX;
        scale.y = scaleY;
    }

    public Surface getSurface() {
        return surface;
    }

    @Override
    public int onCreate(int arg0, long arg1, @Nullable String arg2) {
        FkLogcat.i(TAG, "onCreate tex=" + arg0);
        if (arg0 >= 0) {
            synchronized (this) {
                try {
                    if (createFunc != null) {
                        if (surfaceTexture != null) {
                            surfaceTexture.detachFromGLContext();
                            surfaceTexture.attachToGLContext(arg0);
                        }
                        isDestroy = false;
                        createFunc = null;
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            List<OnRenderListener> listeners = copyInfoListeners();
            for (OnRenderListener l : listeners) {
                if (l != null) {
                    l.onCreate();
                }
            }
            return FkResult.Companion.getOK().getCode();
        }
        return FkResult.Companion.getFAIL().getCode();
    }

    @Override
    public int onDestroy() {
        FkLogcat.i(TAG, "onDestroy");
        synchronized (this) {
            isDestroy = true;
        }
        _destroy();
        List<OnRenderListener> listeners = copyInfoListeners();
        for (OnRenderListener l : listeners) {
            if (l != null) {
                l.onDestroy();
            }
        }
        return FkResult.Companion.getOK().getCode();
    }

    @Override
    public int onRender(int arg0, long arg1, @Nullable String arg2) {
        synchronized (this) {
            if (createFunc != null) {
                createFunc.done(arg0, arg1, arg2);
            }
            if (isRendering()) {
                try {
                    if (surfaceTexture != null) {
                        surfaceTexture.updateTexImage();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        if (surfaceTexture != null) {
            timestampInNS = surfaceTexture.getTimestamp();
            if (timestampInNS > 0) {
                surfaceTexture.getTransformMatrix(matrix);
                synchronized (this) {
                    matrix[1] *= scale.x;
                    matrix[4] *= scale.y;
                }
            }
        }
        return FkResult.Companion.getOK().getCode();
    }

    private boolean isRendering() {
        return !reqDestroy && !isDestroy;
    }

    @NonNull
    @Override
    public int[] getSize() {
//        FkLogcat.i(TAG, String.format("[%f, %f, %f, %f]", matrix[0], matrix[1], matrix[2], matrix[3]));
//        FkLogcat.i(TAG, String.format("[%f, %f, %f, %f]", matrix[4], matrix[5], matrix[6], matrix[7]));
//        FkLogcat.i(TAG, String.format("[%f, %f, %f, %f]", matrix[8], matrix[9], matrix[10], matrix[11]));
//        FkLogcat.i(TAG, String.format("[%f, %f, %f, %f]", matrix[12], matrix[13], matrix[14], matrix[15]));
        return new int[]{
                size.x, size.y
        };
    }

    @Override
    public long getTimestampInNS() {
        return timestampInNS;
    }

    public float[] getTransformMatrix() {
        return matrix;
    }

    @Override
    public long onNativeCreate() {
        return nativeCreateSurface(this);
    }

    public boolean onNativeDestroy(long handle) {
        nativeDestroy(handle);
        return true;
    }

    private native long nativeCreateSurface(FkAbsImageSource instance);

    private native void nativeDestroy(long handle);
}
