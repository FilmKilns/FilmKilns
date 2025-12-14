/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkVideoFrame.h"

#define TAG "FkVideoFrame"

FK_IMPL_CLASS_TYPE(FkVideoFrame, FkAbsMediaFrame)

const int32_t FkVideoFrame::HW_PIC_DEF = 0;
const int32_t FkVideoFrame::HW_PIC_I = 1;

FkVideoFrame::FkVideoFrame(FkSourcesAllocator *allocator,
                           kFrameFormat format,
                           uint32_t width,
                           uint32_t height)
        : FkAbsMediaFrame(allocator, format, FkAbsMediaFrame::getImageSize(format, width, height)) {
    this->width = width;
    this->height = height;
}

FkVideoFrame::~FkVideoFrame() {
    setSize(0, 0);
}

void FkVideoFrame::setSize(uint32_t w, uint32_t h) {
    this->width = w;
    this->height = h;
}

uint32_t FkVideoFrame::getWidth() { return width; }

uint32_t FkVideoFrame::getHeight() { return height; }

uint64_t FkVideoFrame::duration() {
    return 0;
}

FkAbsMediaFrame *FkVideoFrame::clone() {
    FkVideoFrame *destFrame = new FkVideoFrame(nullptr, getFormat(), width, height);
    destFrame->setPts(getPts());
    destFrame->setFormat(getFormat());
    destFrame->setFlags(flags());
    if (destFrame->size() > 0) {
        memcpy(destFrame->data(), data(), destFrame->size());
    }
    return destFrame;
}

void FkVideoFrame::clone(FkAbsMediaFrame *src) {
    if (!src || !src->isVideo() || src->size() < size()) {
        FkLogE(TAG, "Invalid video frame");
        return;
    }
    FkVideoFrame *srcFrame = dynamic_cast<FkVideoFrame *>(src);
    srcFrame->setPts(getPts());
    srcFrame->setFormat(getFormat());
    srcFrame->setSize(getWidth(), getHeight());
    srcFrame->setPicType(getPicType());
    srcFrame->setFlags(flags());
    if (srcFrame->size() > 0 && size() > 0) {
        memcpy(srcFrame->data(), data(), size());
    }
}

void FkVideoFrame::setPicType(int32_t _picType) { this->picType = _picType; }

int32_t FkVideoFrame::getPicType() { return picType; }