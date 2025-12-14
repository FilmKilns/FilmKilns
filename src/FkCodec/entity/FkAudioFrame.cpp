/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkAudioFrame.h"

#define TAG "FkAudioFrame"

FK_IMPL_CLASS_TYPE(FkAudioFrame, FkAbsMediaFrame)

FkAudioFrame::FkAudioFrame(kFrameFormat format,
                           uint16_t channels,
                           uint32_t sampleRate,
                           uint64_t sampleCount)
        : FkAudioFrame(nullptr, format, channels, sampleRate, sampleCount) {
}

FkAudioFrame::FkAudioFrame(FkSourcesAllocator *allocator,
                           kFrameFormat format,
                           uint16_t channels,
                           uint32_t sampleRate,
                           uint64_t sampleCount)
        : FkAbsMediaFrame(allocator, format, sampleCount * channels * getBytesPerSample(format)) {
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->sampleCount = sampleCount;
}

FkAudioFrame::~FkAudioFrame() {
    channels = 0;
    sampleRate = 0;
    sampleCount = 0;
}

uint16_t FkAudioFrame::getChannels() { return channels; }

uint32_t FkAudioFrame::getSampleRate() { return sampleRate; }

uint64_t FkAudioFrame::getSampleCount() { return sampleCount; }

void FkAudioFrame::setSampleFormat(uint16_t _channels, uint32_t _sampleRate, uint64_t _sampleCount) {
    this->channels = _channels;
    this->sampleRate = _sampleRate;
    this->sampleCount = _sampleCount;
}

uint64_t FkAudioFrame::duration() {
    return 1000000 * sampleCount / sampleRate;
}

FkAbsMediaFrame *FkAudioFrame::clone() {
    FkAudioFrame *destFrame = new FkAudioFrame(nullptr, getFormat(),
                                               channels, sampleRate,
                                               sampleCount);
    destFrame->setPts(getPts());
    destFrame->setFormat(getFormat());
    memcpy(destFrame->data(), data(), destFrame->size());
    return destFrame;
}

void FkAudioFrame::clone(FkAbsMediaFrame *src) {
    if (!src || !src->isAudio() || src->size() < size()) {
        FkLogE(TAG, "Invalid audio frame");
        return;
    }
    FkAudioFrame *srcFrame = dynamic_cast<FkAudioFrame *>(src);
    srcFrame->setPts(getPts());
    srcFrame->setFormat(getFormat());
    srcFrame->setSampleFormat(channels, sampleRate, sampleCount);
    srcFrame->setFlags(flags());
    memcpy(srcFrame->data(), data(), size());
}