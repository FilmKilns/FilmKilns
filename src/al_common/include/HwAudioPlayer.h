/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_AUDIOPLAYER_H
#define HARDWAREVIDEOCODEC_AUDIOPLAYER_H

#include <string>
#include "AudioDevice.h"
#include "ObjectBox.h"
#include "RecyclerBlockQueue.h"
#include "EventPipeline.h"
#include "SimpleLock.h"
#include "SLEngine.h"
#include "HwResult.h"
#include <queue>
#include "HwFIFOBuffer.h"

using namespace std;

class HwAudioPlayer : public SLAudioDevice {
public:
    HwAudioPlayer(uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    HwAudioPlayer(HwAudioDeviceMode mode,
                  uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    HwAudioPlayer(SLEngine *engine,
                  HwAudioDeviceMode mode,
                  uint16_t channels,
                  uint32_t sampleRate,
                  uint16_t format,
                  uint32_t samplesPerBuffer);

    virtual ~HwAudioPlayer();

    virtual HwResult start();

    virtual void stop();

    virtual HwResult write(uint8_t *buffer, size_t size);

    virtual HwResult write(uint8_t *buffer, size_t size, int timeOut);

    virtual void flush();

    void bufferEnqueue(SLAndroidSimpleBufferQueueItf slBufferQueueItf);

    size_t getCacheSamples();

private:
    SimpleLock lock;
    std::queue<ObjectBox *> pcmList;
    HwFIFOBuffer *fifo = nullptr;
    SLEngine *engine = nullptr;
    bool ownEngine = false;
    SLObjectItf mixObject = nullptr;
    SLObjectItf playObject = nullptr;
    SLPlayItf playItf = nullptr;
    SLAndroidSimpleBufferQueueItf bufferQueueItf = nullptr;
    uint8_t *silenceData = nullptr;

    HwResult createEngine();

    void destroyEngine();

    HwResult createBufferQueueAudioPlayer();

    void initialize(SLEngine *engine);
};


#endif //HARDWAREVIDEOCODEC_AUDIOPLAYER_H
