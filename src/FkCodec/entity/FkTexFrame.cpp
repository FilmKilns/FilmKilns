/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkTexFrame.h"

FK_IMPL_CLASS_TYPE(FkTexFrame, FkVideoFrame)

FkTexFrame::FkTexFrame(uint32_t tex, int w, int h)
        : FkVideoFrame(nullptr, kFrameFormat::IMAGE_RGBA, w, h), _tex(tex) {
}

FkTexFrame::FkTexFrame(const FkTexFrame &o)
        : FkVideoFrame(nullptr, kFrameFormat::IMAGE_RGBA, 0, 0),
          _tex(o._tex) {

}

FkTexFrame::~FkTexFrame() {
}

uint32_t FkTexFrame::tex() {
    return _tex;
}
