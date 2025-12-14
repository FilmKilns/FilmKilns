/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFFCodec.h"
#include "FkAudioFrame.h"
#include "FkVideoFrame.h"
#include "FkFFUtils.h"

#define TAG "FkFFCodec"

FK_IMPL_CLASS_TYPE(FkFFCodec, FkCodec)

FkFFCodec::FkFFCodec(FkCodec::kID id) : FkCodec(id) {

}

FkFFCodec::~FkFFCodec() {
    release();
}

void FkFFCodec::release() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (mPacket) {
        delete mPacket;
        mPacket = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (ctx) {
        if (avcodec_is_open(ctx)) {
            avcodec_close(ctx);
        }
        avcodec_free_context(&ctx);
    }
    if (translator) {
        delete translator;
        translator = nullptr;
    }
    mExtraData = nullptr;
}

FkResult FkFFCodec::configure(FkBundle &fmt) {
    FkCodec::configure(fmt);
    if (getCodecID() == FkCodec::kID::NONE) {
        return FK_INVALID_PARAMETERS;
    }
    FkFFUtils::init();
    AVCodecID id = static_cast<AVCodecID>(getCodecID());
    AVCodec *pCodec = avcodec_find_encoder(id);
    if (!pCodec) {
        FkLogE(TAG, "could not find %d codec!", id);
        release();
        return FK_FAIL;
    }
    ctx = avcodec_alloc_context3(pCodec);
    if (!ctx) {
        FkLogE(TAG, "could not init %d codec context!", id);
        release();
        return FK_FAIL;
    }
    switch (id) {
        case AV_CODEC_ID_H264: {
            if (!configureVideo(id, pCodec)) {
                FkLogE(TAG, "could not open %d codec!", id);
                return FK_FAIL;
            }
            break;
        }
        case AV_CODEC_ID_GIF: {
            // Configure
            ctx->codec_id = id;
            ctx->codec_type = AVMEDIA_TYPE_VIDEO;
            ctx->pix_fmt = AV_PIX_FMT_PAL8;
            ctx->width = getFormat()->get(KEY_WIDTH, INT32_MIN);
            ctx->height = getFormat()->get(KEY_HEIGHT, INT32_MIN);
            ctx->time_base = {1, getFormat()->get(KEY_FPS, INT32_MIN)};
            ctx->framerate = {getFormat()->get(KEY_FPS, INT32_MIN), 1};
            _configureBitrate(getFormat()->get(KEY_BIT_RATE, INT32_MIN));

            ctx->gop_size = 15;
            ctx->thread_count = 0;
            ctx->max_b_frames = 2;
            ctx->codec = pCodec;
            ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            int len = ctx->extradata_size;
            int ret = avcodec_open2(ctx, pCodec, nullptr);
            if (ret < 0) {
                FkLogE(TAG, "could not open %d codec!", ctx->codec_id);
                release();
                return FK_FAIL;
            }
            break;
        }
        case AV_CODEC_ID_AAC_LATM:
        case AV_CODEC_ID_AAC: {
            if (!configureAudio(id, pCodec)) {
                FkLogE(TAG, "could not open %d codec!", id);
                return FK_FAIL;
            }
            break;
        }
        default:
            return FK_FAIL;
    }
    if (ctx->extradata && ctx->extradata_size > 0) {
        mExtraData = FkBuffer::alloc(ctx->extradata_size);
        mExtraData->put(ctx->extradata, ctx->extradata_size);
        mExtraData->rewind();
        getFormat()->put(KEY_EXTRA_DATA, mExtraData);
    }
    //Copy parameters.
    getFormat()->put(KEY_PROFILE, ctx->profile);
    getFormat()->put(KEY_LEVEL, ctx->level);
    getFormat()->put(KEY_BIT_RATE, static_cast<int32_t>(ctx->bit_rate));
    getFormat()->put(KEY_FRAME_SIZE, ctx->frame_size);
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            getFormat()->put(KEY_MEDIA_TYPE, (int32_t) kMediaType::VIDEO);
            break;
        }
        case AVMEDIA_TYPE_AUDIO: {
            getFormat()->put(KEY_MEDIA_TYPE, (int32_t) kMediaType::AUDIO);
            break;
        }
        default: {
            getFormat()->put(KEY_MEDIA_TYPE, (int32_t) kMediaType::NONE);
            break;
        }
    }
    reqExtraData = true;
    avFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    if (ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        const int32_t f = fmt.get(KEY_FORMAT, INT32_MIN);
        translator = new FkAudioTranslator(FkSampleFormat(kFrameFormat::SAMPLE_FLTP,
                                                          ctx->channels,
                                                          ctx->sample_rate),
                                           FkSampleFormat((kFrameFormat) f,
                                                          ctx->channels,
                                                          ctx->sample_rate));
    }
    return FK_OK;
}

FkResult FkFFCodec::start() {
    return FK_OK;
}

bool FkFFCodec::configureVideo(AVCodecID id, AVCodec *codec) {
    // Configure
    ctx->codec_id = id;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = FkAbsMediaFrame::convertVideoFrameFormat(
            (kFrameFormat) getFormat()->get(KEY_FORMAT, INT32_MIN));
    ctx->width = getFormat()->get(KEY_WIDTH, INT32_MIN);
    ctx->height = getFormat()->get(KEY_HEIGHT, INT32_MIN);
    ctx->time_base = {1, getFormat()->get(KEY_FPS, INT32_MIN)};
    ctx->framerate = {getFormat()->get(KEY_FPS, INT32_MIN), 1};
    _configureBitrate(getFormat()->get(KEY_BIT_RATE, INT32_MIN));

    ctx->gop_size = 15;
//    pCodecCtx->ticks_per_frame = 2;
    ctx->thread_count = 0;
//    ctx->qmin = 10;
//    ctx->qmax = 30;
    ctx->max_b_frames = 2;
//    ctx->me_range = 16;
//    ctx->max_qdiff = 4;
    ctx->codec = codec;
    /**
     * Important. If not set this, the output will be fail.
     */
//    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
    ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == ctx->codec_id) {
        ctx->profile = FF_PROFILE_H264_HIGH;
        //0 - 51
//        av_dict_set_int(&param, "crf", getFormat()->getInt32(KEY_QUALITY), 0);  // or abr,qp
        //ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow and placebo.
        av_dict_set(&param, "preset", getFormat()->get(FkCodec::KEY_PRESET, "").c_str(), 0);
        av_dict_set(&param, "profile", getFormat()->get(FkCodec::KEY_PROFILE, "").c_str(), 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
    }
    int ret = avcodec_open2(ctx, codec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        FkLogE(TAG, "could not open %d codec, ret=%s!", ctx->codec_id, av_err2str(ret));
        release();
        return false;
    }
    return true;
}

bool FkFFCodec::configureAudio(AVCodecID id, AVCodec *codec) {
    ctx->codec_id = id;
    ctx->codec_type = AVMEDIA_TYPE_AUDIO;
    ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    ctx->bit_rate = getFormat()->get(KEY_BIT_RATE, INT32_MIN);
    ctx->sample_rate = getFormat()->get(KEY_SAMPLE_RATE, INT32_MIN);
    ctx->channels = getFormat()->get(KEY_CHANNELS, INT32_MIN);
    ctx->channel_layout = static_cast<uint64_t>(
            av_get_default_channel_layout(ctx->channels));
    ctx->time_base = {1, ctx->sample_rate};
    ctx->codec = codec;
//    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
    ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }

    AVDictionary *param = nullptr;
    int ret = avcodec_open2(ctx, codec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        if (ctx) {
            avcodec_close(ctx);
            avcodec_free_context(&ctx);
        }
        FkLogE(TAG, "could not open audio codec: %s", strerror(AVUNERROR(ret)));
        return false;
    }
    return true;
}

void FkFFCodec::_configureBitrate(int32_t bitrate) {
    ctx->bit_rate = bitrate;
}

void FkFFCodec::flush() {

}

FkResult FkFFCodec::process(FkAbsMediaFrame **frame, FkPacket **pkt) {
    *pkt = nullptr;
    int64_t duration = 1;
    switch (ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: {
            FkVideoFrame *videoFrame = static_cast<FkVideoFrame *>(*frame);
            int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
            avFrame->data[0] = videoFrame->data();
            avFrame->data[1] = videoFrame->data() + pixelCount;
            avFrame->data[2] = videoFrame->data() + pixelCount * 5 / 4;

            avFrame->linesize[0] = videoFrame->getWidth();
            avFrame->linesize[1] = videoFrame->getWidth() / 2;
            avFrame->linesize[2] = videoFrame->getWidth() / 2;
            avFrame->width = videoFrame->getWidth();
            avFrame->height = videoFrame->getHeight();
            avFrame->format = FkAbsMediaFrame::convertVideoFrameFormat(videoFrame->getFormat());
            avFrame->pts = videoFrame->getPts();
            if (FkVideoFrame::HW_PIC_I == videoFrame->getPicType()) {
                avFrame->pict_type = AV_PICTURE_TYPE_I;
            } else {
                avFrame->pict_type = AV_PICTURE_TYPE_NONE;
            }
            duration = avFrame->pts - vLastPts;
            if (duration <= 0) {
                duration = static_cast<int64_t>(AV_TIME_BASE /
                                                (float) ctx->time_base.den );
            }
            vLastPts = avFrame->pts;
            int ret = avcodec_send_frame(ctx, avFrame);
            break;
        }
        case AVMEDIA_TYPE_AUDIO: {
            FkAudioFrame *audioFrame = static_cast<FkAudioFrame *>(*frame);
            int32_t frameSize = ctx->frame_size;
            if (ctx->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
                frameSize = 10000;
            }
            avFrame->data[0] = audioFrame->data();
            avFrame->linesize[0] = audioFrame->size();
            avFrame->format = FkAbsMediaFrame::convertAudioFrameFormat(audioFrame->getFormat());
            avFrame->sample_rate = audioFrame->getSampleRate();
            avFrame->nb_samples = frameSize;
            avFrame->channels = audioFrame->getChannels();
            avFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                    audioFrame->getChannels()));
            avFrame->pts = audioFrame->getPts();

            AVFrame *f = nullptr;
            translator->translate(&f, &avFrame);
            duration = static_cast<int64_t>(AV_TIME_BASE / (float) ctx->sample_rate *
                                            avFrame->nb_samples);
            int ret = avcodec_send_frame(ctx, f);
            break;
        }
        default:
            return FK_FAIL;
    }
    av_packet_unref(avPacket);
    if(reqExtraData) {
        if (mPacket) {
            delete mPacket;
        }
        mPacket = FkPacket::wrap(ctx->extradata, ctx->extradata_size, 0, 0, FK_CTL_FLAG_CONFIG);
        *pkt = mPacket;
        reqExtraData = false;
        return FK_OK;
    }
    int ret = avcodec_receive_packet(ctx, avPacket);
    if (AVERROR(EAGAIN) == ret) {
        FkLogE(TAG, "wait(%s)", strerror(AVUNERROR(ret)));
        return FK_FAIL;
    }
    if (ret < 0) {
        FkLogE(TAG, "failed: %s", strerror(AVUNERROR(ret)));
        return FK_FAIL;
    }
    avPacket->duration = duration;
    if (mPacket) {
        delete mPacket;
    }
    mPacket = FkPacket::wrap(avPacket);
    *pkt = mPacket;
    return FK_OK;
}
