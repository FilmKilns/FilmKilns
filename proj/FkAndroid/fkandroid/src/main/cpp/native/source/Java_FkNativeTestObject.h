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

namespace Java_FkNativeTestObject {

    /**
     * +----------------------------------------+
     * |  START of public interface. NEED IMPL  |
     * +----------------------------------------+
     */
    const std::shared_ptr<FkJBuffer> & nativeGetByteArray(JNIEnv *env, jclass cls, const std::shared_ptr<FkJBuffer> & val);

    const std::shared_ptr<FkJString> & nativeGetString(JNIEnv *env, jclass cls, const std::shared_ptr<FkJString> & val);

    double nativeGetDouble(JNIEnv *env, jclass cls, double val);

    float nativeGetFloat(JNIEnv *env, jclass cls, float val);

    int64_t nativeGetLong(JNIEnv *env, jclass cls, int64_t val);

    int32_t nativeGetInt(JNIEnv *env, jclass cls, int32_t val);

    int16_t nativeGetShort(JNIEnv *env, jclass cls, int16_t val);

    char nativeGetChar(JNIEnv *env, jclass cls, char val);

    uint8_t nativeGetByte(JNIEnv *env, jclass cls, uint8_t val);

    bool nativeGetBool(JNIEnv *env, jclass cls, bool val);

    int32_t nativeDestroy(JNIEnv *env, jclass cls, int64_t handle);

    int64_t nativeCreate(JNIEnv *env, jclass cls, bool arg0, uint8_t arg1, char arg2, int16_t arg3, int32_t arg4, int64_t arg5, float arg6, double arg7, const std::shared_ptr<FkJString> & arg8, std::any arg9, const std::shared_ptr<FkJBuffer> & arg10);

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
    jbyteArray _nativeGetByteArray(JNIEnv *env, jclass cls, jbyteArray val);

    jstring _nativeGetString(JNIEnv *env, jclass cls, jstring val);

    jdouble _nativeGetDouble(JNIEnv *env, jclass cls, jdouble val);

    jfloat _nativeGetFloat(JNIEnv *env, jclass cls, jfloat val);

    jlong _nativeGetLong(JNIEnv *env, jclass cls, jlong val);

    jint _nativeGetInt(JNIEnv *env, jclass cls, jint val);

    jshort _nativeGetShort(JNIEnv *env, jclass cls, jshort val);

    jchar _nativeGetChar(JNIEnv *env, jclass cls, jchar val);

    jbyte _nativeGetByte(JNIEnv *env, jclass cls, jbyte val);

    jboolean _nativeGetBool(JNIEnv *env, jclass cls, jboolean val);

    jint _nativeDestroy(JNIEnv *env, jclass cls, jlong handle);

    jlong _nativeCreate(JNIEnv *env, jclass cls, jboolean arg0, jbyte arg1, jchar arg2, jshort arg3, jint arg4, jlong arg5, jfloat arg6, jdouble arg7, jstring arg8, jobject arg9, jbyteArray arg10);

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env);

    extern const char *CLASS_NAME;

    extern const int METHODS_NUM;

    extern const JNINativeMethod METHODS[12];

};