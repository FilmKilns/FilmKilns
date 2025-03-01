/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
*
* CREATE AUTO. DO NOT EDIT.
*/

/**
* +--------------------------------------------+
* |          CREATE AUTO. DO NOT EDIT          |
* +--------------------------------------------+
*/
#include "Java_FkImageModel.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkImageModel {

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    jint _nativeGetLayers(JNIEnv *env, jclass cls, jlong handle, jobject listener) {
        return (jint)nativeGetLayers(env, cls, handle, listener);
    }

    jint _nativeLoad(JNIEnv *env, jclass cls, jlong handle, jstring file, jobject listener) {
        return (jint)nativeLoad(env, cls, handle, FkJString::create(env, file), listener);
    }

    jint _nativeSave(JNIEnv *env, jclass cls, jlong handle, jstring file) {
        return (jint)nativeSave(env, cls, handle, FkJString::create(env, file));
    }

    void _nativeStop(JNIEnv *env, jclass cls, jlong handle) {
        nativeStop(env, cls, handle);
    }

    void _nativeStart(JNIEnv *env, jclass cls, jlong handle) {
        nativeStart(env, cls, handle);
    }

    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle) {
        nativeDestroy(env, cls, handle);
    }

    void _nativeCreate(JNIEnv *env, jclass cls, jlong handle) {
        nativeCreate(env, cls, handle);
    }

    jlong _nativeCreateInstance(JNIEnv *env, jclass cls, jlong imageEngineHandle) {
        return (jlong)nativeCreateInstance(env, cls, imageEngineHandle);
    }

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/engine/FkImageModel";

    const int METHODS_NUM = 8;

    const JNINativeMethod METHODS[] = {
            {"nativeGetLayers", "(JLcom/alimin/fk/listener/FkNativeMsgListener;)I", (void *) _nativeGetLayers},
            {"nativeLoad", "(JLjava/lang/String;Lcom/alimin/fk/listener/FkNativeMsgListener;)I", (void *) _nativeLoad},
            {"nativeSave", "(JLjava/lang/String;)I", (void *) _nativeSave},
            {"nativeStop", "(J)V", (void *) _nativeStop},
            {"nativeStart", "(J)V", (void *) _nativeStart},
            {"nativeDestroy", "(J)V", (void *) _nativeDestroy},
            {"nativeCreate", "(J)V", (void *) _nativeCreate},
            {"nativeCreateInstance", "(J)J", (void *) _nativeCreateInstance},
    };

};