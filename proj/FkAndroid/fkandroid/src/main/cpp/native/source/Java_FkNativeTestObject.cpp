//
// Created by Alimin on 2025/2/22.
//

#include "Java_FkNativeTestObject.h"
namespace Java_FkNativeTestObject {
    int32_t nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        return 0;
    }

    int64_t nativeCreate(JNIEnv *env, jclass cls, bool arg0, uint8_t arg1, char arg2, int16_t arg3, int32_t arg4, int64_t arg5, float arg6, double arg7, const std::shared_ptr<FkJString> & arg8, std::any arg9, const std::shared_ptr<FkJBuffer> & arg10) {
        return 0;
    }

    const std::shared_ptr<FkJBuffer> & nativeGetByteArray(JNIEnv *env, jclass cls, const std::shared_ptr<FkJBuffer> & val) {
        return val;
    }

    const std::shared_ptr<FkJString> & nativeGetString(JNIEnv *env, jclass cls, const std::shared_ptr<FkJString> & val) {
        return val;
    }

    double nativeGetDouble(JNIEnv *env, jclass cls, double val) {
        return val;
    }

    float nativeGetFloat(JNIEnv *env, jclass cls, float val) {
        return val;
    }

    int64_t nativeGetLong(JNIEnv *env, jclass cls, int64_t val) {
        return val;
    }

    int32_t nativeGetInt(JNIEnv *env, jclass cls, int32_t val) {
        return val;
    }

    int16_t nativeGetShort(JNIEnv *env, jclass cls, int16_t val) {
        return val;
    }

    char nativeGetChar(JNIEnv *env, jclass cls, char val) {
        return val;
    }

    uint8_t nativeGetByte(JNIEnv *env, jclass cls, uint8_t val) {
        return val;
    }

    bool nativeGetBool(JNIEnv *env, jclass cls, bool val) {
        return val;
    }
}