package com.alimin.fk.engine;

import androidx.annotation.Keep;
import androidx.annotation.Nullable;

import com.alimin.fk.entity.FkResult;
import com.alimin.fk.listener.FkGetLayersListener;
import com.alimin.fk.listener.FkNativeMsgListener;
import com.alimin.fk.pb.FkImageLayerOuterClass;
import com.alimin.fk.pb.FkPictureModelOuterClass;
import com.filmkilns.annotation.FkNativeAuto;
import com.google.protobuf.InvalidProtocolBufferException;

import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.util.ArrayList;

@Keep
@FkNativeAuto(path = "cpp/native/engine")
public class FkImageModel extends FkEngine {
    private FkImage engine;

    public FkImageModel(FkImage _engine) {
        this.engine = _engine;
    }

    @Override
    public long onNativeCreate() {
        long handle = engine.withLong(this::nativeCreateInstance);
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

    public FkResult save(String file) {
        return withInt(handle -> nativeSave(handle, file));
    }

    public FkResult load(String file, @NotNull OnDoStatusListener listener) {
        return withInt(handle -> nativeLoad(handle, file, new FkNativeMsgListener() {
            @Override
            public boolean onNativeMsgReceived(int what, int arg, @Nullable String msg, @Nullable ByteBuffer pbObject) {
                listener.onDone();
                return true;
            }
        }));
    }

    public int countOfLayer() {
        return 0;
    }

    public FkResult getLayers(FkGetLayersListener listener) {
        return withInt(handle -> nativeGetLayers(handle, new FkNativeMsgListener() {
            @Override
            public boolean onNativeMsgReceived(int what, int arg, @Nullable String msg, @Nullable ByteBuffer pbObject) {
                FkPictureModelOuterClass.FkPictureModel model = null;
                if (null != pbObject) {
                    try {
                        model = FkPictureModelOuterClass.FkPictureModel.parseFrom(pbObject);
                    } catch (InvalidProtocolBufferException e) {
                        e.printStackTrace();
                    }
                }
                if (model != null) {
                    listener.onGetLayers(model.getLayersList());
                } else {
                    listener.onGetLayers(new ArrayList<>());
                }
                return model != null;
            }
        }));
    }

    public FkImageLayerOuterClass.FkImageLayer getLayer(int layer) {
        return null;
    }

    private native long nativeCreateInstance(long imageEngineHandle);
    private native void nativeCreate(long handle);
    private native void nativeDestroy(long handle);
    private native void nativeStart(long handle);
    private native void nativeStop(long handle);

    private native int nativeSave(long handle, String file);
    private native int nativeLoad(long handle, String file, FkNativeMsgListener listener);
    private native int nativeGetLayers(long handle, FkNativeMsgListener listener);

    public interface OnDoStatusListener {
        void onDone();
    }
}
