/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_CODEC_FKVIDEOFRAME_H
#define FK_CODEC_FKVIDEOFRAME_H

#include "FkAbsMediaFrame.h"

FK_SUPER_CLASS(FkVideoFrame, FkAbsMediaFrame) {
FK_DEF_CLASS_TYPE_FUNC(FkVideoFrame)

public:
    static const int32_t HW_PIC_DEF;
    /** I frame */
    static const int32_t HW_PIC_I;
public:
    FkVideoFrame(FkSourcesAllocator *allocator,
                 kFrameFormat format,
                 uint32_t width,
                 uint32_t height);

    virtual ~FkVideoFrame();

    void setSize(uint32_t w, uint32_t h);

    uint32_t getWidth();

    uint32_t getHeight();

    virtual uint64_t duration() override;

    virtual FkAbsMediaFrame *clone() override;

    virtual void clone(FkAbsMediaFrame *src) override;

    void setPicType(int32_t _picType);

    int32_t getPicType();

private:
    FkVideoFrame(const FkVideoFrame &o)
            : FkAbsMediaFrame(nullptr, kFrameFormat::NONE, 0) {};

private:
    uint32_t width = 0;
    uint32_t height = 0;
    int32_t picType = HW_PIC_DEF;
};


#endif //FK_CODEC_FKVIDEOFRAME_H
