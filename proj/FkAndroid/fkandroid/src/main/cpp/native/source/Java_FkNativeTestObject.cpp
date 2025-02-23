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
}