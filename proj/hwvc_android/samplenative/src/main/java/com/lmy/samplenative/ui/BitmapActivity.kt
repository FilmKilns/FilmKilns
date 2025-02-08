package com.lmy.samplenative.ui

import android.graphics.Bitmap
import android.graphics.ImageFormat
import android.graphics.Rect
import android.graphics.YuvImage
import com.lmy.samplenative.BaseActivity
import com.lmy.samplenative.R
import kotlinx.android.synthetic.main.activity_bitmap.*
import java.io.FileInputStream
import java.io.FileOutputStream
import java.nio.ByteBuffer

class BitmapActivity : BaseActivity() {
    override fun getLayoutResource(): Int = R.layout.activity_bitmap
    override fun initView() {
        enterBtn.setOnClickListener {
            show()
        }
    }

    private fun show() {
        val width = widthEdit.text.toString().toInt()
        val height = heightEdit.text.toString().toInt()
        val path = pathEdit.text.toString()
        val buf = ByteArray(width * height * 4)
        val ios = FileInputStream(path)
        val len = ios.read(buf)

        val image = YuvImage(buf, ImageFormat.NV21, width, height, null)
        image.compressToJpeg(Rect(0, 0, width, height), 80, FileOutputStream("/sdcard/pixels.jpg"))
//        val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
//        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(buf))
//        imageView.setImageBitmap(bitmap)
    }
}