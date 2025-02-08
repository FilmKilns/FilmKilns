/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAudioCompiler.h"
#include "HwBuffer.h"

HwAudioCompiler::HwAudioCompiler(string alias) : Unit(alias) {
    registerEvent(EVENT_MICROPHONE_OUT_SAMPLES,
                  reinterpret_cast<EventFunc>(&HwAudioCompiler::eventReceiveData));
}

HwAudioCompiler::~HwAudioCompiler() {

}

bool HwAudioCompiler::onDestroy(AlMessage *msg) {
    if (muxer) {
        delete muxer;
        muxer = nullptr;
    }
    return true;
}

bool HwAudioCompiler::onCreate(AlMessage *msg) {
    Object *f = getObject("format");
    HwSampleFormat *format = reinterpret_cast<HwSampleFormat *>(f);
    string path = getString("path");
    muxer = WAVRawMuxer::build(path, *format);
    return true;
}

bool HwAudioCompiler::eventReceiveData(AlMessage *msg) {
    HwBuffer *buf = dynamic_cast<HwBuffer *>(msg->obj);
    if (buf && muxer) {
        muxer->write(buf);
    }
    return true;
}