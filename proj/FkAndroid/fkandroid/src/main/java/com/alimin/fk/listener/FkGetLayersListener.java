package com.alimin.fk.listener;

import androidx.annotation.Keep;

import com.alimin.fk.pb.FkImageLayerOuterClass;

import java.util.List;

@Keep
public interface FkGetLayersListener {
    void onGetLayers(List<FkImageLayerOuterClass.FkImageLayer> layers);
}
