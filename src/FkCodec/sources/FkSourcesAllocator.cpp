/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkSourcesAllocator.h"

#define TAG "FkSourcesAllocator"

FK_IMPL_CLASS_TYPE(FkSources, FkObject)

FkSources::FkSources(FkSourcesAllocator *allocator) : FkObject() {
    this->allocator = allocator;
}

FkSources::~FkSources() {
    allocator = nullptr;
}

void FkSources::recycle() {
    if (!isDetach()) {
        FkSources *entity = this;
        if (!allocator->recycle(&entity)) {
            FkLogE(TAG, "FkSources recycle failed. Is it(%p) managed by allocator(%p)",
                      this,
                      allocator);
        }
    }
}

bool FkSources::isDetach() {
    return nullptr == allocator;
}

FK_IMPL_CLASS_TYPE(FkSourcesAllocator, FkObject)

FkSourcesAllocator::FkSourcesAllocator() : FkObject() {

}

FkSourcesAllocator::~FkSourcesAllocator() {

}