package com.alimin.fk.entity

open class FkCameraFeatureKey(val key: Int, val desc: String) {
    companion object {
        val SCENE_NIGHT_EXT = FkCameraFeatureKey(0x1, "Ext scene night")
        val SCENE_HDR_EXT = FkCameraFeatureKey(0x2, "Ext scene HDR")
        val SCENE_BOKEH_EXT = FkCameraFeatureKey(0x3, "Ext scene person bokeh")
        val SCENE_FACE_RETOUCH_EXT = FkCameraFeatureKey(0x4, "Ext scene face retouch")
        val SCENE_AUTO_EXT = FkCameraFeatureKey(0x5, "Ext scene auto")
        val SCENE_NIGHT_EXT_POST_VIEW = FkCameraFeatureKey(0x6, "Post view of ${SCENE_NIGHT_EXT.desc}")
        val SCENE_HDR_EXT_POST_VIEW = FkCameraFeatureKey(0x7, "Post view of ${SCENE_HDR_EXT.desc}")
        val SCENE_BOKEH_EXT_POST_VIEW = FkCameraFeatureKey(0x8, "Post view of ${SCENE_BOKEH_EXT.desc}")
        val SCENE_FACE_RETOUCH_EXT_POST_VIEW = FkCameraFeatureKey(0x9, "Post view of ${SCENE_FACE_RETOUCH_EXT.desc}")
        val SCENE_AUTO_EXT_POST_VIEW = FkCameraFeatureKey(0xA, "Post view of ${SCENE_AUTO_EXT.desc}")
        val SCENE_CROP_RAW = FkCameraFeatureKey(0xB, "Crop raw")
        val AE_MODE_AUTO = FkCameraFeatureKey(0x10, "Exp auto")
        val AE_MODE_OFF = FkCameraFeatureKey(0x11, "Exp off")
        val AE_MODE_ISO_FIRST = FkCameraFeatureKey(0x12, "Exp auto iso first")
        val AE_MODE_TIME_FIRST = FkCameraFeatureKey(0x13, "Exp auto time first")
        val FLASH_OFF = FkCameraFeatureKey(0x20, "Flash off")
        val FLASH_ON = FkCameraFeatureKey(0x21, "Flash on")
        val FLASH_AUTO = FkCameraFeatureKey(0x22, "Flash auto")
    }

    override fun equals(other: Any?): Boolean {
        return if (other is FkCameraFeatureKey) {
            return other.key == key
        } else {
            return false
        }
    }

    override fun hashCode(): Int {
        return key
    }

    class Param<T>(var k: FkCameraFeatureKey, val value: T) : FkCameraFeatureKey(k.key, k.desc)
    class VoidParam<Void>
    class IntParam<Int>
    class FloatParam<Float>
}