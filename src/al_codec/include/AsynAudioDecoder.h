/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_ASYNAUDIODECODER_H
#define HARDWAREVIDEOCODEC_ASYNAUDIODECODER_H

#include "AbsVideoDecoder.h"
#include "DefaultAudioDecoder.h"
#include "AlEventPipeline.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "SimpleLock.h"
#include "PlayState.h"
#include <queue>
#include <atomic>

using namespace std;

class AsynAudioDecoder : public AbsAudioDecoder {
public:
    AsynAudioDecoder();

    virtual ~AsynAudioDecoder();

    virtual bool prepare(string path) override;

    void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    HwResult grab(HwAbsMediaFrame **frame) override;

    virtual void setOutSampleFormat(HwSampleFormat format) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override;

private:
    void loop();

    bool grab();

    void clear();

private:
    const int CACHE_SIZE = 5;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AbsAudioDecoder *decoder = nullptr;
//    HwAndroidAudioDecoder *decoder = nullptr;
    AlEventPipeline *pipeline = nullptr;
    queue<HwAbsMediaFrame *> cache;
    HwAbsMediaFrame *outputFrame = nullptr;//用于缓存一帧，以便在下次grab的时候进行回收
    atomic_bool playing;
    SimpleLock grabLock;
    SimpleLock releaseLock;
};


#endif //HARDWAREVIDEOCODEC_ASYNAUDIODECODER_H
