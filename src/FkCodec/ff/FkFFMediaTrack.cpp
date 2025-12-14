//
// Created by lmy on 2023/9/27.
//

#include "FkFFMediaTrack.h"
#include "FkFFramePacket.h"
#include "FkFFPacket.h"

#define TAG "FkFFMediaTrack"

FK_IMPL_CLASS_TYPE(FkFFMediaTrack, FkObject)

FkFFMediaTrack::FkFFMediaTrack(const std::shared_ptr<FkFFMediaContext> &context, int trackId)
        : FkMediaTrack(context, trackId) {
    this->context = context;
    this->trackId = trackId;

    AVCodecParameters *avCodecParameters = context->_getCodecParameters(trackId);
    AVCodec *codec = NULL;
    if (AV_CODEC_ID_H264 == avCodecParameters->codec_id) {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
//        codec = avcodec_find_decoder_by_name("h264_mediacodec");
//        if (NULL == codec) {
//            LOGE("Selected AV_CODEC_ID_H264.");
//            codec = avcodec_find_decoder(avCodecParameters->codec_id);
//        }
    } else {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
    }
    if (NULL == codec) {
        FkLogE(TAG, "Couldn't find codec.");
        return;
    }
    //打开编码器
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, avCodecParameters);
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        FkLogE(TAG, "Couldn't open codec.");
        _destroy();
        return;
    }
    std::string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    avFrame = av_frame_alloc();
    FkLogI(TAG, "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
              avCodecParameters->format, avCodecParameters->frame_size);
}

FkFFMediaTrack::~FkFFMediaTrack() {
    _destroy();
}

void FkFFMediaTrack::_destroy() {
    if (codecContext) {
        avcodec_close(codecContext);
        codecContext = nullptr;
    }
}

std::shared_ptr<FkAbsPacket> FkFFMediaTrack::grab(const std::shared_ptr<FkAbsPacket> &input) {
    auto mediaContext = context.lock();
    if (mediaContext == nullptr || input == nullptr) {
        return nullptr;
    }
    if (!FK_INSTANCE_OF(input, FkFFPacket)) {
        return nullptr;
    }
    auto pkt = std::dynamic_pointer_cast<FkFFPacket>(input);
    AVPacket *avPacket = static_cast<AVPacket *>(pkt->getOrigin());
    avcodec_send_packet(codecContext, avPacket);
    auto ret = avcodec_receive_frame(codecContext, avFrame);
    if (0 == ret) {
        auto timebase = mediaContext->ctx->streams[trackId]->time_base;
        std::shared_ptr<FkAbsPacket> result = FkFFramePacket::make(avFrame,
                                                                   pkt->getType(),
                                                                   timebase);
        return result;
    }
    return nullptr;
}
