package com.alimin.fk.entity

import android.util.Size

data class FkCameraSettings(
    val facing: FkCameraFeatures.kFacing,
    /**
     * ShortSide x LongSide. Example 1440x1920
     */
    val previewSize: Size,
    /**
     * ShortSide x LongSide. Example 1440x1920
     */
    val pictureSize: Size,
    val reqFeatures: MutableList<FkCameraFeatureKey> = ArrayList()
) {
    init {
        reqFeatures.add(FkCameraFeatureKey.AE_MODE_AUTO)
    }

    fun clone(newFacing: FkCameraFeatures.kFacing): FkCameraSettings {
        return FkCameraSettings(newFacing, previewSize, pictureSize, cloneReqFeatures())
    }

    private fun cloneReqFeatures(): MutableList<FkCameraFeatureKey> {
        val list = ArrayList<FkCameraFeatureKey>()
        reqFeatures.forEach {
            list.add(it)
        }
        return list
    }
}