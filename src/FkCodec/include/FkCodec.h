/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKCODEC_H
#define FK_CODEC_FKCODEC_H

#include "FkObject.h"
#include "FkBundle.h"
#include "FkAbsMediaFrame.h"
#include "FkPacket.h"
#include "FkBuffer.h"

/// +------------------------------+
/// |    Annex-B   |  AVCC/HVCC    |
/// +------------------------------+
/// | 00 00 00 01  |  NALU LENGTH  |
/// +------------------------------+
FK_SUPER_CLASS(FkCodec, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkCodec)
public:
    FK_ENUM kID : int32_t {
            NONE = AV_CODEC_ID_NONE,
            H264 = AV_CODEC_ID_H264,
            AAC = AV_CODEC_ID_AAC,
            GIF = AV_CODEC_ID_GIF
    };

public:
    FkCodec(FkCodec::kID id);

    virtual  ~FkCodec();

    virtual FkResult configure(FkBundle &fmt);

    virtual FkResult start() = 0;

    virtual FkCodec::kID getCodecID();

    virtual std::shared_ptr<FkBundle> getFormat();

    virtual kMediaType getMediaType();

    /**
     * For encoder. HwAbsMediaFrame in & AVPacket out.
     * For decoder. AVPacket in & HwAbsMediaFrame out.
     */
    virtual FkResult process(FkAbsMediaFrame **frame, FkPacket **pkt) = 0;

    virtual void flush() = 0;

private:
    FkCodec::kID id;
    std::shared_ptr<FkBundle> format = nullptr;

public:
    static const std::string KEY_MIME;
    static const std::string KEY_CODEC_ID;
    static const std::string KEY_MEDIA_TYPE;
    static const std::string KEY_FORMAT;
    static const std::string KEY_PROFILE;
    static const std::string KEY_PRESET;
    static const std::string KEY_LEVEL;
    static const std::string KEY_BIT_RATE;
    static const std::string KEY_DURATION;
    // Video
    static const std::string KEY_WIDTH;
    static const std::string KEY_HEIGHT;
    static const std::string KEY_FPS;
    static const std::string KEY_QUALITY;
    static const std::string KEY_ROTATION;
    //Audio
    static const std::string KEY_SAMPLE_RATE;
    static const std::string KEY_CHANNELS;
    static const std::string KEY_FRAME_SIZE;

    static const std::string KEY_CSD_0;
    static const std::string KEY_CSD_1;
    static const std::string KEY_CSD_2;
    static const std::string KEY_CSD_3;
    static const std::string KEY_EXTRA_DATA;
};


#endif //FK_CODEC_FKCODEC_H
