package com.alimin.fk.source;

import android.util.Size;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.alimin.fk.core.FkAbsImageSource;
import com.alimin.fk.utils.FkLogcat;
import com.filmkilns.annotation.FkNativeAuto;

import java.nio.ByteBuffer;

@Keep
@FkNativeAuto(path = "cpp/native/source")
public class FkYUV420SPImageSource extends FkAbsImageSource {
    private final static String TAG = "FkYUV420SPImageSource";
    private Size size;
    private ByteBuffer y;
    private ByteBuffer u;
    private ByteBuffer v;
    private int orientation;
    private long timestamp;

    public FkYUV420SPImageSource(ByteBuffer y, ByteBuffer uv, Size size, int orientation, long timestamp) {
        super();
        FkLogcat.i(TAG, "Constructor");
        this.size = size;
        this.y = y;
        this.u = uv;
        this.v = null;
        this.orientation = orientation;
        this.timestamp = timestamp;
    }

    public FkYUV420SPImageSource(ByteBuffer y, ByteBuffer u, ByteBuffer v, Size size, int orientation, long timestamp) {
        super();
        FkLogcat.i(TAG, "Constructor");
        this.size = size;
        this.y = y;
        this.u = u;
        this.v = v;
        this.orientation = orientation;
        this.timestamp = timestamp;
    }

    @Override
    public int onCreate(int arg0, long arg1, @Nullable String arg2) {
        FkLogcat.i(TAG, "onCreate, tex=" + arg0);
        return 0;
    }

    @Override
    public int onDestroy() {
        FkLogcat.i(TAG, "onDestroy");
        return 0;
    }

    @Override
    public int onRender(int arg0, long arg1, @Nullable String arg2) {
        return 0;
    }

    @NonNull
    @Override
    public int[] getSize() {
        return new int[]{size.getWidth(), size.getHeight()};
    }

    @Override
    public long getTimestampInNS() {
        return timestamp;
    }

    @Override
    public long onNativeCreate() {
        return nativeCreate(this, y, u, v, size.getWidth(), size.getHeight(), orientation);
    }

    public boolean onNativeDestroy(long handle) {
        nativeDestroy(handle);
        return true;
    }

    private native long nativeCreate(FkAbsImageSource instance, ByteBuffer y, ByteBuffer u, ByteBuffer v, int width, int height, int orientation);

    private native void nativeDestroy(long handle);
}
