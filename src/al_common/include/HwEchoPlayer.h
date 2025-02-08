/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef HARDWAREVIDEOCODEC_EOCHER_H
#define HARDWAREVIDEOCODEC_EOCHER_H

#include "HwAudioPlayer.h"
#include "HwAudioRecorder.h"
#include "AlEventPipeline.h"
#include "SLEngine.h"

class HwEchoPlayer : public Object {
public:
    HwEchoPlayer(int channels, int sampleHz, int format, int samplesPerBuffer);

    ~HwEchoPlayer();

    void start();

    void stop();

private:
    int samplesPerBuffer = 0;
    int minBufferSize = 0;
    SLEngine *engine = nullptr;
    HwAudioPlayer *player = nullptr;
    HwAudioRecorder *recorder = nullptr;
    AlEventPipeline *pipeline = nullptr;
    uint8_t *buffer = nullptr;
    bool running = false;
    SimpleLock simpleLock;

    void loop();
};


#endif //HARDWAREVIDEOCODEC_EOCHER_H
