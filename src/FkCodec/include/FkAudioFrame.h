/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_CODEC_FKAUDIOFRAME_H
#define FK_CODEC_FKAUDIOFRAME_H

#include "FkAbsMediaFrame.h"

FK_SUPER_CLASS(FkAudioFrame, FkAbsMediaFrame) {
FK_DEF_CLASS_TYPE_FUNC(FkAudioFrame)

public:
    FkAudioFrame(kFrameFormat format,
                 uint16_t channels,
                 uint32_t sampleRate,
                 uint64_t sampleCount);

    FkAudioFrame(FkSourcesAllocator *allocator,
                 kFrameFormat format,
                 uint16_t channels,
                 uint32_t sampleRate,
                 uint64_t sampleCount);

    virtual ~FkAudioFrame();

    uint16_t getChannels();

    uint32_t getSampleRate();

    uint64_t getSampleCount();

    void setSampleFormat(uint16_t _channels, uint32_t _sampleRate, uint64_t _sampleCount);

    virtual uint64_t duration() override;

    virtual FkAbsMediaFrame *clone() override;

    virtual void clone(FkAbsMediaFrame *src) override;

private:
    uint16_t channels = 0;
    uint32_t sampleRate = 0;
    uint64_t sampleCount = 0;
};


#endif //FK_CODEC_FKAUDIOFRAME_H
