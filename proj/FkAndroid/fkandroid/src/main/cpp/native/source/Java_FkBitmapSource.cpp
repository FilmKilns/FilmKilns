/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-23 16:43:54
*/
#include "Java_FkCompressedImageSource.h"
#include "FkInstanceHolder.h"
#include "FkCompressedImageSource.h"
#include "FkBitmap.h"

namespace Java_FkBitmapSource {

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        FkInstanceHolder::getInstance().release(handle);
    }

    int64_t nativeCreate(JNIEnv *env, jclass cls, jobject instance, jobject buf, int32_t width, int32_t height) {
        auto address = env->GetDirectBufferAddress(buf);
        auto bitmap = FkBitmap::create(width, height);
        memcpy(bitmap->getPixels(), address, width * height * 4);
        auto source = std::make_shared<FkCompressedImageSource>(instance, bitmap, FkEncodedOrigin::kDefault);
        return FkInstanceHolder::getInstance().put(std::dynamic_pointer_cast<FkAbsImageSource>(source));
    }
}