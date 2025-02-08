/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCodec.h"

const string AlCodec::KEY_MIME = "media-mime";
const string AlCodec::KEY_CODEC_ID = "media-codec-id";
const string AlCodec::KEY_MEDIA_TYPE = "media-type";
const string AlCodec::KEY_FORMAT = "media-format";
const string AlCodec::KEY_PROFILE = "media-profile";
const string AlCodec::KEY_PRESET = "media-preset";
const string AlCodec::KEY_LEVEL = "media-level";
const string AlCodec::KEY_BIT_RATE = "media-bit-rate";
const string AlCodec::KEY_DURATION = "media-duration";
const string AlCodec::KEY_WIDTH = "video-width";
const string AlCodec::KEY_HEIGHT = "video-height";
const string AlCodec::KEY_FPS = "video-fps";
const string AlCodec::KEY_QUALITY = "video-quality";
const string AlCodec::KEY_SAMPLE_RATE = "audio-sample-rate";
const string AlCodec::KEY_CHANNELS = "audio-channels";
const string AlCodec::KEY_FRAME_SIZE = "audio-frame-size";
const string AlCodec::KEY_CSD_0 = "csd-0";
const string AlCodec::KEY_CSD_1 = "csd-1";
const string AlCodec::KEY_CSD_2 = "csd-2";
const string AlCodec::KEY_CSD_3 = "csd-3";
const string AlCodec::KEY_EXTRA_DATA = "extra_data";

AlCodec::AlCodec(AlCodec::kID id) : Object(), id(id) {

}

AlCodec::~AlCodec() {
}

HwResult AlCodec::configure(AlBundle &format) {
    this->format = std::make_shared<AlBundle>(format);
    this->format->put(KEY_CODEC_ID, (int32_t) getCodecID());
    this->format->put(KEY_MEDIA_TYPE, (int32_t) getMediaType());
    return Hw::SUCCESS;
}

AlCodec::kID AlCodec::getCodecID() { return id; }

AlBundle &AlCodec::getFormat() { return *format; }

AlCodec::kMediaType AlCodec::getMediaType() {
    switch (getCodecID()) {
        case AlCodec::kID::H264: {
            return AlCodec::kMediaType::VIDEO;
        }
        case AlCodec::kID::AAC: {
            return AlCodec::kMediaType::AUDIO;
        }
        default: {
            return AlCodec::kMediaType::UNKNOWN;
        }
    }
}

