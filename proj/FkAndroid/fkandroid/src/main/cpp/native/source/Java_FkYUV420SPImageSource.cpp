/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-23 15:39:54
*/
#include "Java_FkYUV420SPImageSource.h"
#include "FkInstanceHolder.h"
#include "FkYUV420SPImageSource.h"

namespace Java_FkYUV420SPImageSource {

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        FkInstanceHolder::getInstance().release(handle);
    }

    int64_t nativeCreate(JNIEnv *env, jclass cls, jobject instance, jobject y, jobject u, jobject v, int32_t width, int32_t height, int32_t orientation) {
        if (y != nullptr && u != nullptr) {
            size_t size = 2;
            if (v != nullptr) {
                size = 3;
            }
            std::vector<std::shared_ptr<FkBuffer>> _planes(size);

            auto capacity = env->GetDirectBufferCapacity(y);
            auto address = env->GetDirectBufferAddress(y);
            _planes[0] = FkBuffer::alloc(capacity + 1);
            _planes[0]->put(address, capacity);
            env->DeleteLocalRef(y);

            capacity = env->GetDirectBufferCapacity(u);
            address = env->GetDirectBufferAddress(u);
            _planes[1] = FkBuffer::alloc(capacity + 1);
            _planes[1]->put(address, capacity);
            env->DeleteLocalRef(u);

            if (v != nullptr) {
                capacity = env->GetDirectBufferCapacity(v);
                address = env->GetDirectBufferAddress(v);
                _planes[2] = FkBuffer::alloc(capacity + 1);
                _planes[2]->put(address, capacity);
                env->DeleteLocalRef(v);
            }

            FkBufferedImageSource::BufferDesc desc = {_planes, {width, height}, (FkEncodedOrigin) orientation};
            auto source = std::make_shared<FkYUV420SPImageSource>(desc);
            return FkInstanceHolder::getInstance().put(std::dynamic_pointer_cast<FkAbsImageSource>(source));
        }
        return 0;
    }

}