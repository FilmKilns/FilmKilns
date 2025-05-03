package com.alimin.fk.utils

import android.graphics.ImageFormat
import android.hardware.camera2.CaptureRequest
import android.hardware.camera2.CaptureResult
import android.hardware.camera2.TotalCaptureResult
import android.hardware.camera2.params.MeteringRectangle
import android.os.Build
import android.util.Range
import android.util.Size
import com.alimin.fk.entity.FkCameraFeatureKey
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import com.alimin.fk.entity.FkCaptureMetadata
import kotlin.math.ln
import kotlin.math.pow

class FkCaptureReqUtils {
    companion object{
         fun withBasicParams(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            builder.apply {
                set(CaptureRequest.CONTROL_EFFECT_MODE, CaptureRequest.CONTROL_EFFECT_MODE_OFF)
                set(CaptureRequest.JPEG_QUALITY, 100.toByte())
            }
            return builder
        }

         fun withAutoRequest(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO)
                set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON)
                set(CaptureRequest.CONTROL_AWB_MODE, CaptureRequest.CONTROL_AWB_MODE_AUTO)
            }
        }

        fun withManualRequest(
            builder: CaptureRequest.Builder,
            metadata: FkCaptureMetadata?
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO)
                set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_OFF)
                set(CaptureRequest.CONTROL_AWB_MODE, CaptureRequest.CONTROL_AWB_MODE_AUTO)
                metadata?.let {
                    withExposure(this, it)
                }
            }
        }

        fun withManualRequest(
            builder: CaptureRequest.Builder,
            iso: Int,
            exposureTime: Long
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO)
                set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_OFF)
                set(CaptureRequest.CONTROL_AWB_MODE, CaptureRequest.CONTROL_AWB_MODE_AUTO)
                withExposure(builder, iso, 0, exposureTime, 1.0f, 0, 0.0f)
            }
        }

        fun isManualExposure(builder: CaptureRequest.Builder): Boolean {
            return builder.get(CaptureRequest.CONTROL_AE_MODE) == CaptureRequest.CONTROL_AE_MODE_OFF
        }

        fun withIntentManualRequest(
            builder: CaptureRequest.Builder,
            metadata: FkCaptureMetadata?
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO)
                set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_OFF)
                set(CaptureRequest.CONTROL_AWB_MODE, CaptureRequest.CONTROL_AWB_MODE_AUTO)
                set(CaptureRequest.CONTROL_CAPTURE_INTENT, CaptureRequest.CONTROL_CAPTURE_INTENT_MANUAL)
                withExposure(this, metadata)
            }
        }

        fun withExposure(
            builder: CaptureRequest.Builder,
            metadata: FkCaptureMetadata?
        ): CaptureRequest.Builder {
            return builder.apply {
                metadata?.apply {
                    withExposure(builder, ISO, boostISO, exposureTime, exposureFactor, exposureCompensation, aperture)
                }
            }
        }

        fun withExposure(
            builder: CaptureRequest.Builder,
            iso: Int,
            boostISO: Int,
            exposureTime: Long,
            exposureFactor: Float,
            exposureCompensation: Int,
            aperture: Float
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.SENSOR_SENSITIVITY, iso)
                set(CaptureRequest.SENSOR_EXPOSURE_TIME, exposureTime)
                set(CaptureRequest.LENS_APERTURE, aperture)
                set(CaptureRequest.CONTROL_AE_EXPOSURE_COMPENSATION, exposureCompensation)
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                    set(CaptureRequest.CONTROL_POST_RAW_SENSITIVITY_BOOST, boostISO)
                }
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                    set(CaptureRequest.REPROCESS_EFFECTIVE_EXPOSURE_FACTOR, exposureFactor)
                }
            }
        }

        fun withFaceAE(
            builder: CaptureRequest.Builder,
            metadata: FkCaptureMetadata?,
            previewSize: Size
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.STATISTICS_FACE_DETECT_MODE, CaptureRequest.STATISTICS_FACE_DETECT_MODE_FULL)
                set(CaptureRequest.CONTROL_SCENE_MODE, CaptureRequest.CONTROL_SCENE_MODE_PORTRAIT)
                metadata?.apply {
                    if (faces.isNotEmpty()) {
//                        val rect = faces[0].bounds
//                        val region = MeteringRectangle(
//                            Rect(
//                                rect.left * 2000 / previewSize.width - 1000,
//                                rect.top * 2000 / previewSize.height - 1000,
//                                rect.right * 2000 / previewSize.width - 1000,
//                                rect.bottom * 2000 / previewSize.height - 1000
//                            ),
//                            MeteringRectangle.METERING_WEIGHT_MAX
//                        )
                        val region = MeteringRectangle(faces[0].bounds, MeteringRectangle.METERING_WEIGHT_MAX)
                        set(CaptureRequest.CONTROL_AE_REGIONS, arrayOf(region))
                        set(CaptureRequest.CONTROL_AF_REGIONS, arrayOf(region))
                        set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START)
                        set(CaptureRequest.CONTROL_AF_TRIGGER, CaptureRequest.CONTROL_AF_TRIGGER_START)
                    }
                }
            }
        }

        fun withHDR(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_SCENE_MODE, CaptureRequest.CONTROL_SCENE_MODE_HDR)
            }
        }

        fun withFpsRange(
            builder: CaptureRequest.Builder,
            fpsRange: Range<Int>
        ): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_AE_TARGET_FPS_RANGE, fpsRange)
            }
        }

        fun withFlashMode(
            builder: CaptureRequest.Builder,
            featureKey: FkCameraFeatureKey
        ): CaptureRequest.Builder {
            return builder.apply {
                if (featureKey == FkCameraFeatureKey.FLASH_OFF) {
                    set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON)
                    set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_OFF)
                } else if (featureKey == FkCameraFeatureKey.FLASH_AUTO) {
                    set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START)
                    set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH)
                    set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_SINGLE)
                } else if (featureKey == FkCameraFeatureKey.FLASH_ON) {
                    set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER_START)
                    set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_ALWAYS_FLASH)
                    set(CaptureRequest.FLASH_MODE, CaptureRequest.FLASH_MODE_SINGLE)
                }
            }
        }

        fun withHighQuality(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.EDGE_MODE, CaptureRequest.EDGE_MODE_HIGH_QUALITY)
                set(CaptureRequest.SHADING_MODE, CaptureRequest.SHADING_MODE_HIGH_QUALITY)
                set(CaptureRequest.TONEMAP_MODE, CaptureRequest.TONEMAP_MODE_HIGH_QUALITY)
                set(CaptureRequest.COLOR_CORRECTION_MODE, CaptureRequest.COLOR_CORRECTION_MODE_HIGH_QUALITY)
                set(CaptureRequest.COLOR_CORRECTION_ABERRATION_MODE, CaptureRequest.COLOR_CORRECTION_ABERRATION_MODE_HIGH_QUALITY)
                set(CaptureRequest.HOT_PIXEL_MODE, CaptureRequest.HOT_PIXEL_MODE_HIGH_QUALITY)
                set(CaptureRequest.NOISE_REDUCTION_MODE, CaptureRequest.NOISE_REDUCTION_MODE_HIGH_QUALITY)
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    set(CaptureRequest.DISTORTION_CORRECTION_MODE, CaptureRequest.DISTORTION_CORRECTION_MODE_HIGH_QUALITY)
                }
            }
        }

        fun withScenePortraitRequest(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_USE_SCENE_MODE)
                set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON)
                set(CaptureRequest.CONTROL_SCENE_MODE, CaptureRequest.CONTROL_SCENE_MODE_PORTRAIT)
            }
        }

        fun withDenoiseRequest(builder: CaptureRequest.Builder): CaptureRequest.Builder {
            return builder.apply {
                set(CaptureRequest.NOISE_REDUCTION_MODE, CaptureRequest.NOISE_REDUCTION_MODE_HIGH_QUALITY)
            }
        }

        fun getExposureBrightness(result: TotalCaptureResult): Double {
            val iso = result.get(CaptureResult.SENSOR_SENSITIVITY) ?: 0
            val expTime = result.get(CaptureResult.SENSOR_EXPOSURE_TIME) ?: 0
            val aperture = result.get(CaptureResult.LENS_APERTURE) ?: 0.0f
            return calcExposureBrightness(iso, expTime, aperture)
        }

        fun calcExposureBrightness(iso: Int, expTime: Long, aperture: Float): Double {
            return ln((100 * aperture.toDouble().pow(2.0)) / (iso * expTime / 1000000000)) / ln(2.0)
        }

        fun calcISO(ev: Double, expTime: Long, aperture: Float): Int {
//            ln(iso * expTime) = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0)
//            ln(iso) + ln(expTime) = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0)
//            ln(iso)  = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0) - ln(expTime)
            val iso = Math.E.pow(ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0) - ln(expTime.toDouble() / 1000000000))
            return iso.toInt()
        }

        fun calcExposureTime(ev: Double, iso: Int, aperture: Float): Long {
//            ln(iso * expTime) = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0)
//            ln(iso) + ln(expTime) = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0)
//            ln(expTime) = ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0) - ln(iso)
            val expTime = Math.E.pow(ln(100 * aperture.toDouble().pow(2.0)) - ev * ln(2.0) - ln(iso.toDouble())) * 1000000000
            return expTime.toLong()
        }

        fun containsFeatureKey(features: FkCameraFeatures?, settings: FkCameraSettings?, key: FkCameraFeatureKey): Boolean {
            return features?.contain(key) == true && settings?.reqFeatures?.contains(key) == true
        }

        fun withRequests(
            builder: CaptureRequest.Builder,
            features: FkCameraFeatures?,
            settings: FkCameraSettings?
        ) {
            if (containsFeatureKey(features, settings, FkCameraFeatureKey.FLASH_OFF)) {
                withFlashMode(builder, FkCameraFeatureKey.FLASH_OFF)
            } else if (containsFeatureKey(features, settings, FkCameraFeatureKey.FLASH_AUTO)) {
                withFlashMode(builder, FkCameraFeatureKey.FLASH_AUTO)
            } else if (containsFeatureKey(features, settings, FkCameraFeatureKey.FLASH_ON)) {
                withFlashMode(builder, FkCameraFeatureKey.FLASH_ON)
            }
        }

        fun getFormatStr(format: Int): String {
            when (format) {
                ImageFormat.RGB_565 -> return "RGB_565"
                ImageFormat.NV16 -> return "NV16"
                ImageFormat.YUY2 -> return "YUY2"
                ImageFormat.YV12 -> return "YV12"
                ImageFormat.Y8 -> return "Y8"
                /*ImageFormat.Y16, */ImageFormat.DEPTH16 -> return "DEPTH16"
                ImageFormat.NV21 -> return "NV21"
                ImageFormat.YUV_420_888 -> return "YUV_420_888"
                ImageFormat.YUV_422_888 -> return "YUV_422_888"
                ImageFormat.YUV_444_888 -> return "YUV_444_888"
                ImageFormat.FLEX_RGB_888 -> return "FLEX_RGB_888"
                ImageFormat.FLEX_RGBA_8888 -> return "FLEX_RGBA_8888"
                /*ImageFormat.RAW_DEPTH, */ImageFormat.RAW_SENSOR -> return "RAW_SENSOR"
                ImageFormat.YCBCR_P010 -> return "YCBCR_P010"
                /*ImageFormat.RAW_DEPTH10, */ImageFormat.RAW10 -> return "RAW10"
                ImageFormat.RAW12 -> return "RAW12"
                ImageFormat.JPEG -> return "JPEG"
                ImageFormat.JPEG_R -> return "JPEG_R"
                ImageFormat.DEPTH_JPEG -> return "DEPTH_JPEG"
            }
            return "Unknown"
        }
    }
}