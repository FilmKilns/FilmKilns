package com.alimin.fk.listener;

import androidx.annotation.Keep;
import androidx.annotation.Nullable;

import java.nio.ByteBuffer;

@Keep
public interface FkNativeMsgListener {
    boolean onNativeMsgReceived(int what, int arg, @Nullable String msg, ByteBuffer pbObject);
}
