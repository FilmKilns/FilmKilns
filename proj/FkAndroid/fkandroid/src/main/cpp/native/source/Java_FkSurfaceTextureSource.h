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

namespace Java_FkSurfaceTextureSource {
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * +----------------------------------------+
     * |  START of public interface. NEED IMPL  |
     * +----------------------------------------+
     */
    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle);

    int64_t nativeCreateSurface(JNIEnv *env, jclass cls, jobject instance);

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
    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle);

    jlong _nativeCreateSurface(JNIEnv *env, jclass cls, jobject instance);

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env);

    extern const char *CLASS_NAME;

    extern const int METHODS_NUM;

    extern const JNINativeMethod METHODS[2];

#ifdef __cplusplus
}
#endif
};