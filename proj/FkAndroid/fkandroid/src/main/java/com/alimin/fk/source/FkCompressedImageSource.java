package com.alimin.fk.source;

import android.util.Size;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.alimin.fk.core.FkAbsImageSource2;
import com.alimin.fk.utils.FkLogcat;
import com.filmkilns.annotation.FkNativeAuto;

import java.nio.ByteBuffer;

@Keep
@FkNativeAuto(path = "cpp/native/source")
public class FkCompressedImageSource extends FkAbsImageSource2 {
    private final static String TAG = "FkCompressedImageSource";
    private ByteBuffer buf;
    private int orientation;

    public FkCompressedImageSource(ByteBuffer buf, int orientation) {
        super();
        FkLogcat.i(TAG, "Constructor");
        this.buf = buf;
        this.orientation = orientation;
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
        return new int[]{0, 0};
    }

    @Override
    public long getTimestampInNS() {
        return System.currentTimeMillis() * 1000000;
    }

    @Override
    public long onNativeCreate() {
        return nativeCreate(this, buf, orientation);
    }

    public boolean onNativeDestroy(long handle) {
        nativeDestroy(handle);
        return true;
    }

    private native long nativeCreate(FkAbsImageSource2 instance, ByteBuffer buf, int orientation);

    private native void nativeDestroy(long handle);
}
