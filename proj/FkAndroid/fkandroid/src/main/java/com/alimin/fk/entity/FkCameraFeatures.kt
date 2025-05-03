package com.alimin.fk.entity

import android.graphics.ImageFormat
import android.graphics.Point
import android.graphics.SurfaceTexture
import android.hardware.camera2.CameraCharacteristics
import android.hardware.camera2.CameraExtensionCharacteristics
import android.hardware.camera2.CameraMetadata
import android.hardware.camera2.params.StreamConfigurationMap
import android.os.Build
import android.util.Range
import android.util.Size
import android.view.Surface
import com.alimin.fk.utils.FkLogcat
import kotlin.math.abs

class FkCameraFeatures(val id: String, cc: CameraCharacteristics, ccExt: CameraExtensionCharacteristics?) {
    companion object {
        private val TAG = "FkCameraFeatures"
    }

    enum class kFacing {
        Front,
        Back,
        Unknown
    }

    val facing: kFacing
    val orientation: Int
    val focalDistance: Float
    val lensLength: Float
    val outputFormats: IntArray
    val exposureTimeRange: Range<Long>
    val supportedHardwareLevel: Int
    val evRange: Range<Int>?
    val isoRange: Range<Int>
    val fpsRanges: Array<Range<Int>>
    val maxFrameDuration: Long
    val maxAERegions: Int
    val maxAFRegions: Int
    private var map: StreamConfigurationMap? = null
    private val sizeMap: MutableMap<String, List<Size>> = HashMap()
    private var filled = false
    val availableKeys = ArrayList<FkCameraFeatureKey>()

    init {
        val face = cc.get(CameraCharacteristics.LENS_FACING)
        orientation = cc.get(CameraCharacteristics.SENSOR_ORIENTATION)!!
        map = cc.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
        map?.let {
            addSizes(SurfaceTexture::class.java, ::getOutputSizesFunc)
            addSizes(Surface::class.java, ::getOutputSizesFunc)
            addSizes(ImageFormat.JPEG, ::getOutputSizesFunc)
            if (it.isOutputSupportedFor(ImageFormat.YUV_420_888)) {
                addSizes(ImageFormat.YUV_420_888, ::getOutputSizesFunc)
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            ccExt?.let {
                val func: (format: Any) -> Array<Size> = { format: Any ->
                    if (format is Int) {
                        it.getExtensionSupportedSizes(
                            CameraExtensionCharacteristics.EXTENSION_AUTOMATIC,
                            format
                        ).toTypedArray()
                    } else if (format is Class<*>) {
                        it.getExtensionSupportedSizes(
                            CameraExtensionCharacteristics.EXTENSION_AUTOMATIC,
                            format
                        ).toTypedArray()
                    } else {
                        emptyArray()
                    }
                }
                addSizes(SurfaceTexture::class.java, func, "ext")
                addSizes(Surface::class.java, func, "ext")
                addSizes(ImageFormat.JPEG, func, "ext")
                addSizes(ImageFormat.YUV_420_888, func, "ext")
            }
        }
        outputFormats = map!!.outputFormats
        focalDistance = cc.get(CameraCharacteristics.LENS_INFO_HYPERFOCAL_DISTANCE) ?: 0.0f
        val lensLengths = cc.get(CameraCharacteristics.LENS_INFO_AVAILABLE_FOCAL_LENGTHS)
        lensLength = lensLengths?.get(0) ?: 0.0f
        fpsRanges = cc.get(CameraCharacteristics.CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES) ?: arrayOf(
            Range(30, 30)
        )
        facing = when (face) {
            CameraCharacteristics.LENS_FACING_FRONT -> {
                kFacing.Front
            }
            CameraCharacteristics.LENS_FACING_BACK -> {
                kFacing.Back
            }
            else -> {
                kFacing.Unknown
            }
        }
        this.exposureTimeRange = cc.get(CameraCharacteristics.SENSOR_INFO_EXPOSURE_TIME_RANGE) ?: Range(0L, 0L)
        this.supportedHardwareLevel = cc.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL) ?: CameraMetadata.INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED
        this.evRange = cc.get(CameraCharacteristics.CONTROL_AE_COMPENSATION_RANGE)
        this.isoRange = cc.get(CameraCharacteristics.SENSOR_INFO_SENSITIVITY_RANGE) ?: Range(0, 0)
        this.maxFrameDuration = cc.get(CameraCharacteristics.SENSOR_INFO_MAX_FRAME_DURATION) ?: 0L
        this.maxAERegions = cc.get(CameraCharacteristics.CONTROL_MAX_REGIONS_AE) ?: 0
        this.maxAFRegions = cc.get(CameraCharacteristics.CONTROL_MAX_REGIONS_AF) ?: 0
        _fillDefaultAvailableKeys(cc)
    }

    private fun _fillDefaultAvailableKeys(cc: CameraCharacteristics) {
        _addAvailableKey(FkCameraFeatureKey.AE_MODE_AUTO)
        _addAvailableKey(FkCameraFeatureKey.AE_MODE_OFF)
        _addAvailableKey(FkCameraFeatureKey.AE_MODE_ISO_FIRST)
        _addAvailableKey(FkCameraFeatureKey.AE_MODE_TIME_FIRST)
        _addAvailableKey(FkCameraFeatureKey.FLASH_OFF)
        if (cc.get(CameraCharacteristics.FLASH_INFO_AVAILABLE) == true) {
            _addAvailableKey(FkCameraFeatureKey.FLASH_ON)
            _addAvailableKey(FkCameraFeatureKey.FLASH_AUTO)
        }
    }

    fun fill(cc: CameraCharacteristics, ccExt: CameraExtensionCharacteristics?) {
        if (!filled) {
            filled = true
            _fillAvailableKeys(cc, ccExt)
        }
    }

    private fun _fillAvailableKeys(
        cc: CameraCharacteristics,
        ccExt: CameraExtensionCharacteristics?
    ) {
        val supportedExtensions = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            ccExt?.supportedExtensions
        } else {
            emptyList()
        }
        if (_isSupported(supportedExtensions, CameraExtensionCharacteristics.EXTENSION_NIGHT)) {
            _addAvailableKey(FkCameraFeatureKey.SCENE_NIGHT_EXT)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE
                && ccExt?.isPostviewAvailable(CameraExtensionCharacteristics.EXTENSION_NIGHT) == true) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_NIGHT_EXT_POST_VIEW)
            }
        }
        if (_isSupported(supportedExtensions, CameraExtensionCharacteristics.EXTENSION_HDR)) {
            _addAvailableKey(FkCameraFeatureKey.SCENE_HDR_EXT)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE
                && ccExt?.isPostviewAvailable(CameraExtensionCharacteristics.EXTENSION_HDR) == true) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_HDR_EXT_POST_VIEW)
            }
        }
        if (_isSupported(supportedExtensions, CameraExtensionCharacteristics.EXTENSION_BOKEH)) {
            _addAvailableKey(FkCameraFeatureKey.SCENE_BOKEH_EXT)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE
                && ccExt?.isPostviewAvailable(CameraExtensionCharacteristics.EXTENSION_BOKEH) == true) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_BOKEH_EXT_POST_VIEW)
            }
        }
        if (_isSupported(supportedExtensions, CameraExtensionCharacteristics.EXTENSION_FACE_RETOUCH)) {
            _addAvailableKey(FkCameraFeatureKey.SCENE_FACE_RETOUCH_EXT)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE
                && ccExt?.isPostviewAvailable(CameraExtensionCharacteristics.EXTENSION_FACE_RETOUCH) == true) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_FACE_RETOUCH_EXT_POST_VIEW)
            }
        }
        if (_isSupported(supportedExtensions, CameraExtensionCharacteristics.EXTENSION_AUTOMATIC)) {
            _addAvailableKey(FkCameraFeatureKey.SCENE_AUTO_EXT)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE
                && ccExt?.isPostviewAvailable(CameraExtensionCharacteristics.EXTENSION_AUTOMATIC) == true) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_AUTO_EXT_POST_VIEW)
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            try {
                ccExt?.getEstimatedCaptureLatencyRangeMillis(
                    CameraExtensionCharacteristics.EXTENSION_AUTOMATIC,
                    Size(3264, 2448),
                    ImageFormat.JPEG
                )?.apply {
                    FkLogcat.i(TAG, "Capture latency ${lower}-${upper}")
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            val cases = cc.get(CameraCharacteristics.SCALER_AVAILABLE_STREAM_USE_CASES)
            val key = CameraMetadata.SCALER_AVAILABLE_STREAM_USE_CASES_CROPPED_RAW.toLong()
            if (_isSupported(cases, key)) {
                _addAvailableKey(FkCameraFeatureKey.SCENE_CROP_RAW)
            }
        }
    }

    private fun _addAvailableKey(key: FkCameraFeatureKey) {
        availableKeys.add(key)
    }

    private fun _isSupported(cases: LongArray?, req: Long): Boolean {
        var isSupported = false
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            cases?.forEach {
                if (req == it) {
                    isSupported = true
                }
            }
        }
        return isSupported
    }

    private fun _isSupported(supportedExtensions: List<Int>?, req: Int): Boolean {
        var isSupported = false
        supportedExtensions?.forEach {
            if (req == it) {
                isSupported = true
            }
        }
        return isSupported
    }

    private fun getSizeKey(format: Any, flag: String): String {
        if (format is Int && map!!.isOutputSupportedFor(format)) {
            return "${format}_$flag"
        } else if (format is Class<*>) {
            return "${format.simpleName}_$flag"
        }
        return "${format.javaClass.simpleName}_$flag"
    }

    private fun getOutputSizesFunc(format: Any): Array<Size> {
        return if (format is Int && map!!.isOutputSupportedFor(format)) {
            map!!.getOutputSizes(format)
        } else if (format is Class<*>) {
            map!!.getOutputSizes(format)
        } else {
            return emptyArray()
        }
    }

    private fun addSizes(format: Any, func: (format: Any) -> Array<Size>, flag: String = "") {
        val key = getSizeKey(format, flag)
        if (format is Int && map!!.isOutputSupportedFor(format)) {
            val sizes: Array<Size> = func(format)
            sizeMap[key] = sizes.asList()
            FkLogcat.i(TAG,  "format=${format}: ${sizes.contentToString()}")
        } else {
            if (StreamConfigurationMap.isOutputSupportedFor(format as Class<*>)) {
                val sizes = func(format)
                if (sizes != null) {
                    sizeMap[key] = sizes.asList()
                    FkLogcat.i(TAG, "format=$key: ${sizes.contentToString()}")
                } else {
                    FkLogcat.w(TAG,  "format=$key sizes is empty.")
                }
            }
        }
    }

    fun getSizesFor(format: Any, flag: String): List<Size>? {
        val key = getSizeKey(format, flag)
        return if (sizeMap.containsKey(key)) {
            sizeMap[key]
        } else null
    }

    fun isOutputSupportedFor(format: Int): Boolean {
        return map!!.isOutputSupportedFor(format)
    }

    fun getBestSize(width: Int, height: Int, format: Any, flag: String): Size {
        val key = getSizeKey(format, flag)
        var delta = Int.MAX_VALUE
        val size = Point(width, height)
        val list = sizeMap[key]
        list?.forEach {
            if (it.width == width && it.height == height) {
                return it
            }
            val d = abs(it.width * it.height - width * height)
            if (it.width * height == width * it.height && d < delta) {
                delta = d
                size.x = it.width
                size.y = it.height
            }
        }
        return Size(size.x, size.y)
    }

    fun getMaxDiffFpsRange(): Range<Int> {
        val firstRanges = fpsRanges.filter { it.upper >= 30 }
        val targetRanges = if (firstRanges.isNotEmpty()) {
            firstRanges
        } else {
            fpsRanges.filter { true }
        }
        var latestDelta = 0
        var latestRange = fpsRanges.first()
        targetRanges.forEach {
            val d = it.upper - it.lower
            if (d > latestDelta) {
                latestDelta = d
                latestRange = it
            }
        }
        return latestRange
    }

    fun getAvailableKeysContent(): String {
        val sb = StringBuilder("[")
        availableKeys.forEach {
            sb.append("${it.desc}, ")
        }
        sb.delete(sb.length - 2, sb.length)
        sb.append("]")
        return sb.toString()
    }

    fun contain(key: FkCameraFeatureKey): Boolean {
        return availableKeys.find { it.key == key.key } != null
    }

    override fun toString(): String {
        return ("FkCamMetadata(id=" + id + ", "
                + "facing=" + facing + ", "
                + "evRange=" + evRange.toString() + ", "
                + "isoRange=" + isoRange.toString() + ", "
                + "exposureTimeRange=" + exposureTimeRange.toString() + ", "
                + "maxFrameDuration=" + maxFrameDuration + ", "
                + "supportedHardwareLevel=" + supportedHardwareLevel + ", "
                + "fpsRanges=" + fpsRanges.contentToString() + ", "
                + "orientation=" + orientation + ", "
                + "availableKeys=" + getAvailableKeysContent() + ")")
    }
}