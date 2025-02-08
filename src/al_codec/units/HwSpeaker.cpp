/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <libavutil/samplefmt.h>
#include "../include/HwSpeaker.h"
#include "AlLogcat.h"

#define TAG "HwSpeaker"

HwSpeaker::HwSpeaker(string alias) : HwSpeaker(alias, HwAudioDeviceMode::Normal) {
}

HwSpeaker::HwSpeaker(string alias, HwAudioDeviceMode mode) : Unit(alias), mode(mode) {
    al_reg_msg(EVENT_SPEAKER_FEED, HwSpeaker::eventFeed);
}

HwSpeaker::~HwSpeaker() {
    LOGI("HwSpeaker::~HwSpeaker");
    if (player) {
        player->stop();
        delete player;
        player = nullptr;
    }
}

bool HwSpeaker::onCreate(AlMessage *msg) {
    return false;
}

bool HwSpeaker::onDestroy(AlMessage *msg) {
    return false;
}

bool HwSpeaker::eventFeed(AlMessage *msg) {
    if (msg->obj) {
        HwAudioFrame *frame = dynamic_cast<HwAudioFrame *>(msg->obj);
        createFromAudioFrame(frame);
        if (player) {
//            Logcat::i("HWVC", "HwSpeaker::play audio: %d, %d, %lld, %lld",
//                      frame->getChannels(),
//                      frame->getSampleRate(),
//                      frame->getSampleCount(),
//                      frame->getDataSize());
            player->write(frame->data(), frame->size(), 1000000);
            auto *msg1 = AlMessage::obtain(MSG_SPEAKER_FEED_DONE);
            msg1->arg1 = frame->getSampleCount();
            postMessage(msg1);
        }
    }
    return false;
}

void HwSpeaker::createFromAudioFrame(HwAudioFrame *frame) {
    if (player) {
        return;
    }
    int format;
    switch (frame->getFormat()) {
        case HwFrameFormat::HW_SAMPLE_U8:
            format = SL_PCMSAMPLEFORMAT_FIXED_8;
            break;
        case HwFrameFormat::HW_SAMPLE_S16:
            format = SL_PCMSAMPLEFORMAT_FIXED_16;
            break;
        default:
            format = SL_PCMSAMPLEFORMAT_FIXED_32;
    }
    player = new HwAudioPlayer(mode,
                               frame->getChannels(),
                               frame->getSampleRate(),
                               static_cast<uint16_t>(format),
                               static_cast<uint32_t>(frame->getSampleCount()));
    player->start();
}