/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlEncoderBuilder.h"
#include "AlAsyncEncoder.h"
#include "AlFFEncoder.h"
#include "platform/android/encoder/HwAndroidEncoder.h"

#define TAG "AlEncoderBuilder"

AlEncoderBuilder::AlEncoderBuilder()
        : Object(), size(0, 0), audioFormat(HwFrameFormat::HW_FMT_NONE, 0, 0) {

}

AlEncoderBuilder::~AlEncoderBuilder() {

}

AlEncoderBuilder &AlEncoderBuilder::setSize(AlSize size) {
    this->size = size;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setAudioFormat(HwSampleFormat &format) {
    this->audioFormat = format;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setEncoderType(AlAbsEncoder::kType type) {
    this->type = type;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setEnableAsync(bool enable) {
    this->enableAsync = enable;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setBitrate(int32_t rate) {
    this->bitrate = rate;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setProfile(std::string profile) {
    this->profile = profile;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setPreset(std::string preset) {
    this->preset = preset;
    return *this;
}

AlEncoderBuilder &AlEncoderBuilder::setOutput(std::string output) {
    this->output = output;
    return *this;
}

AlAbsVideoEncoder *AlEncoderBuilder::build() {
    if (0 != size.width % 16 || 0 != size.height % 16) {
        AlLogE(TAG, "Not align 16. %dx%d", size.width, size.height);
        return nullptr;
    }
    AlAbsVideoEncoder *encoder = nullptr;
    AlAbsEncoder::Desc desc;
    desc.aID = AlCodec::kID::AAC;
    desc.vID = AlCodec::kID::H264;
    desc.type = type;
    if (enableAsync) {
        if (AlAbsEncoder::kType::SOFT == type) {
            encoder = new AlAsyncEncoder(desc);
        } else {
            if (AlCodec::kID::H264 == desc.vID) {
                encoder = new HwAndroidEncoder(desc);
            }
        }
    } else {
        if (AlAbsEncoder::kType::SOFT == type) {
            encoder = new AlFFEncoder(desc);
        } else {
            if (AlCodec::kID::H264 == desc.vID) {
                encoder = new HwAndroidEncoder(desc);
            }
        }
    }
    AlLogI(TAG,
           "Alloc encoder video(width=%d, height=%d, bitrate=%d, profile=%s, enc_type=%d), audio(fmt=%d, sample rate=%d, channels=%d), out=%s",
           size.width, size.height, bitrate, profile.c_str(), type,
           (int) audioFormat.getFormat(), audioFormat.getSampleRate(), audioFormat.getChannels(),
           output.c_str());
    encoder->setBitrate(bitrate);
    encoder->setProfile(profile);
    if (!encoder->prepare(output, size.width, size.height, audioFormat)) {
        AlLogE(TAG, "Prepare video encoder failed");
        delete encoder;
        return nullptr;
    }
    return encoder;
}
