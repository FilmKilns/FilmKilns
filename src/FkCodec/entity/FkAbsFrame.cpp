/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAbsFrame.h"

FK_IMPL_CLASS_TYPE(FkAbsFrame, FkObject)

FkAbsFrame::FkAbsFrame(size_t size) : FkObject() {
    if (size > 0) {
        buf = FkBuffer::alloc(size);
    }
}

FkAbsFrame::~FkAbsFrame() {
    buf = nullptr;
}

uint8_t *FkAbsFrame::data() {
    if (buf) {
        return buf->data();
    }
    return nullptr;
}

size_t FkAbsFrame::size() {
    if (buf) {
        return buf->size();
    }
    return 0;
}