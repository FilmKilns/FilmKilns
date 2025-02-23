/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-23 14:50:54
*/

#include "Java_FkSurfaceTextureSource.h"
#include "FkInstanceHolder.h"
#include "FkSurfaceTextureSource.h"

namespace Java_FkSurfaceTextureSource {

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        FkInstanceHolder::getInstance().release(handle);
    }

    int64_t nativeCreateSurface(JNIEnv *env, jclass cls, jobject instance) {
        std::shared_ptr<FkAbsImageSource> source = std::make_shared<FkSurfaceTextureSource>(instance);
        return FkInstanceHolder::getInstance().put(source);
    }
}