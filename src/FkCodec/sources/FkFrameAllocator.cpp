/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkFrameAllocator.h"
#include "FkVideoFrame.h"
#include "FkAudioFrame.h"

#define TAG "FkFrameAllocator"

FK_IMPL_CLASS_TYPE(FkFrameAllocator, FkSourcesAllocator)

FkFrameAllocator::FkFrameAllocator() : FkSourcesAllocator() {

}

FkFrameAllocator::~FkFrameAllocator() {
    unRefLock.lock();
    std::set<FkAbsMediaFrame *>::iterator itr = unRefQueue.begin();
    while (itr != unRefQueue.end()) {
        FkAbsMediaFrame *frame = *itr;
        delete frame;
        ++itr;
    }
    unRefQueue.clear();
    unRefLock.unlock();
    refLock.lock();
    itr = refQueue.begin();
    while (itr != refQueue.end()) {
        FkAbsMediaFrame *frame = *itr;
        delete frame;
        ++itr;
    }
    refQueue.clear();
    refLock.unlock();
}

FkAbsMediaFrame *FkFrameAllocator::ref(AVFrame *avFrame) {
    if (!avFrame) return nullptr;
    if (avFrame->width * avFrame->height > 0) {
        return refVideo(avFrame);
    }
    return refAudio(avFrame);
}

bool FkFrameAllocator::recycle(FkSources **entity) {
    FkAbsMediaFrame *frame = dynamic_cast<FkAbsMediaFrame *>(entity[0]);
//    entity[0] = nullptr;
    refLock.lock();
    std::set<FkAbsMediaFrame *>::iterator itr = refQueue.find(frame);
    if (refQueue.end() == itr) {
        FkLogI(TAG, "HwSources(%p) recycle by allocator(%p) failed a", frame, this);
        refLock.unlock();
        return false;
    }
    refQueue.erase(itr);
    refLock.unlock();
    unRefLock.lock();
    unRefQueue.insert(*itr);
    unRefLock.unlock();
    return true;
}

bool FkFrameAllocator::isRef(FkAbsMediaFrame *frame) {
    return refQueue.end() != refQueue.find(frame);
}

FkAbsMediaFrame *FkFrameAllocator::refAudio(AVFrame *avFrame) {
    FkAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        auto avFrameByteSize = avFrame->nb_samples *
                               av_get_bytes_per_sample((AVSampleFormat) avFrame->format) *
                               avFrame->channels;
        std::set<FkAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isAudio()
                && (*itr)->size() == avFrameByteSize) {//帧类型相同，data大小相等，则可以复用
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (!frame) {
        frame = new FkAudioFrame(this,
                                 FkAbsMediaFrame::convertToAudioFrameFormat(
                                         static_cast<AVSampleFormat>(avFrame->format)),
                                 static_cast<uint16_t>(avFrame->channels),
                                 static_cast<uint32_t>(avFrame->sample_rate),
                                 static_cast<uint64_t>(avFrame->nb_samples));
    }
    memset(frame->data(), 0, frame->size());
    copyInfo(frame, avFrame);
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

FkAbsMediaFrame *FkFrameAllocator::ref(uint8_t *buf, int nbSample) {
    return nullptr;
}

FkAbsMediaFrame *FkFrameAllocator::refVideo(AVFrame *avFrame) {
    FkAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        int byteCount = FkAbsMediaFrame::getImageSize(
                FkAbsMediaFrame::convertToVideoFrameFormat(
                        static_cast<AVPixelFormat>(avFrame->format)),
                avFrame->width, avFrame->height);
        std::set<FkAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isVideo()
                && (*itr)->size() == byteCount) {//帧类型相同，data大小相等，则可以复用
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (!frame) {
        frame = new FkVideoFrame(this,
                                 FkAbsMediaFrame::convertToVideoFrameFormat(
                                         static_cast<AVPixelFormat>(avFrame->format)),
                                 static_cast<uint32_t>(avFrame->width),
                                 static_cast<uint32_t>(avFrame->height));
    }
    frame->setFormat(FkAbsMediaFrame::convertToVideoFrameFormat(
            static_cast<AVPixelFormat>(avFrame->format)));
    frame->setPts(avFrame->pts);
    av_image_copy_to_buffer(frame->data(), frame->size(), (const uint8_t *const *) avFrame->data,
                            avFrame->linesize, (AVPixelFormat) avFrame->format,
                            avFrame->width, avFrame->height, 1);
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

void FkFrameAllocator::copyInfo(FkAbsMediaFrame *dest, AVFrame *src) {
    memcpy(dest->data(), src->data[0], dest->size());
    dest->setPts(src->pts);
}

FkAbsMediaFrame *FkFrameAllocator::ref(FkAbsMediaFrame *src) {
    FkAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        std::set<FkAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->getFormat() == src->getFormat()
                && (*itr)->size() == src->size()) {
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (frame) {
        src->clone(frame);
    } else {
        if (src->isVideo()) {
            FkVideoFrame *videoFrame = static_cast<FkVideoFrame *>(src);

            frame = new FkVideoFrame(this, videoFrame->getFormat(),
                                     videoFrame->getWidth(),
                                     videoFrame->getHeight());
            videoFrame->clone(frame);
            FkLogI(TAG, "HwSources(%p) alloc by allocator(%p)", frame, this);
        } else if (src->isAudio()) {
            FkAudioFrame *audioFrame = static_cast<FkAudioFrame *>(src);

            frame = new FkAudioFrame(this, audioFrame->getFormat(), audioFrame->getChannels(),
                                     audioFrame->getSampleRate(), audioFrame->getSampleCount());
            audioFrame->clone(frame);
        }
    }
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

void FkFrameAllocator::printInfo() {
    FkLogI(TAG, "FkFrameAllocator(%p)::info: ref=%d, unRef=%d",
           this,
           refQueue.size(),
           unRefQueue.size());
}