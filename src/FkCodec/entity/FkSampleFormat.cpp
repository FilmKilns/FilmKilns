/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSampleFormat.h"
#include "FkFFUtils.h"

FK_IMPL_CLASS_TYPE(FkSampleFormat, FkObject)

const FkSampleFormat FkSampleFormat::NONE = FkSampleFormat(kFrameFormat::NONE, 0, 0);

FkSampleFormat::FkSampleFormat(kFrameFormat format,
                               uint16_t channels,
                               uint32_t sampleRate)
        : FkObject() {
    this->format = format;
    this->channels = channels;
    this->sampleRate = sampleRate;
}

FkSampleFormat::FkSampleFormat(const FkSampleFormat &o) : FkObject() {
    this->format = o.format;
    this->channels = o.channels;
    this->sampleRate = o.sampleRate;
}

FkSampleFormat::~FkSampleFormat() {
    this->format = kFrameFormat::NONE;
    this->channels = 0;
    this->sampleRate = 0;
}

kFrameFormat FkSampleFormat::getFormat() { return format; }

int32_t FkSampleFormat::getAVFormat() {
    return FkFFUtils::convertAudioFrameFormat(getFormat());
}

uint16_t FkSampleFormat::getChannels() { return channels; }

uint32_t FkSampleFormat::getSampleRate() { return sampleRate; }

FkSampleFormat &FkSampleFormat::operator=(const FkSampleFormat &o) {
    this->format = o.format;
    this->channels = o.channels;
    this->sampleRate = o.sampleRate;
    return *this;
}

bool FkSampleFormat::operator==(const FkSampleFormat &o) {
    return this->format == o.format &&
           this->channels == o.channels &&
           this->sampleRate == o.sampleRate;
}

bool FkSampleFormat::operator!=(const FkSampleFormat &o) {
    return this->format != o.format ||
           this->channels != o.channels ||
           this->sampleRate != o.sampleRate;
}

size_t FkSampleFormat::getBytesPerSample() {
    return static_cast<size_t>(FkFFUtils::getBytesPerSample(format) * channels);
}

bool FkSampleFormat::valid() {
    bool validChannel = 1 == channels || 2 == channels;
    bool validSampleRate = 8000 == sampleRate || 16000 == sampleRate || 32000 == sampleRate ||
                           44100 == sampleRate || 48000 == sampleRate;
    return kFrameFormat::NONE != format && validChannel && validSampleRate;
}
