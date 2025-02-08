/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Egl.h"
#include "log.h"
#include <android/native_window_jni.h>

#define TAG "Egl"

const int CONFIG_WIN[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                          EGL_RED_SIZE, 8,
                          EGL_GREEN_SIZE, 8,
                          EGL_BLUE_SIZE, 8,
                          EGL_ALPHA_SIZE, 8,
                          EGL_DEPTH_SIZE, 0,
                          EGL_STENCIL_SIZE, 0,
                          EGL_NONE};
const int CONFIG_BUFFER[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                             EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                             EGL_RED_SIZE, 8,
                             EGL_GREEN_SIZE, 8,
                             EGL_BLUE_SIZE, 8,
                             EGL_ALPHA_SIZE, 8,
                             EGL_DEPTH_SIZE, 0,
                             EGL_STENCIL_SIZE, 0,
                             EGL_NONE};

//eglGetProcAddress( "eglPresentationTimeANDROID")
/**
 * 1. eglGetConfigs
 * 2. eglBindAPI
 * 3. eglSwapInterval
 * 4. ANativeWindow_setBuffersGeometry
 */
EGLContext Egl::currentContext() {
    EGLContext context = eglGetCurrentContext();
    if (EGL_NO_CONTEXT == context) {
        Logcat::i(TAG, "Egl: Current thread has non context.");
    }
    return context;
}

Egl *Egl::create(EGLContext context, HwWindow *win, bool focusTypeWin) {
    return new Egl(context, win, focusTypeWin);
}

Egl::Egl() : Object() {
    focusTypeWin = false;
    init(nullptr, nullptr);
}

Egl::Egl(EGLContext context) : Object() {
    focusTypeWin = false;
    init(context, nullptr);
}

Egl::Egl(HwWindow *win) : Object() {
    focusTypeWin = nullptr != win;
    init(nullptr, win);
}

Egl::Egl(EGLContext context, HwWindow *win) : Object() {
    focusTypeWin = nullptr != win;
    init(context, win);
}

Egl::Egl(EGLContext context, HwWindow *win, bool focusTypeWin) : Object() {
    this->focusTypeWin = focusTypeWin || nullptr != win;
    init(context, win);
}

Egl::~Egl() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        if (!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            checkError();
            Logcat::e(TAG, "~Egl makeCurrent failed");
        }
        if (eglSurface == EGL_NO_SURFACE || EGL_TRUE != eglDestroySurface(eglDisplay, eglSurface)) {
            Logcat::e(TAG, "~Egl eglDestroySurface failed");
        }
        if (eglContext == EGL_NO_CONTEXT || EGL_TRUE != eglDestroyContext(eglDisplay, eglContext)) {
            Logcat::e(TAG, "~Egl eglDestroyContext failed");
        }
        if (EGL_TRUE != eglTerminate(eglDisplay)) {
            Logcat::e(TAG, "~Egl eglTerminate failed");
        }
    }
    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;
    eglDisplay = EGL_NO_DISPLAY;
    eglConfig = nullptr;
    if (this->win) {
        delete this->win;
        this->win = nullptr;
    }
    Logcat::i(TAG, "Egl::~Egl");
}

void Egl::init(EGLContext context, HwWindow *win) {
    if (EGL_NO_DISPLAY != eglDisplay
        || EGL_NO_SURFACE != eglContext
        || EGL_NO_SURFACE != eglSurface) {
        Logcat::e(TAG, "Dirty env!!!!!!!!!!");
        return;
    }
    this->win = win;
    createDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == this->eglDisplay) {
        Logcat::e(TAG, "$s failed", __func__);
        return;
    }
    if (focusTypeWin || (win && win->getANativeWindow())) {
        createConfig(CONFIG_WIN);
    } else {
        createConfig(CONFIG_BUFFER);
    }
    if (!this->eglConfig) {
        Logcat::e(TAG, "$s bad config", __func__);
        return;
    }
    if (this->win) {
        createWindowSurface(this->win);
    } else {
        createPbufferSurface();
    }
    if (EGL_NO_SURFACE == this->eglSurface) {
        Logcat::e(TAG, "$s bad surface", __func__);
        return;
    }
    if (context) {
        createContext(context);
    } else {
        createContext(EGL_NO_CONTEXT);
    }
    if (EGL_NO_CONTEXT == this->eglContext) {
        Logcat::e(TAG, "$s bad context", __func__);
        return;
    }
    makeCurrent();
    int width = 0, height = 0;
    if (!eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width) ||
        !eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height)) {
        Logcat::e(TAG, "Egl init failed");
    }

#ifdef __ANDROID__
    eglPresentationTimeANDROID = reinterpret_cast<EGL_PRESENTATION_TIME_ANDROID>(eglGetProcAddress(
            "eglPresentationTimeANDROID"));
#endif
    //If interval is set to a value of 0, buffer swaps are not synchronized to a video frame, and the swap happens as soon as the render is complete.
//    eglSwapInterval(eglDisplay, 0);
}

EGLDisplay Egl::createDisplay(EGLNativeDisplayType display_id) {
    eglDisplay = eglGetDisplay(display_id);
    if (EGL_NO_DISPLAY == eglDisplay || !checkError()) {
        Logcat::e(TAG, "eglGetDisplay failed");
        return EGL_NO_DISPLAY;
    }
    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(eglDisplay, // 创建的EGL连接
                       &majorVersion, // 返回EGL主板版本号
                       &minorVersion) || !checkError()) { // 返回EGL次版本号
        Logcat::e(TAG, "eglInitialize failed");
        return EGL_NO_DISPLAY;
    }
    return eglDisplay;
}

EGLConfig Egl::createConfig(const int *configSpec) {
    EGLint configsCount = 0;
    const EGLint maxConfigs = 2;
    EGLConfig configs[maxConfigs];
    //Get a list of all EGL frame buffer configurations for a display
    EGLBoolean ret = eglGetConfigs(eglDisplay, configs, maxConfigs, &configsCount);
    if (ret != EGL_TRUE || configsCount <= 0) {
        Logcat::e(TAG, "eglChooseConfig failed");
        return nullptr;
    }

    // Get a list of EGL frame buffer configurations that match specified attributes
    ret = eglChooseConfig(eglDisplay, // 创建的和本地窗口系统的连接
                          configSpec, // 指定渲染表面的参数列表，可以为null
                          configs, // 调用成功，返会符合条件的EGLConfig列表
                          maxConfigs, // 最多返回的符合条件的EGLConfig个数
                          &configsCount); // 实际返回的符合条件的EGLConfig个数
    if (EGL_TRUE != ret || configsCount <= 0 || !checkError()) {
        Logcat::e(TAG, "eglChooseConfig failed");
        return nullptr;
    }
    eglConfig = configs[0];
    if (win && win->getANativeWindow()) {
        EGLint format;
        if (EGL_TRUE != eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format)) {
            Logcat::e(TAG, "eglGetConfigAttrib failed");
        } else {
            ANativeWindow_setBuffersGeometry(win->getANativeWindow(), 0, 0, format);
        }
    }
    return configs[0];
}

EGLContext Egl::createContext(EGLContext context) {
    int contextSpec[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    eglContext = eglCreateContext(eglDisplay, eglConfig, context, contextSpec);
    if (EGL_NO_CONTEXT == eglContext || !checkError()) {
        Logcat::e(TAG, "eglCreateContext failed");
        return EGL_NO_CONTEXT;
    }
    return eglContext;
}

EGLSurface Egl::createPbufferSurface() {
//    EGLint values;
//    eglQueryContext(eglDisplay, eglContext, EGL_CONTEXT_CLIENT_VERSION, &values);
    int attrib_list[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, attrib_list);
    if (nullptr == eglSurface || EGL_NO_SURFACE == eglSurface || !checkError()) {
        Logcat::e(TAG, "eglCreatePbufferSurface failed");
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        Logcat::e(TAG, "eglBindAPI failed");
        return EGL_NO_SURFACE;
    }
    return eglSurface;
}

EGLSurface Egl::createWindowSurface(HwWindow *win) {
//    EGLint values;
//    eglQueryContext(eglDisplay, eglContext, EGL_CONTEXT_CLIENT_VERSION, &values);
    int attrib_list[] = {EGL_NONE};
    eglSurface = eglCreateWindowSurface(eglDisplay,
                                        eglConfig, // 选好的可用EGLConfig
                                        win->getANativeWindow(), // 指定原生窗口
                                        attrib_list); // 指定窗口属性列表，可以为null，一般指定渲染所用的缓冲区使用但缓冲或者后台缓冲，默认为后者。
    if (nullptr == eglSurface || EGL_NO_SURFACE == eglSurface || !checkError()) {
        Logcat::e(TAG, "eglCreateWindowSurface failed");
        return EGL_NO_SURFACE;
    }
    EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
    if (EGL_TRUE != ret) {
        Logcat::e(TAG, "eglBindAPI failed");
        return EGL_NO_SURFACE;
    }
    return eglSurface;
}

int Egl::width() {
    return getParams(EGL_WIDTH);
}

int Egl::height() {
    return getParams(EGL_HEIGHT);
}

EGLint Egl::getParams(EGLint attribute) {
    EGLint params;
    eglQuerySurface(eglDisplay, eglSurface, attribute, &params);
    return params;
}

void Egl::makeCurrent() {
    if (EGL_NO_DISPLAY == eglDisplay) {
        Logcat::e(TAG, "name egl failed had release!");
        return;
    }
    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) || !checkError()) {
        Logcat::e(TAG, "name makeCurrent failed");
    }
}

void Egl::swapBuffers() {
    if (!eglSwapBuffers(eglDisplay, eglSurface)) {
        Logcat::e(TAG, "name swapBuffers failed!");
    }
}

bool Egl::checkError() {
    EGLint error = eglGetError();
    if (EGL_SUCCESS != error) {
        Logcat::e(TAG, "Bad EGL environment: %d", error);
        return false;
    }
    return true;
}

bool Egl::updateWindow(HwWindow *win) {
    if (!focusTypeWin) {
        return false;
    }
    if (this->win) {
        delete this->win;
        this->win = nullptr;
    }
    this->win = win;
    if (EGL_NO_SURFACE != eglSurface) {
        eglDestroySurface(eglDisplay, eglSurface);
        eglSurface = EGL_NO_SURFACE;
    }
    createWindowSurface(this->win);
    makeCurrent();
    Logcat::e(TAG, "%s(%d) update window", __FUNCTION__, __LINE__);
    return true;
}

bool Egl::isAttachWindow() { return nullptr != win; }

EGLContext Egl::getContext() {
    return eglContext;
}

#ifdef __ANDROID__

bool Egl::setTimestamp(int64_t nsecs) {
    if (nullptr == eglPresentationTimeANDROID || EGL_NO_DISPLAY == eglDisplay) {
        return false;
    }
    return EGL_TRUE == eglPresentationTimeANDROID(eglDisplay, eglSurface, nsecs);
}

#endif