/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-23 15:39:54
*/
#include "Java_FkCompressedImageSource.h"
#include "FkInstanceHolder.h"
#include "FkCompressedImageSource.h"

namespace Java_FkCompressedImageSource {

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        FkInstanceHolder::getInstance().release(handle);
    }

    int64_t nativeCreate(JNIEnv *env, jclass cls, jobject instance, jobject buf, int32_t orientation) {
        auto address = env->GetDirectBufferAddress(buf);
        auto _buf = FkBuffer::wrap((uint8_t *) address, env->GetDirectBufferCapacity(buf));
        auto source = std::make_shared<FkCompressedImageSource>(instance, FkBitmap::from(_buf), (FkEncodedOrigin) orientation);
        return FkInstanceHolder::getInstance().put(std::dynamic_pointer_cast<FkAbsImageSource>(source));
    }
}
