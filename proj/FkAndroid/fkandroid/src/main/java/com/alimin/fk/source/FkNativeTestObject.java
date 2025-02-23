package com.alimin.fk.source;

import androidx.annotation.Keep;

import com.alimin.fk.core.FkNativeObject;
import com.alimin.fk.entity.FkResult;
import com.filmkilns.annotation.FkNativeAuto;

@Keep
@FkNativeAuto(path = "cpp/native/source")
public class FkNativeTestObject extends FkNativeObject {
    @Override
    public long onNativeCreate() {
        return nativeCreate(false, (byte) 1, '2', (short) 3, 4, 5, 6.0f, 7.0, "888", new Object(), new byte[2]);
    }

    @Override
    public boolean onNativeDestroy(long handle) {
        return nativeDestroy(handle) == FkResult.Companion.getOK().getCode();
    }

    public boolean getBool(boolean val) {
        return nativeGetBool(val);
    }

    public byte getByte(byte val) {
        return nativeGetByte(val);
    }

    public char getChar(char val) {
        return nativeGetChar(val);
    }

    public short getShort(short val) {
        return nativeGetShort(val);
    }

    public int getInt(int val) {
        return nativeGetInt(val);
    }

    public long getLong(long val) {
        return nativeGetLong(val);
    }

    public float getFloat(float val) {
        return nativeGetFloat(val);
    }

    public double getDouble(double val) {
        return nativeGetDouble(val);
    }

    public String getString(String val) {
        return nativeGetString(val);
    }

    public byte[] getByteArray(byte[] val) {
        return nativeGetByteArray(val);
    }

    private native long nativeCreate(boolean arg0, byte arg1, char arg2, short arg3, int arg4, long arg5, float arg6, double arg7, String arg8, Object arg9, byte[] arg10);

    private native int nativeDestroy(long handle);

    private native boolean nativeGetBool(boolean val);

    private native byte nativeGetByte(byte val);

    private native char nativeGetChar(char val);

    private native short nativeGetShort(short val);

    private native int nativeGetInt(int val);

    private native long nativeGetLong(long val);

    private native float nativeGetFloat(float val);

    private native double nativeGetDouble(double val);

    private native String nativeGetString(String val);

    private native byte[] nativeGetByteArray(byte[] val);
}
