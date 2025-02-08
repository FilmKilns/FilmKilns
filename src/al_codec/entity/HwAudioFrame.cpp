/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAudioFrame.h"
#include "Logcat.h"

HwAudioFrame::HwAudioFrame(HwFrameFormat format,
                           uint16_t channels,
                           uint32_t sampleRate,
                           uint64_t sampleCount)
        : HwAudioFrame(nullptr, format, channels, sampleRate, sampleCount) {
}

HwAudioFrame::HwAudioFrame(HwSourcesAllocator *allocator,
                           HwFrameFormat format,
                           uint16_t channels,
                           uint32_t sampleRate,
                           uint64_t sampleCount)
        : HwAbsMediaFrame(allocator, format, sampleCount * channels * getBytesPerSample(format)) {
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->sampleCount = sampleCount;
}

HwAudioFrame::~HwAudioFrame() {
    channels = 0;
    sampleRate = 0;
    sampleCount = 0;
}

uint16_t HwAudioFrame::getChannels() { return channels; }

uint32_t HwAudioFrame::getSampleRate() { return sampleRate; }

uint64_t HwAudioFrame::getSampleCount() { return sampleCount; }

void HwAudioFrame::setSampleFormat(uint16_t channels, uint32_t sampleRate, uint64_t sampleCount) {
    this->channels = channels;
    this->sampleRate = sampleRate;
    this->sampleCount = sampleCount;
}

uint64_t HwAudioFrame::duration() {
    return 1000000 * sampleCount / sampleRate;
}

HwAbsMediaFrame *HwAudioFrame::clone() {
    HwAudioFrame *destFrame = new HwAudioFrame(nullptr, getFormat(),
                                               channels, sampleRate,
                                               sampleCount);
    destFrame->setPts(getPts());
    destFrame->setFormat(getFormat());
    memcpy(destFrame->data(), data(), destFrame->size());
    return destFrame;
}

void HwAudioFrame::clone(HwAbsMediaFrame *src) {
    if (!src || !src->isAudio() || src->size() < size()) {
        Logcat::e("HWVC", "Invalid audio frame");
        return;
    }
    HwAudioFrame *srcFrame = dynamic_cast<HwAudioFrame *>(src);
    srcFrame->setPts(getPts());
    srcFrame->setFormat(getFormat());
    srcFrame->setSampleFormat(channels, sampleRate, sampleCount);
    srcFrame->setFlags(flags());
    memcpy(srcFrame->data(), data(), size());
}