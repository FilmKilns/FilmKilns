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
#pragma once
#include "FkJniDefinition.h"
#include <any>

namespace Java_FkImageModel {

    /**
     * +----------------------------------------+
     * |  START of public interface. NEED IMPL  |
     * +----------------------------------------+
     */
    int32_t nativeGetLayers(JNIEnv *env, jclass cls, int64_t handle, jobject listener);

    int32_t nativeLoad(JNIEnv *env, jclass cls, int64_t handle, const std::shared_ptr<FkJString> & file, jobject listener);

    int32_t nativeSave(JNIEnv *env, jclass cls, int64_t handle, const std::shared_ptr<FkJString> & file);

    void nativeStop(JNIEnv *env, jclass cls, int64_t handle);

    void nativeStart(JNIEnv *env, jclass cls, int64_t handle);

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle);

    void nativeCreate(JNIEnv *env, jclass cls, int64_t handle);

    int64_t nativeCreateInstance(JNIEnv *env, jclass cls, int64_t imageEngineHandle);

    /**
     * +--------------------------------------+
     * |  END of public interface. NEED IMPL  |
     * +--------------------------------------+
     */

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    jint _nativeGetLayers(JNIEnv *env, jclass cls, jlong handle, jobject listener);

    jint _nativeLoad(JNIEnv *env, jclass cls, jlong handle, jstring file, jobject listener);

    jint _nativeSave(JNIEnv *env, jclass cls, jlong handle, jstring file);

    void _nativeStop(JNIEnv *env, jclass cls, jlong handle);

    void _nativeStart(JNIEnv *env, jclass cls, jlong handle);

    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle);

    void _nativeCreate(JNIEnv *env, jclass cls, jlong handle);

    jlong _nativeCreateInstance(JNIEnv *env, jclass cls, jlong imageEngineHandle);

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env);

    extern const char *CLASS_NAME;

    extern const int METHODS_NUM;

    extern const JNINativeMethod METHODS[8];

};