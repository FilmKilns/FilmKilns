/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCodec.h"

const std::string FkCodec::KEY_MIME = "media-mime";
const std::string FkCodec::KEY_CODEC_ID = "media-codec-id";
const std::string FkCodec::KEY_MEDIA_TYPE = "media-type";
const std::string FkCodec::KEY_FORMAT = "media-format";
const std::string FkCodec::KEY_PROFILE = "media-profile";
const std::string FkCodec::KEY_PRESET = "media-preset";
const std::string FkCodec::KEY_LEVEL = "media-level";
const std::string FkCodec::KEY_BIT_RATE = "media-bit-rate";
const std::string FkCodec::KEY_DURATION = "media-duration";
const std::string FkCodec::KEY_WIDTH = "video-width";
const std::string FkCodec::KEY_HEIGHT = "video-height";
const std::string FkCodec::KEY_FPS = "video-fps";
const std::string FkCodec::KEY_QUALITY = "video-quality";
const std::string FkCodec::KEY_ROTATION = "video-rotation";
const std::string FkCodec::KEY_SAMPLE_RATE = "audio-sample-rate";
const std::string FkCodec::KEY_CHANNELS = "audio-channels";
const std::string FkCodec::KEY_FRAME_SIZE = "audio-frame-size";
const std::string FkCodec::KEY_CSD_0 = "csd-0";
const std::string FkCodec::KEY_CSD_1 = "csd-1";
const std::string FkCodec::KEY_CSD_2 = "csd-2";
const std::string FkCodec::KEY_CSD_3 = "csd-3";
const std::string FkCodec::KEY_EXTRA_DATA = "extra_data";

FK_IMPL_CLASS_TYPE(FkCodec, FkObject)

FkCodec::FkCodec(FkCodec::kID id) : FkObject(), id(id) {

}

FkCodec::~FkCodec() {
}

FkResult FkCodec::configure(FkBundle &fmt) {
    this->format = std::make_shared<FkBundle>(fmt);
    this->format->put(KEY_CODEC_ID, (int32_t) getCodecID());
    this->format->put(KEY_MEDIA_TYPE, (int32_t) getMediaType());
    return FK_OK;
}

FkCodec::kID FkCodec::getCodecID() { return id; }

std::shared_ptr<FkBundle> FkCodec::getFormat() { return format; }

kMediaType FkCodec::getMediaType() {
    switch (getCodecID()) {
        case FkCodec::kID::H264: {
            return kMediaType::VIDEO;
        }
        case FkCodec::kID::AAC: {
            return kMediaType::AUDIO;
        }
        default: {
            return kMediaType::NONE;
        }
    }
}

