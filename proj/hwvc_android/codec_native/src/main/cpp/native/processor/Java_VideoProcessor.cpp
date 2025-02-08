/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "platform/android/AlJavaNativeHelper.h"
#include "HwVideoProcessor.h"
#include "../include/HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription vPlayProgressDesc = {
        "com/lmy/hwvcnative/processor/VideoProcessor",
        "onPlayProgress", "(JJ)V"};

static HwVideoProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwVideoProcessor *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setPlayProgressListener([handler](int64_t us, int64_t duration) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID methodID = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          vPlayProgressDesc,
                                                          &methodID)) {
            pEnv->CallVoidMethod(jObject, methodID, static_cast<jlong>(us),
                                 static_cast<jlong>(duration));
        }
    });
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_create
        (JNIEnv *env, jobject thiz) {
    HwVideoProcessor *p = new HwVideoProcessor();
    p->post([] {
        AlJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    AlJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    bindListener(handler);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_setSource
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        std::string pathStr(pPath);
        env->ReleaseStringUTFChars(path, pPath);
        getHandler(handler)->setSource(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_updateWindow
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_seek
        (JNIEnv *env, jobject thiz, jlong handler, jlong us) {
    if (handler) {
        getHandler(handler)->seek(us);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_setFilter
        (JNIEnv *env, jobject thiz, jlong handler, jlong filter) {
    if (handler && filter) {
        getHandler(handler)->setFilter(reinterpret_cast<HwAbsFilter *>(filter));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_VideoProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwVideoProcessor *p = getHandler(handler);
        p->post([] {
            AlJavaNativeHelper::getInstance()->detachThread();
        });
        p->release();
        delete p;
    }
    AlJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

#ifdef __cplusplus
}
#endif