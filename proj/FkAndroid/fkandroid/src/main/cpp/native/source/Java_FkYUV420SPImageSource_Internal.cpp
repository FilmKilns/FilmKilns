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
#include "Java_FkYUV420SPImageSource.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkYUV420SPImageSource {
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

    jlong _nativeCreate(JNIEnv *env, jclass cls, jobject instance, jobject y, jobject u, jobject v, jint width, jint height, jint orientation) {
        return (jlong)nativeCreate(env, cls, instance, y, u, v, width, height, orientation);
    }

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/source/FkYUV420SPImageSource";

    const int METHODS_NUM = 2;

    const JNINativeMethod METHODS[] = {
            {"nativeDestroy", "(J)V", (void *) _nativeDestroy},
            {"nativeCreate", "(Lcom/alimin/fk/core/FkAbsImageSource;Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;III)J", (void *) _nativeCreate},
    };

#ifdef __cplusplus
    }
#endif
};