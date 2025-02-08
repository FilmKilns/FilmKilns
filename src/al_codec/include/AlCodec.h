/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCODEC_H
#define HWVC_ANDROID_ALCODEC_H

#include "Object.h"
#include "HwResult.h"
#include "AlBundle.h"
#include "HwAbsMediaFrame.h"
#include "HwPacket.h"
#include "AlBuffer.h"

/// +------------------------------+
/// |    Annex-B   |  AVCC/HVCC    |
/// +------------------------------+
/// | 00 00 00 01  |  NALU LENGTH  |
/// +------------------------------+
class AlCodec : public Object {
public:
    AL_ENUM kID : int {
        NONE = AV_CODEC_ID_NONE,
        H264 = AV_CODEC_ID_H264,
        AAC = AV_CODEC_ID_AAC,
        GIF = AV_CODEC_ID_GIF
    };

    /// Same with AVMEDIA_TYPE_AUDIO/AVMEDIA_TYPE_VIDEO
    AL_ENUM kMediaType : int {
        UNKNOWN = -1,
        VIDEO,
        AUDIO,
    };

public:
    AlCodec(AlCodec::kID id);

    virtual  ~AlCodec();

    virtual HwResult configure(AlBundle &format);

    virtual HwResult start() = 0;

    virtual AlCodec::kID getCodecID();

    virtual AlBundle &getFormat();

    virtual AlCodec::kMediaType getMediaType();

    /**
     * For encoder. HwAbsMediaFrame in & AVPacket out.
     * For decoder. AVPacket in & HwAbsMediaFrame out.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) = 0;

    virtual void flush() = 0;

private:
    AlCodec::kID id;
    std::shared_ptr<AlBundle> format = nullptr;

public:
    static const string KEY_MIME;
    static const string KEY_CODEC_ID;
    static const string KEY_MEDIA_TYPE;
    static const string KEY_FORMAT;
    static const string KEY_PROFILE;
    static const string KEY_PRESET;
    static const string KEY_LEVEL;
    static const string KEY_BIT_RATE;
    static const string KEY_DURATION;
    // Video
    static const string KEY_WIDTH;
    static const string KEY_HEIGHT;
    static const string KEY_FPS;
    static const string KEY_QUALITY;
    //Audio
    static const string KEY_SAMPLE_RATE;
    static const string KEY_CHANNELS;
    static const string KEY_FRAME_SIZE;

    static const string KEY_CSD_0;
    static const string KEY_CSD_1;
    static const string KEY_CSD_2;
    static const string KEY_CSD_3;
    static const string KEY_EXTRA_DATA;
};


#endif //HWVC_ANDROID_ALCODEC_H
