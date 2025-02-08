/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWASYNFFENCODER_H
#define HWVC_ANDROID_HWASYNFFENCODER_H

#include "AlAbsVideoEncoder.h"
#include "AlEventPipeline.h"
#include "HwFrameAllocator.h"
#include "AlSafeQueue.h"

class AlAsyncEncoder : public AlAbsVideoEncoder {
public:
    AlAsyncEncoder(const AlAbsEncoder::Desc &desc);

    virtual ~AlAsyncEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string profile) override;

    virtual void setPreset(std::string preset) override;

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    void loop();

    void write();

private:
    const int MAX_V_FRAME_CACHE = 32;
    AlAbsVideoEncoder *encoder = nullptr;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AlEventPipeline *pipeline = nullptr;
    AlSafeQueue<HwAbsMediaFrame *> vQueue;
    AlSafeQueue<HwAbsMediaFrame *> aQueue;
    AlSafeQueue<bool> tQueue;
    std::mutex mtx;
    SimpleLock writeBlock;
    bool looping = false;
    std::atomic_bool released;
};


#endif //HWVC_ANDROID_HWASYNFFENCODER_H
