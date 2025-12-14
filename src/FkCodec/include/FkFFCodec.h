/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKFFCODEC_H
#define FK_CODEC_FKFFCODEC_H

#include "FkCodec.h"
#include "FkAudioTranslator.h"
#include "FkBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

FK_SUPER_CLASS(FkFFCodec, FkCodec) {
FK_DEF_CLASS_TYPE_FUNC(FkFFCodec)

public:
    FkFFCodec(FkCodec::kID id);

    virtual ~FkFFCodec();

    virtual FkResult configure(FkBundle &fmt) override;

    virtual FkResult start() override;

    /**
     * @param packet AvPacket
     */
    virtual FkResult process(FkAbsMediaFrame **frame, FkPacket **pkt) override;

    virtual void flush() override;

private:
    void release();

    bool configureVideo(AVCodecID id, AVCodec *codec);

    bool configureAudio(AVCodecID id, AVCodec *codec);

    void _configureBitrate(int32_t bitrate);

private:
    AVCodecContext *ctx = nullptr;
    AVFrame *avFrame = nullptr;
    AVPacket *avPacket = nullptr;
    FkPacket *mPacket = nullptr;
    /**
     * Just for audio codec.
     */
    FkAudioTranslator *translator = nullptr;
    std::shared_ptr<FkBuffer> mExtraData = nullptr;
    int64_t vLastPts = 0;
    bool reqExtraData = true;
};

#endif //FK_CODEC_FKFFCODEC_H
