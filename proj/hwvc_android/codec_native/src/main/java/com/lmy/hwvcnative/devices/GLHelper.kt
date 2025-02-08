/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

import android.app.ActivityManager
import android.content.Context
import android.opengl.GLES20
import android.util.Log

/**
 * Created by aliminabc@gmail.com on 2018/8/6.
 */
object GLHelper {
//    init {
//        System.loadLibrary("glhelper")
//    }

    private val PBO_SUPPORT_VERSION = 0x30000

//    external fun glReadPixels(x: Int,
//                              y: Int,
//                              width: Int,
//                              height: Int,
//                              format: Int,
//                              type: Int)
//
//    external fun copyToByteArray(src: ByteBuffer,
//                                 dest: ByteArray,
//                                 row: Int,
//                                 stride: Int,
//                                 stridePadding: Int)
//
//    external fun memcpy(src: ByteArray,
//                        dest: ByteArray,
//                        length: Int)
//
//    external fun glVersion(): String
    /**
     * @param context
     * @return hex
     */
    fun glVersion(context: Context): Int {
        val am = context.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val info = am.deviceConfigurationInfo ?: return 0
        return info.reqGlEsVersion
    }

    fun isSupportPBO(context: Context): Boolean {
        return GLHelper.glVersion(context) > PBO_SUPPORT_VERSION
    }

    fun checkGLES2Error(tag: String): Int {
        val error = GLES20.glGetError()
        if (error != GLES20.GL_NO_ERROR) {
            Log.e(tag, "glError 0x" + Integer.toHexString(error))
        }
        return error
    }
}