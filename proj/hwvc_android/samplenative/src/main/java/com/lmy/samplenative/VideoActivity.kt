package com.lmy.samplenative

import android.content.Intent
import android.graphics.SurfaceTexture
import android.net.Uri
import android.os.Environment
import android.text.TextUtils
import android.util.Log
import android.view.KeyEvent
import android.view.Surface
import android.view.SurfaceHolder
import android.view.TextureView
import android.widget.SeekBar
import android.widget.Toast
import com.lmy.hwvcnative.processor.VideoProcessor
import kotlinx.android.synthetic.main.activity_video.*
import kotlinx.android.synthetic.main.view_filter.*
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

class VideoActivity : BaseActivity(),
        SeekBar.OnSeekBarChangeListener {

    private lateinit var mFilterController: FilterController
    private val formator = SimpleDateFormat("mm:ss")
    private var processor: VideoProcessor? = null
    private var prepared = false
    private var surface: Surface? = null
    private var playing: Boolean = true
    private var duration: Long = -1
    private val surfaceCallback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
            processor?.updateWindow(holder.surface)
        }

        override fun surfaceDestroyed(p0: SurfaceHolder?) {
            Log.i("HWVC", "surfaceDestroyed")
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            Log.i("HWVC", "surfaceCreated")
        }
    }

    override fun getLayoutResource(): Int = R.layout.activity_video
    override fun initView() {
        var uri = intent.data
        if (uri == null)
            uri = intent.getParcelableExtra(Intent.EXTRA_STREAM)
        if (uri == null) {
            val testFile = File(externalCacheDir, "/video/hw_small.mp4")
            if (!testFile.exists()) {
                Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
                finish()
                return
            }
            uri = Uri.fromFile(testFile)
        }
        val path = getRealFilePath(uri)
        if (TextUtils.isEmpty(path)) {
            Toast.makeText(this, "没有找到该文件", Toast.LENGTH_SHORT).show()
            finish()
            return
        }
        processor = lastCustomNonConfigurationInstance as VideoProcessor?
        if (null != processor) {
            prepared = true
        } else {
            processor = VideoProcessor()
            processor?.setSource(path!!)
        }
        processor?.setOnPlayProgressListener { us, duration ->
            timeView.text = formator.format(Date(us / 1000))
            seekBar.progress = (us * 100 / duration).toInt()
            if (this.duration < 0) {
                this.duration = duration
            }
        }
        mFilterController = FilterController(processor!!, progressLayout)
        filterBtn.setOnClickListener {
            mFilterController.chooseFilter(this)
        }
        seekBar.setOnSeekBarChangeListener(this)
        playBtn.setOnClickListener {
            if (playing) {
                playBtn.setImageResource(android.R.drawable.ic_media_play)
                processor?.pause()
            } else {
                playBtn.setImageResource(android.R.drawable.ic_media_pause)
                processor?.start()
            }
            playing = !playing
        }
        surfaceView.keepScreenOn = true
        surfaceView.holder.addCallback(surfaceCallback)
    }

    override fun onRetainCustomNonConfigurationInstance(): Any? {
        Log.i("HWVC", "VideoActivity onRetainCustomNonConfigurationInstance")
        return processor
    }

    override fun onResume() {
        super.onResume()
        processor?.start()
    }

    override fun onPause() {
        super.onPause()
        processor?.pause()
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.i("HWVC", "VideoActivity onDestroy.")
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            Log.i("HWVC", "VideoActivity back key pressed.")
            processor?.release()
            processor = null
        }
        return super.onKeyDown(keyCode, event)
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        if (fromUser) {
            processor?.seek(duration * progress.toLong() / 100)
        }
    }

    override fun onStartTrackingTouch(seekBar: SeekBar) {
        processor?.pause()
    }

    override fun onStopTrackingTouch(seekBar: SeekBar) {
        processor?.start()
    }
}