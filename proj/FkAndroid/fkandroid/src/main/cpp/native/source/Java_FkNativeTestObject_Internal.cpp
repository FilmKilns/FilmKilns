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
#include "Java_FkNativeTestObject.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkNativeTestObject {
#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    jbyteArray _nativeGetByteArray(JNIEnv *env, jclass cls, jbyteArray val) {
        return nativeGetByteArray(env, cls, FkJBuffer::create(env, val))->jVal();
    }

    jstring _nativeGetString(JNIEnv *env, jclass cls, jstring val) {
        return nativeGetString(env, cls, FkJString::create(env, val))->jVal();
    }

    jdouble _nativeGetDouble(JNIEnv *env, jclass cls, jdouble val) {
        return (jdouble)nativeGetDouble(env, cls, val);
    }

    jfloat _nativeGetFloat(JNIEnv *env, jclass cls, jfloat val) {
        return (jfloat)nativeGetFloat(env, cls, val);
    }

    jlong _nativeGetLong(JNIEnv *env, jclass cls, jlong val) {
        return (jlong)nativeGetLong(env, cls, val);
    }

    jint _nativeGetInt(JNIEnv *env, jclass cls, jint val) {
        return (jint)nativeGetInt(env, cls, val);
    }

    jshort _nativeGetShort(JNIEnv *env, jclass cls, jshort val) {
        return (jshort)nativeGetShort(env, cls, val);
    }

    jchar _nativeGetChar(JNIEnv *env, jclass cls, jchar val) {
        return (jchar)nativeGetChar(env, cls, val);
    }

    jbyte _nativeGetByte(JNIEnv *env, jclass cls, jbyte val) {
        return (jbyte)nativeGetByte(env, cls, val);
    }

    jboolean _nativeGetBool(JNIEnv *env, jclass cls, jboolean val) {
        return nativeGetBool(env, cls, val = JNI_TRUE)? JNI_TRUE : JNI_FALSE;
    }

    jint _nativeDestroy(JNIEnv *env, jclass cls, jlong handle) {
        return (jint)nativeDestroy(env, cls, handle);
    }

    jlong _nativeCreate(JNIEnv *env, jclass cls, jboolean arg0, jbyte arg1, jchar arg2, jshort arg3, jint arg4, jlong arg5, jfloat arg6, jdouble arg7, jstring arg8, jobject arg9, jbyteArray arg10) {
        return (jlong)nativeCreate(env, cls, arg0 = JNI_TRUE, arg1, arg2, arg3, arg4, arg5, arg6, arg7, FkJString::create(env, arg8), arg9, FkJBuffer::create(env, arg10));
    }

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/source/FkNativeTestObject";

    const int METHODS_NUM = 12;

    const JNINativeMethod METHODS[] = {
            {"nativeGetByteArray", "([B)[B", (void *) _nativeGetByteArray},
            {"nativeGetString", "(Ljava/lang/String;)Ljava/lang/String;", (void *) _nativeGetString},
            {"nativeGetDouble", "(D)D", (void *) _nativeGetDouble},
            {"nativeGetFloat", "(F)F", (void *) _nativeGetFloat},
            {"nativeGetLong", "(J)J", (void *) _nativeGetLong},
            {"nativeGetInt", "(I)I", (void *) _nativeGetInt},
            {"nativeGetShort", "(S)S", (void *) _nativeGetShort},
            {"nativeGetChar", "(C)C", (void *) _nativeGetChar},
            {"nativeGetByte", "(B)B", (void *) _nativeGetByte},
            {"nativeGetBool", "(Z)Z", (void *) _nativeGetBool},
            {"nativeDestroy", "(J)I", (void *) _nativeDestroy},
            {"nativeCreate", "(ZBCSIJFDLjava/lang/String;Ljava/lang/Object;[B)J", (void *) _nativeCreate},
    };

#ifdef __cplusplus
    }
#endif
};