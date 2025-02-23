package com.alimin.fk.device

import android.annotation.SuppressLint
import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.camera2.CameraAccessException
import android.hardware.camera2.CameraDevice
import android.hardware.camera2.CameraManager
import android.hardware.camera2.CaptureRequest
import android.hardware.camera2.TotalCaptureResult
import android.media.ExifInterface
import android.media.Image
import android.media.ImageReader
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.util.Size
import android.view.Surface
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.core.FkAbsImageSource2
import com.alimin.fk.entity.FkCameraFeatureKey
import com.alimin.fk.entity.FkCameraFeatures
import com.alimin.fk.entity.FkCameraSettings
import com.alimin.fk.entity.FkCaptureMetadata
import com.alimin.fk.entity.FkResult
import com.alimin.fk.source.FkCompressedImageSource
import com.alimin.fk.source.FkSurfaceTextureSource
import com.alimin.fk.source.FkYUV420SPImageSource
import com.alimin.fk.utils.FkCaptureReqUtils
import com.alimin.fk.utils.FkLogcat
import java.util.LinkedList


class FkCamera2(private val manager: CameraManager) : FkAbsCamera() {
    private val featuresList = ArrayList<FkCameraFeatures>()
    private var filledFeatures = false
    private val cameraDeviceLock = Object()
    private var cameraDevice: CameraDevice? = null
    private lateinit var thread: HandlerThread
    private lateinit var handler: Handler
    private var curFeatures: FkCameraFeatures? = null
    private val surfaceSource = FkSurfaceTextureSource()
    private var captureSession: FkAbsCameraSession? = null
    private var mPreviewReqBuilder: CaptureRequest.Builder? = null
    private var cameraSettings = FkCameraSettings(FkCameraFeatures.kFacing.Front, Size(1080, 1920), Size(1080, 1920))
    private var previewSize = Size(0, 0)
    private var pictureSize = Size(0, 0)
    private var imageReader: ImageReader? = null
    private var state = FkCameraState.IDLE
    private val captureListenerQueue = LinkedList<OnCaptureListener>()
    private var latestPrevMetadata: FkCaptureMetadata? = null
    private var curExpMetadata: FkCaptureMetadata? = null

    companion object {
        private val TAG = "FkCamera2"
    }

    @Synchronized
    override fun create(): Int {
        FkLogcat.i(TAG, "create")
        thread = HandlerThread(TAG)
        thread.start()
        handler = Handler(thread.looper)
        try {
            for (it in manager.cameraIdList) {
                val ccExt = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    manager.getCameraExtensionCharacteristics(it!!)
                } else {
                    null
                }
                val features = FkCameraFeatures(it, manager.getCameraCharacteristics(it!!), ccExt)
                if (features.facing !== FkCameraFeatures.kFacing.Unknown) {
                    featuresList.add(features)
                    FkLogcat.i(TAG, "Query camera features: $features")
                }
            }
            dispatchInfo(FkResult.INFO_CAMERA_FILL_SOME_FEATURES_FINISH, 0, getFeatures())
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
        surfaceSource.create()
        return 0
    }

    @Synchronized
    override fun destroy(): Int {
        FkLogcat.i(TAG, "destroy")
        filledFeatures = false
        featuresList.clear()
        surfaceSource.destroy()
        captureListenerQueue.clear()
        thread.quitSafely()
        return 0
    }

    @SuppressLint("MissingPermission")
    @Synchronized
    override fun start(settings: FkCameraSettings): Int {
        FkLogcat.i(TAG, "start")
        this.cameraSettings = settings
        featuresList.filter { it.facing == settings.facing }.let {
            curFeatures = it[0]
            previewSize = curFeatures!!.getBestSize(
                settings.previewSize.height,
                settings.previewSize.width,
                SurfaceTexture::class.java
            )
            if (curFeatures!!.facing == FkCameraFeatures.kFacing.Back) {
                surfaceSource.setScale(-1f, 1f)
            } else if (curFeatures!!.facing == FkCameraFeatures.kFacing.Front) {
                surfaceSource.setScale(1f, -1f)
            }
            FkLogcat.i(TAG, "Select camera ${curFeatures!!.id}")
        }
        if (curFeatures == null) {
            FkLogcat.e(TAG, "Camera feature not found!")
            return -1
        }
        this.curExpMetadata = null
        FkLogcat.i(TAG, "Preview size ${settings.previewSize.width}x${settings.previewSize.height} -> ${previewSize.width}x${previewSize.height}")
        surfaceSource.setSize(previewSize.width, previewSize.height)
        try {
            manager.openCamera(curFeatures!!.id, deviceStateCallback, handler)
        } catch (e: CameraAccessException) {
            e.printStackTrace();
            return -1
        }
        return 0
    }

    @Synchronized
    override fun stop(): Int {
        FkLogcat.i(TAG, "stop")
        state = FkCameraState.STOPPING
        closeCaptureSession()
        cameraDevice?.close()
        return 0
    }

    override fun getImageSource(): FkAbsImageSource2 = surfaceSource

    override fun takePicture(listener: OnCaptureListener): Int {
        if (cameraDevice == null) {
            FkLogcat.e(TAG, "Camera device is null")
            return -1
        }
        val callback = object : FkAbsCameraSession.CaptureCallback {
            override fun onCaptureCompleted(
                session: FkAbsCameraSession,
                request: CaptureRequest,
                result: TotalCaptureResult
            ) {
                val metadata = FkCaptureMetadata(result)
                FkLogcat.i(TAG, "onCaptureCompleted capture: ${metadata}")
            }
        }
        try {
            captureSession?.stopRepeating()
            val fpsRange = curFeatures!!.getMaxDiffFpsRange()
            val template = CameraDevice.TEMPLATE_STILL_CAPTURE
            val request = cameraDevice!!.createCaptureRequest(template).apply {
                addTarget(imageReader!!.surface)
                val isFacingBack = curFeatures?.facing == FkCameraFeatures.kFacing.Back
                set(CaptureRequest.JPEG_ORIENTATION, if (isFacingBack) 90 else 270)
                FkCaptureReqUtils.withBasicParams(this)
                val metadata = latestPrevMetadata?.getSuggestMetadataByISOFirst(curFeatures!!.isoRange)
                FkLogcat.d(TAG, "Preview metadata: ${latestPrevMetadata}")
                FkLogcat.d(TAG, "Capture metadata: ${metadata}, fpsRange=$fpsRange")
//                if (metadata?.ISO!! <= 400) {
                if (metadata?.ISO!! <= -1) {
                    FkCaptureReqUtils.withManualRequest(this, metadata)
                    FkLogcat.d(TAG, "Capture with manual mode")
                } else {
                    FkCaptureReqUtils.withAutoRequest(this)
                    FkLogcat.d(TAG, "Capture with auto mode")
                }
                FkCaptureReqUtils.withFaceAE(this, metadata, previewSize)
                FkCaptureReqUtils.withHighQuality(this)
                FkCaptureReqUtils.withFpsRange(this, fpsRange)
            }
            captureSession?.capture(request.build(), callback, handler)
        } catch (e: Exception) {
            FkLogcat.e(TAG, "Create still capture request with exception: $e");
            return -1
        }
        captureListenerQueue.offer(listener)
        return 0
    }

    override fun getFeatures(): List<FkCameraFeatures> = featuresList

    override fun updateExposure(curExpValue: Int): Int {
        mPreviewReqBuilder?.apply {
            val scale = 1 - Math.sin(curExpValue * Math.PI / 2 / 100)
            if (FkCaptureReqUtils.isManualExposure(this) && curExpMetadata != null) {
                val metadata = if (FkCaptureReqUtils.containsFeatureKey(curFeatures, cameraSettings, FkCameraFeatureKey.AE_MODE_ISO_FIRST)) {
                    curExpMetadata!!.getNextMetadataByISOFirst(curFeatures!!.isoRange, curFeatures!!.exposureTimeRange, curExpValue, scale, 80)
                } else {
                    curExpMetadata!!
                }
                FkLogcat.i(TAG, "exposure_value exp=$curExpValue, scale=$scale, ${metadata.ISO}/${metadata.exposureTime}")
                if ((metadata.ISO != curExpMetadata!!.ISO || metadata.exposureTime != curExpMetadata!!.exposureTime)
                    && latestPrevMetadata!!.ISO == curExpMetadata!!.ISO
                    && latestPrevMetadata!!.exposureTime == curExpMetadata!!.exposureTime
                ) {
                    curExpMetadata = metadata
                    FkCaptureReqUtils.withManualRequest(this, metadata.ISO, metadata.exposureTime)
                    updatePreview()
                }
            }
            return 0
        }
        return -1
    }

    private fun closeCaptureSession() {
        FkLogcat.i(TAG, "closeCaptureSession")
        captureSession?.let {
            try {
//                it.abortCaptures()
                it.stopRepeating()
            } catch (e: CameraAccessException) {
                e.printStackTrace()
            }
            it.close()
        }
        destroyImageReader()
        captureSession = null
    }

    private fun openCaptureSession() {
        FkLogcat.i(TAG, "openCaptureSession")
        surfaceSource.allocBuffer()
        val surfaces: MutableList<Surface> = ArrayList()
        surfaceSource.getSurface()?.let {
            surfaces.add(it)
        }
        createImageReader().let {
            surfaces.add(it)
        }
        try {
            val callback = object : FkAbsCameraSession.StateCallback {
                override fun onConfigured(session: FkAbsCameraSession) {
                    startPreview()
                }

                override fun onConfigureFailed(session: FkAbsCameraSession) {
                    closeCaptureSession()
                }

            }
            captureSession = if (cameraSettings.reqFeatures.find { it.equals(FkCameraFeatureKey.SCENE_AUTO_EXT) } != null) {
                FkLogcat.i(TAG, "Create ext session")
                FkCameraExtSession(cameraDevice, surfaces, handler, callback)
            } else {
                FkLogcat.i(TAG, "Create session")
                FkCameraSession(cameraDevice, surfaces, handler, callback)
            }
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    private fun startPreview() {
        FkLogcat.i(TAG, "startPreview")
        try {
            mPreviewReqBuilder = cameraDevice?.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW)
            mPreviewReqBuilder?.apply {
                surfaceSource.getSurface()?.let { surface ->
                    addTarget(surface)
                }

                val fpsRange = curFeatures!!.getMaxDiffFpsRange()
                FkCaptureReqUtils.withFpsRange(this, fpsRange)
                if (FkCaptureReqUtils.containsFeatureKey(curFeatures, cameraSettings, FkCameraFeatureKey.AE_MODE_AUTO)) {
                    FkCaptureReqUtils.withAutoRequest(this)
                } else {
                    FkCaptureReqUtils.withManualRequest(this, curFeatures!!.isoRange.lower, 10000000)
                }
                set(
                    CaptureRequest.CONTROL_AF_MODE,
                    CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO
                )
            }
            updatePreview()
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    private fun updatePreview() {
        FkLogcat.i(TAG, "updatePreview")
        try {
            mPreviewReqBuilder?.apply {
                captureSession?.stopRepeating()
                captureSession?.setRepeatingRequest(mPreviewReqBuilder!!.build(), previewCaptureCallback, handler)
            }
        } catch (e: CameraAccessException) {
            e.printStackTrace()
        }
    }

    private fun createImageSource(reader: ImageReader, image: Image): FkAbsImageSource? {
        val orientation = if (curFeatures!!.facing == FkCameraFeatures.kFacing.Back) ExifInterface.ORIENTATION_ROTATE_90 else ExifInterface.ORIENTATION_TRANSVERSE
        val source = if (reader.imageFormat == ImageFormat.JPEG) {
            val buf = image.planes[0].buffer
            FkCompressedImageSource(buf, orientation)
        } else if (reader.imageFormat == ImageFormat.YUV_420_888) {
            val planes = image.planes
            val result =
                if (planes.size == 2 || (planes.size == 3 && planes[1].rowStride == image.width && planes[1].pixelStride == 2)) {
                    FkYUV420SPImageSource(
                        planes[0].buffer, planes[1].buffer,
                        Size(image.width, image.height), orientation
                    )
                } else if (planes.size == 3) {
                    FkYUV420SPImageSource(
                        planes[0].buffer,
                        planes[1].buffer,
                        planes[2].buffer, Size(image.width, image.height), orientation
                    )
                } else {
                    null
                }
            result
        } else {
            null
        }
        return source
    }

    private fun createImageReader(): Surface {
        val jpegSize = curFeatures!!.getBestSize(
            cameraSettings.pictureSize.height,
            cameraSettings.pictureSize.width,
            ImageFormat.JPEG
        )
        val yuvSize = if (curFeatures!!.isOutputSupportedFor(ImageFormat.YUV_420_888)) {
            curFeatures!!.getBestSize(
                cameraSettings.pictureSize.height,
                cameraSettings.pictureSize.width,
                ImageFormat.YUV_420_888
            )
        } else {
            Size(1, 1)
        }
        pictureSize = yuvSize
        var format = ImageFormat.YUV_420_888
        if (jpegSize.width * jpegSize.height > yuvSize.width * yuvSize.height) {
            pictureSize = jpegSize
            format = ImageFormat.JPEG
        }
        FkLogcat.i(TAG, "Picture size ${cameraSettings.pictureSize.width}x${cameraSettings.pictureSize.height} -> ${pictureSize.width}x${pictureSize.height}, format=$format")
        val reader = ImageReader.newInstance(
            pictureSize.width,
            pictureSize.height,
            format,
            3
        ).apply {
            setOnImageAvailableListener({ reader ->
                val image = reader.acquireNextImage()
                if (image != null) {
                    val format = image.format
                    val timestamp = image.timestamp
                    FkLogcat.i(TAG, "Get picture ${image.width}x${image.height}, format=${format}, timestamp=${timestamp}")
                    captureListenerQueue.poll()?.onResult(createImageSource(reader, image))
                    image.close()
                }
            }, handler)
        }
        this.imageReader = reader
        return reader.surface
    }

    private fun destroyImageReader() {
        imageReader?.close()
    }

    private fun _fillFeatures() {
        if (filledFeatures) {
            return
        }
        featuresList.forEach {
            handler.post {
                val ccExt = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    manager.getCameraExtensionCharacteristics(it.id)
                } else {
                    null
                }
                it.fill(manager.getCameraCharacteristics(it.id), ccExt)
            }
        }
        filledFeatures = true
        val msg = "Fill features finish."
        FkLogcat.i(TAG, "[OK] $msg")
        dispatchInfo(FkResult.INFO_CAMERA_FILL_ALL_FEATURES_FINISH, 0, getFeatures())
    }

    private val previewCaptureCallback = object : FkAbsCameraSession.CaptureCallback {
        override fun onCaptureCompleted(
            session: FkAbsCameraSession,
            request: CaptureRequest,
            result: TotalCaptureResult
        ) {
            latestPrevMetadata = FkCaptureMetadata(result)
            if (curExpMetadata == null) {
                curExpMetadata = FkCaptureMetadata(result)
            }
            FkLogcat.d(TAG, "onCaptureCompleted preview: ${latestPrevMetadata!!.ISO}, ${latestPrevMetadata!!.exposureTime}")
            _fillFeatures()
        }
    }

    private val deviceStateCallback = object : CameraDevice.StateCallback() {

        override fun onOpened(camera: CameraDevice) {
            FkLogcat.i(TAG, "onOpened")
            synchronized(cameraDeviceLock) {
                cameraDevice = camera
            }
            openCaptureSession()
        }

        override fun onDisconnected(camera: CameraDevice) {
            FkLogcat.i(TAG, "onDisconnected")
            closeCaptureSession()
        }

        override fun onError(camera: CameraDevice, error: Int) {
            FkLogcat.i(TAG, "onError error=$error")
            synchronized(cameraDeviceLock) {
                cameraDevice = null
            }
        }

        override fun onClosed(camera: CameraDevice) {
            FkLogcat.i(TAG, "CameraDevice onClosed")
            super.onClosed(camera)
            synchronized(cameraDeviceLock) {
                cameraDevice = null
            }
            synchronized(this@FkCamera2) {
                state = FkCameraState.DESTROYED
            }
            dispatchInfo(FkResult.INFO_CAMERA_STOPPED, 0, null)
        }
    }
}