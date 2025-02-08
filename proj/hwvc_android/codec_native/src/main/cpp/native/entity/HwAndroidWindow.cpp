/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAndroidWindow.h"
#include "log.h"

HwAndroidWindow::HwAndroidWindow() : HwWindow() {

}

HwAndroidWindow::HwAndroidWindow(JNIEnv *env, jobject surface) {
    setANativeWindow(ANativeWindow_fromSurface(env, surface));
    if (!getANativeWindow()) {
        LOGE("ANativeWindow_fromSurface failed");
        return;
    }
    setWidth(ANativeWindow_getWidth(getANativeWindow()));
    setHeight(ANativeWindow_getHeight(getANativeWindow()));
}

HwAndroidWindow::~HwAndroidWindow() {
    if (getANativeWindow()) {
        ANativeWindow_release(reinterpret_cast<ANativeWindow *>(getANativeWindow()));
    }
}