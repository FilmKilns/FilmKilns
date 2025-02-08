/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_HWSPEAKER_H
#define HARDWAREVIDEOCODEC_HWSPEAKER_H

#include "Unit.h"
#include "HwAudioPlayer.h"
#include "HwAudioFrame.h"

class HwSpeaker : public Unit {
public:
    HwSpeaker(string alias);

    HwSpeaker(string alias, HwAudioDeviceMode mode);

    virtual ~HwSpeaker();

    bool onDestroy(AlMessage *msg) override;

    bool onCreate(AlMessage *msg) override;

    bool eventFeed(AlMessage *msg);

private:
    void createFromAudioFrame(HwAudioFrame *frame);

private:
    HwAudioDeviceMode mode = HwAudioDeviceMode::Normal;
    HwAudioPlayer *player = nullptr;
};


#endif //HARDWAREVIDEOCODEC_HWSPEAKER_H
