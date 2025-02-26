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
#include "Java_FkSurfaceTextureSource.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkSurfaceTextureSource {
#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle) {
        nativeDestroy(env, cls, handle);
    }

    jlong _nativeCreateSurface(JNIEnv *env, jclass cls, jobject instance) {
        return (jlong)nativeCreateSurface(env, cls, instance);
    }

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/source/FkSurfaceTextureSource";

    const int METHODS_NUM = 2;

    const JNINativeMethod METHODS[] = {
            {"nativeDestroy", "(J)V", (void *) _nativeDestroy},
            {"nativeCreateSurface", "(Lcom/alimin/fk/core/FkAbsImageSource;)J", (void *) _nativeCreateSurface},
    };

#ifdef __cplusplus
    }
#endif
};