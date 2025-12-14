/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWASYNFFENCODER_H
#define HWVC_ANDROID_HWASYNFFENCODER_H

#include "FkAbsVideoEncoder.h"
#include "FkFrameAllocator.h"
#include "FkHandlerThread.h"
#include "FkHandler.h"
#include "FkSafeQueue.h"

FK_SUPER_CLASS(FkAsyncEncoder, FkAbsVideoEncoder) {
FK_DEF_CLASS_TYPE_FUNC(FkAsyncEncoder)

public:
    FkAsyncEncoder(const FkAbsEncoder::Desc &desc);

    virtual ~FkAsyncEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string profile) override;

    virtual void setPreset(std::string preset) override;

    virtual bool prepare(std::string path, int width, int height, FkSampleFormat audioFormat) override;

    virtual FkResult write(FkAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    void loop();

    void write();

    FkResult _dispatch(std::shared_ptr<FkMessage> &msg);

private:
    const int MAX_V_FRAME_CACHE = 32;
    FkAbsVideoEncoder *encoder = nullptr;
    FkFrameAllocator *hwFrameAllocator = nullptr;
    std::shared_ptr<FkHandlerThread> mThread = nullptr;
    std::shared_ptr<FkHandler> mHandler = nullptr;
    FkSafeQueue<FkAbsMediaFrame *> vQueue;
    FkSafeQueue<FkAbsMediaFrame *> aQueue;
    FkSafeQueue<bool> tQueue;
    std::mutex mtx;
    FkSimpleLock writeBlock;
    bool looping = false;
    std::atomic_bool released;
    bool isWithoutAudioTrack = false;
};


#endif //HWVC_ANDROID_HWASYNFFENCODER_H
