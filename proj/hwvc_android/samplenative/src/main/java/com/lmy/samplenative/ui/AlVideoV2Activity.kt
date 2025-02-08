package com.lmy.samplenative.ui

import android.content.Intent
import android.text.TextUtils
import android.util.Log
import android.view.KeyEvent
import android.view.SurfaceHolder
import android.widget.Toast
import com.lmy.hwvcnative.entity.AlLayer
import com.lmy.hwvcnative.entity.AlMediaTrack
import com.lmy.hwvcnative.entity.AlMediaType
import com.lmy.hwvcnative.processor.AlVideoV2Processor
import com.lmy.hwvcnative.tools.AlFFUtils
import com.lmy.hwvcnative.widget.AlTrackContainer
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_video_v2.*
import kotlinx.android.synthetic.main.activity_video_v2.surfaceView
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

class AlVideoV2Activity : BaseActivity() {
    override fun getLayoutResource(): Int = R.layout.activity_video_v2
    private var processor: AlVideoV2Processor? = AlVideoV2Processor()
    private val fmt = SimpleDateFormat("mm:ss")
    private var mCurrentLayer = AlLayer.none()
    private var playing: Boolean = true
    private var shouldSeek: Boolean = false
    private var duration: Long = -1
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            processor?.updateWindow(holder.surface)
        }

        override fun surfaceDestroyed(p0: SurfaceHolder?) {
            Log.i("AlVideoV2Activity", "surfaceDestroyed")
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i("AlVideoV2Activity", "surfaceCreated")
        }
    }

    override fun initView() {
        var uri = intent.data
        if (uri == null)
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        if (uri == null) {
            finish()
            return
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "File NOT exist.", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        setupView()
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
        processor?.setOnPlayProgressListener { timeInUS, duration ->
            onPlayProgress(timeInUS, duration)
        }
        processor?.setOnTrackUpdateListener {
            onTrackUpdate(it)
        }
        val info = AlFFUtils.trackInfo(path!!)
        if (info and 0x1 > 0) {
            addTrack(path!!, AlMediaType.TYPE_VIDEO)
        }
        if (info and 0x2 > 0) {
            addTrack(path!!, AlMediaType.TYPE_AUDIO)
        }
        processor?.start()
    }

    private fun addTrack(
        path: String, type: Int,
        seqInInUS: Long = 0,
        trimInInUS: Long = 0,
        durationInUS: Long = 0
    ) {
        val id = processor?.addTrack(type, path, seqInInUS, trimInInUS, durationInUS)
        if (null != id && id >= 0) {
            trackView.addTrack(AlMediaTrack(id, type))
        }
    }

    private fun onPlayProgress(timeInUS: Long, duration: Long) {
        this.duration = duration
        trackView.setDuration(duration)
        trackView.setProgress(timeInUS / duration.toFloat())
        timeView.text =
            "${fmt.format(Date(timeInUS / 1000))}/${fmt.format(Date(duration / 1000))}"
    }

    private fun onTrackUpdate(track: AlMediaTrack) {
        trackView.updateTrack(track)
    }

    private fun setupView() {
        playBtn.setOnClickListener {
            if (playing) {
                pausePlay()
            } else {
                startPlay()
            }
        }
        addBtn.setOnClickListener {
            addTrack(File(externalCacheDir, "/image/cat.gif").absolutePath, AlMediaType.TYPE_VIDEO)
        }
        trackView.setOnSeekBarChangeListener(object : AlTrackContainer.OnSeekBarChangeListener {
            override fun onProgressChanged(progress: Float, fromUser: Boolean) {
                if (fromUser) {
                    processor?.seek((duration * progress).toLong())
                }
            }

            override fun onStartTrackingTouch() {
                pausePlay()
            }

            override fun onStopTrackingTouch() {
                shouldSeek = true
            }
        })
        surfaceView?.setOnClickListener { v, x, y ->
            if (null != processor) {
                setCurLayer(AlLayer(processor!!.getLayer(x, y), 0, 0))
            }
        }
        surfaceView.setOnScrollListener { v, x, y, dx, dy, s ->
            processor?.postTranslate(getCurrentLayer().id, dx, dy)
        }
        surfaceView?.setOnScaleListener { v, ds, anchor ->
            processor?.postScale(getCurrentLayer().id, ds, anchor)
        }
    }

    fun setCurLayer(layer: AlLayer) {
        mCurrentLayer = layer
    }

    fun getCurrentLayer(): AlLayer = mCurrentLayer

    private fun pausePlay() {
        playBtn.setImageResource(android.R.drawable.ic_media_play)
        processor?.pause()
        playing = false
    }

    private fun startPlay() {
        playBtn.setImageResource(android.R.drawable.ic_media_pause)
        if (shouldSeek) {
            shouldSeek = false
            processor?.seek((duration * trackView.getProgress()).toLong())
        }
        processor?.start()
        playing = true
    }

    override fun onStop() {
        super.onStop()
        pausePlay()
    }

    override fun onDestroy() {
        super.onDestroy()
        processor?.release()
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            processor?.pause()
            processor?.release()
            processor = null
        }
        return super.onKeyDown(keyCode, event)
    }
}