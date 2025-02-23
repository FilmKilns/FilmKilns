/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)

* CREATE AUTO. DO NOT EDIT.
*/

#include "Java_FkNativeTestObject.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkNativeTestObject {

    jint _nativeDestroy(JNIEnv *env, jclass cls, jlong handle) {
        return (jint)nativeDestroy(env, cls, handle);
    }

    jlong _nativeCreate(JNIEnv *env, jclass cls, jboolean arg0, jbyte arg1, jchar arg2, jshort arg3, jint arg4, jlong arg5, jfloat arg6, jdouble arg7, jstring arg8, jobject arg9, jbyteArray arg10) {
        return (jlong)nativeCreate(env, cls, arg0 = JNI_TRUE, arg1, arg2, arg3, arg4, arg5, arg6, arg7, FkJString::create(env, arg8), arg9, FkJBuffer::create(env, arg10));
    }

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/source/FkNativeTestObject";

    const int METHODS_NUM = 2;

    const JNINativeMethod METHODS[] = {
            {"_nativeDestroy", "(J)I", (void *) _nativeDestroy},
            {"_nativeCreate", "(ZBCSIJFDLjava/lang/String;Ljava/lang/Object;[B)J", (void *) _nativeCreate},
    };

};