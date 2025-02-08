/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVCSCREEN_ALENCODERBUILDER_H
#define HWVCSCREEN_ALENCODERBUILDER_H

#include "Object.h"
#include "AlSize.h"
#include "HwSampleFormat.h"
#include "AlAbsVideoEncoder.h"
#include "AlCodec.h"

al_class AlEncoderBuilder al_extend Object {
public:
    AlEncoderBuilder();

    virtual ~AlEncoderBuilder();

    AlEncoderBuilder &setSize(AlSize size);

    AlEncoderBuilder &setAudioFormat(HwSampleFormat &format);

    /// AlCodec::kType::SOFT
    /// \param type AlCodec::kType
    /// \return
    AlEncoderBuilder &setEncoderType(AlAbsEncoder::kType type);

    /// Default true.
    /// \param enable
    /// \return
    AlEncoderBuilder &setEnableAsync(bool enable);

    AlEncoderBuilder &setBitrate(int32_t rate);

    AlEncoderBuilder &setProfile(std::string profile);

    AlEncoderBuilder &setPreset(std::string preset);

    AlEncoderBuilder &setOutput(std::string output);

    AlAbsVideoEncoder *build();

private:
    AlSize size;
    HwSampleFormat audioFormat;
    AlAbsEncoder::kType type = AlAbsEncoder::kType::SOFT;
    bool enableAsync = true;
    int32_t bitrate = 0;
    std::string profile;
    std::string preset;
    std::string output;
};


#endif //HWVCSCREEN_ALENCODERBUILDER_H
