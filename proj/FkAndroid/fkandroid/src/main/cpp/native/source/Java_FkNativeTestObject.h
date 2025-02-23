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

#pragma once
#include "FkJniDefinition.h"
#include <any>

namespace Java_FkNativeTestObject {

    int32_t nativeDestroy(JNIEnv *env, jclass cls, int64_t handle);

    int64_t nativeCreate(JNIEnv *env, jclass cls, bool arg0, uint8_t arg1, char arg2, int16_t arg3, int32_t arg4, int64_t arg5, float arg6, double arg7, const std::shared_ptr<FkJString> & arg8, std::any arg9, const std::shared_ptr<FkJBuffer> & arg10);

    jint _nativeDestroy(JNIEnv *env, jclass cls, jlong handle);

    jlong _nativeCreate(JNIEnv *env, jclass cls, jboolean arg0, jbyte arg1, jchar arg2, jshort arg3, jint arg4, jlong arg5, jfloat arg6, jdouble arg7, jstring arg8, jobject arg9, jbyteArray arg10);

    bool reg(JNIEnv *env);

    extern const char *CLASS_NAME;

    extern const int METHODS_NUM;

    extern const JNINativeMethod METHODS[2];

};