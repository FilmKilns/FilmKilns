/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKSAMPLEFORMAT_H
#define FK_CODEC_FKSAMPLEFORMAT_H

#include "FkObject.h"
#include "FkCodecDefinition.h"

FK_SUPER_CLASS(FkSampleFormat, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSampleFormat)

public:
    FkSampleFormat(kFrameFormat format, uint16_t channels, uint32_t sampleRate);

    FkSampleFormat(const FkSampleFormat &o);

    virtual ~FkSampleFormat();

    uint16_t getChannels();

    uint32_t getSampleRate();

    kFrameFormat getFormat();

    int32_t getAVFormat();

    FkSampleFormat &operator=(const FkSampleFormat &o);

    bool operator==(const FkSampleFormat &o);

    bool operator!=(const FkSampleFormat &o);

    size_t getBytesPerSample();

    bool valid();

private:
    uint16_t channels = 0;
    uint32_t sampleRate = 0;
    kFrameFormat format = kFrameFormat::NONE;

public:
    static const FkSampleFormat NONE;
};


#endif //FK_CODEC_FKSAMPLEFORMAT_H
