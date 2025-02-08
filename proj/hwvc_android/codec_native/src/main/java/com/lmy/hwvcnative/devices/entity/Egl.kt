/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices.entity

import android.graphics.SurfaceTexture
import android.opengl.*
import android.util.Log
import android.view.Surface
import javax.microedition.khronos.egl.EGL10


/**
 * Created by aliminabc@gmail.com on 2018/3/26.
 */
class Egl(private val name: String) {
    companion object {
        const val COLOR_CHANNELS = 4
        const val GL_COLOR_DEFAULT = GLES20.GL_RGBA
        private const val EGL_RECORDABLE_ANDROID = 0x3142
        private val CONFIG_DEFAULT = intArrayOf(
                EGL14.EGL_RENDERABLE_TYPE, EGL14.EGL_OPENGL_ES2_BIT,
                EGL14.EGL_RED_SIZE, 8,
                EGL14.EGL_GREEN_SIZE, 8,
                EGL14.EGL_BLUE_SIZE, 8,
                EGL14.EGL_ALPHA_SIZE, 8,
                EGL14.EGL_DEPTH_SIZE, 0,
                EGL14.EGL_STENCIL_SIZE, 0,
                EGL14.EGL_NONE
        )
        private val CONFIG_ANDROID = intArrayOf(
                EGL14.EGL_RENDERABLE_TYPE, EGL14.EGL_OPENGL_ES2_BIT,
                EGL14.EGL_RED_SIZE, 8,
                EGL14.EGL_GREEN_SIZE, 8,
                EGL14.EGL_BLUE_SIZE, 8,
                EGL14.EGL_ALPHA_SIZE, 8,
                EGL_RECORDABLE_ANDROID, 1,
                EGL14.EGL_DEPTH_SIZE, 0,
                EGL14.EGL_STENCIL_SIZE, 0,
                EGL14.EGL_NONE
        )
    }

    var eglDisplay: EGLDisplay? = null
    var eglConfig: EGLConfig? = null
    var eglSurface: EGLSurface? = null
    var eglContext: EGLContext? = null
    val width: Int
        get() {
            val result = IntArray(1)
            EGL14.eglQuerySurface(eglDisplay, eglSurface, EGL14.EGL_WIDTH, result, result.size)
            return result[0]
        }
    val height: Int
        get() {
            val result = IntArray(1)
            EGL14.eglQuerySurface(eglDisplay, eglSurface, EGL14.EGL_HEIGHT, result, result.size)
            return result[0]
        }

    private fun createDisplay(displayId: Int): EGLDisplay? {
        val eglDisplay = EGL14.eglGetDisplay(displayId)
        if (EGL14.EGL_NO_DISPLAY === eglDisplay) {
            Log.e(name, "eglGetDisplay,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return null
        }
        val versions = IntArray(2)
        if (!EGL14.eglInitialize(eglDisplay, versions, 0, versions, 1)) {
            Log.e(name, "eglInitialize,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return null
        }
        return eglDisplay
    }

    private fun createConfig(display: EGLDisplay, configSpec: IntArray): EGLConfig? {
        val configsCount = IntArray(1)
        val configs = arrayOfNulls<EGLConfig>(1)
        EGL14.eglChooseConfig(display, configSpec, 0,
                configs, 0, 1, configsCount, 0)
        if (configsCount[0] <= 0) {
            Log.e(name, "eglChooseConfig,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return null
        }
        return configs[0]
    }

    private fun createContext(display: EGLDisplay, config: EGLConfig, context: EGLContext?): EGLContext? {
        val contextSpec = intArrayOf(EGL14.EGL_CONTEXT_CLIENT_VERSION, 2, EGL14.EGL_NONE)
        val eglContext = EGL14.eglCreateContext(display, config,
                context ?: EGL14.EGL_NO_CONTEXT, contextSpec, 0)
        if (EGL14.EGL_NO_CONTEXT === eglContext) {
            Log.e(name, "eglCreateContext,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return null
        }
        return eglContext
    }

    private fun createPbufferSurface() {
        val surfaceAttribs = intArrayOf(EGL10.EGL_WIDTH, 1, EGL10.EGL_HEIGHT, 1, EGL14.EGL_NONE)
        val values = IntArray(1)
        EGL14.eglQueryContext(eglDisplay, eglContext, EGL14.EGL_CONTEXT_CLIENT_VERSION, values, 0)
        eglSurface = EGL14.eglCreatePbufferSurface(eglDisplay, eglConfig, surfaceAttribs, 0)
        if (null == eglSurface || EGL14.EGL_NO_SURFACE == eglSurface) {
            Log.e(name, "eglCreateWindowSurface,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return
        }
    }

    private fun createWindowSurface(surface: Any) {
        val values = IntArray(1)
        val surfaceAttribs = intArrayOf(EGL14.EGL_NONE)
        EGL14.eglQueryContext(eglDisplay, eglContext, EGL14.EGL_CONTEXT_CLIENT_VERSION, values, 0)
        eglSurface = EGL14.eglCreateWindowSurface(eglDisplay, eglConfig, surface, surfaceAttribs, 0)
        if (null == eglSurface || EGL14.EGL_NO_SURFACE == eglSurface) {
            Log.e(name, "eglCreateWindowSurface,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()))
            return
        }
    }

    fun initEGL() {
        eglDisplay = createDisplay(EGL14.EGL_DEFAULT_DISPLAY)
        if (null == eglDisplay) return
        eglConfig = createConfig(eglDisplay!!, CONFIG_DEFAULT)
        if (null == eglConfig) return
        eglContext = createContext(eglDisplay!!, eglConfig!!, null)
        if (null == eglContext) return
        createPbufferSurface()
    }

    fun initEGL(surfaceTexture: SurfaceTexture, context: EGLContext?) {
        eglDisplay = createDisplay(EGL14.EGL_DEFAULT_DISPLAY)
        if (null == eglDisplay) return
        eglConfig = createConfig(eglDisplay!!, CONFIG_DEFAULT)
        if (null == eglConfig) return
        eglContext = createContext(eglDisplay!!, eglConfig!!, context)
        if (null == eglContext) return
        createWindowSurface(surfaceTexture)
    }

    fun initEGL(surface: Surface, context: EGLContext?) {
        eglDisplay = createDisplay(EGL14.EGL_DEFAULT_DISPLAY)
        if (null == eglDisplay) return
        eglConfig = createConfig(eglDisplay!!, CONFIG_ANDROID)
        if (null == eglConfig) return
        eglContext = createContext(eglDisplay!!, eglConfig!!, context)
        if (null == eglContext) return
        createWindowSurface(surface)
    }

    fun makeCurrent() {
        synchronized(this) {
            if (EGL14.EGL_NO_CONTEXT == eglContext) {
                Log.e(name, "$name egl failed had release!")
                return
            }
            if (!EGL14.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
                //            throw new RuntimeException("eglMakeCurrent,failed:" + GLUtils.getEGLErrorString(EGL14.eglGetError()));
                Log.e(name, "$name makeCurrent failed: ${GLUtils.getEGLErrorString(EGL14.eglGetError())}")
            }

        }
    }

    fun swapBuffers() {
        synchronized(this) {
            if (!EGL14.eglSwapBuffers(eglDisplay, eglSurface)) {
                Log.e(name, "$name swapBuffers,failed!")
            }
        }
    }

    fun setPresentationTime(nsecs: Long) {
        EGLExt.eglPresentationTimeANDROID(eglDisplay, eglSurface, nsecs)
    }

    fun release() {
        synchronized(this) {
            makeCurrent()
            EGL14.eglDestroySurface(eglDisplay, eglSurface)
            EGL14.eglDestroyContext(eglDisplay, eglContext)
            EGL14.eglTerminate(eglDisplay)
            EGL14.eglMakeCurrent(eglDisplay, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT)
            eglContext = EGL14.EGL_NO_CONTEXT
            eglDisplay = EGL14.EGL_NO_DISPLAY
            eglSurface = EGL14.EGL_NO_SURFACE
        }
    }
}