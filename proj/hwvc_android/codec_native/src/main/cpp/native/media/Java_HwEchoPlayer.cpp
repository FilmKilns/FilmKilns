/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <jni.h>
#include <log.h>
#include "HwEchoPlayer.h"

#ifdef __cplusplus
extern "C" {
#endif

static HwEchoPlayer *getHandler(jlong handler) {
    return reinterpret_cast<HwEchoPlayer *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_media_HwEchoPlayer_create
        (JNIEnv *env, jobject thiz, jint channels, jint sampleHz, jint format, jint minBufferSize) {
    return reinterpret_cast<jlong>(new HwEchoPlayer(channels, sampleHz, format, minBufferSize));
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_media_HwEchoPlayer_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_media_HwEchoPlayer_stop
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->stop();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_media_HwEchoPlayer_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwEchoPlayer *p = getHandler(handler);
        delete p;
    }
}

#ifdef __cplusplus
}
#endif