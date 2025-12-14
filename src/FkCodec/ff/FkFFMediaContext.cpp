//
// Created by lmy on 2023/9/27.
//

#include "FkFFMediaContext.h"
#include "FkFFUtils.h"
#include "FkPacket2.h"
#include "FkFFPacket.h"

#define TAG "FkFFMediaContext"

FK_IMPL_CLASS_TYPE(FkFFMediaContext, FkObject)

FkFFMediaContext::FkFFMediaContext(const std::string &path) : FkObject(), path(path) {
    ctx = avformat_alloc_context();
    if (avformat_open_input(&ctx, path.c_str(), NULL, NULL) != 0) {
        FkLogE(TAG, "Couldn't open input stream.");
        _destroy();
    }
    if (avformat_find_stream_info(ctx, NULL) < 0) {
        FkLogW(TAG, "Couldn't find stream information.");
    }
    avPacket = av_packet_alloc();
}

FkFFMediaContext::~FkFFMediaContext() {
    _destroy();
    if (bsfContext) {
        av_bsf_free(&bsfContext);
        bsfContext = nullptr;
    }
}

void FkFFMediaContext::_destroy() {
    if (ctx) {
        avformat_close_input(&ctx);
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (avPacket) {
        av_packet_unref(avPacket);
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
}

std::vector<FkMediaTrack::Desc> FkFFMediaContext::getTracks() {
    std::vector<FkMediaTrack::Desc> vec;
    if (ctx == nullptr) {
        return vec;
    }
    for (int i = 0; i < ctx->nb_streams; i++) {
        auto type = ctx->streams[i]->codecpar->codec_type;
        auto duration = ctx->streams[i]->duration;
        auto avTimeBase = ctx->streams[i]->time_base;
        FkMediaTrack::Desc desc{i, FkFFUtils::makeMediaType(type),
                                {duration, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}}};
        vec.emplace_back(desc);
    }
    return vec;
}

std::shared_ptr<FkMediaTrack> FkFFMediaContext::openTrack(int trackId) {
    return std::make_shared<FkFFMediaTrack>(shared_from_this(), trackId);
}

FkMediaTrack::Desc FkFFMediaContext::_getVideoTrack() {
    for (int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO) {
            return {i, FkFFUtils::makeMediaType(ctx->streams[i]->codecpar->codec_type)};
        }
    }
    return {-1, kMediaType::NONE};
}

AVCodecParameters *FkFFMediaContext::_getCodecParameters(int trackId) {
    return ctx->streams[trackId]->codecpar;
}

void FkFFMediaContext::_checkBsf(int trackId) {
    if (ctx->streams[trackId]->codecpar->codec_id == AV_CODEC_ID_H264 && bsfContext == nullptr) {
        const AVBitStreamFilter *bsf = av_bsf_get_by_name("h264_mp4toannexb");
        if (bsf != nullptr) {
            if (av_bsf_alloc(bsf, &bsfContext) == 0) {
                auto ret = avcodec_parameters_copy(bsfContext->par_in, ctx->streams[trackId]->codecpar);
                if (ret < 0 || av_bsf_init(bsfContext) < 0) {
                    av_bsf_free(&bsfContext);
                    bsfContext = nullptr;
                }
            }
        }
    }
}

std::shared_ptr<FkAbsPacket> FkFFMediaContext::grab() {
    auto desc = _getVideoTrack();
    if (desc.trackId >= 0) {
        auto trackId = desc.trackId;
        auto itr = cnfGrabStatus.find(trackId);
        if (itr == cnfGrabStatus.end()) {
            cnfGrabStatus[trackId] = true;
            auto src = FkBuffer::wrap(ctx->streams[trackId]->codecpar->extradata, ctx->streams[trackId]->codecpar->extradata_size);
            auto out = FkFFUtils::transAvcExtraData2AnneXB(src);
            FkLogI(TAG, "[%d, %d, %d, %d]", out->data()[0], out->data()[1], out->data()[2], out->data()[3]);
            auto pkt = FkPacket2::make(out, kMediaType::VIDEO, FkTime::zero());
            pkt->setFlags(FK_CTL_FLAG_CONFIG);
            return pkt;
        }
        _checkBsf(trackId);
    }

    kMediaType type = kMediaType::AUDIO;
    av_packet_unref(avPacket);
    while (true) {
        auto ret = av_read_frame(ctx, avPacket);
        if (ret != 0) {
            FkLogW(TAG, "Read fail: %x(%s)", ret, av_err2str(ret));
        }
        if (ret == AVERROR_EOF) {
            return nullptr;
        }
        if (avPacket->stream_index == desc.trackId) {
            type = kMediaType::VIDEO;
            ret = av_bsf_send_packet(bsfContext, avPacket);
            ret = av_bsf_receive_packet(bsfContext, avPacket);
            if (ret == AVERROR(EAGAIN)) {
                continue;
            }
        }
        break;
    }
    auto timebase = ctx->streams[desc.trackId]->time_base;
    auto result = FkFFPacket::make(avPacket, type, timebase);
    return result;
}

std::shared_ptr<FkAbsPacket> FkFFMediaContext::grab(int trackId) {
    auto type = ctx->streams[avPacket->stream_index]->codecpar->codec_type;
    auto itr = cnfGrabStatus.find(trackId);
    if (itr == cnfGrabStatus.end()) {
        cnfGrabStatus[trackId] = true;
        auto src = FkBuffer::wrap(ctx->streams[trackId]->codecpar->extradata, ctx->streams[trackId]->codecpar->extradata_size);
        auto out = FkFFUtils::transAvcExtraData2AnneXB(src);
        FkLogI(TAG, "[%d, %d, %d, %d]", out->data()[0], out->data()[1], out->data()[2], out->data()[3]);
        auto pkt = FkPacket2::make(out, FkFFUtils::makeMediaType(type), FkTime::zero());
        pkt->setFlags(FK_CTL_FLAG_CONFIG);
        return pkt;
    }
    _checkBsf(trackId);

    av_packet_unref(avPacket);
    while (true) {
        auto ret = av_read_frame(ctx, avPacket);
        if (ret == AVERROR(EOF)) {
            FkLogW(TAG, "Read fail: %x(%s)", ret, av_err2str(ret));
            return nullptr;
        }
        ret = av_bsf_send_packet(bsfContext, avPacket);
        ret = av_bsf_receive_packet(bsfContext, avPacket);
        if (ret != 0) {
            continue;
        }

        FkLogI(TAG, "[%d, %d, %d, %d]", avPacket->data[0], avPacket->data[1], avPacket->data[2], avPacket->data[3]);
        if (trackId < 0 || avPacket->stream_index == trackId) {
            break;
        }
    }
    auto timebase = ctx->streams[trackId]->time_base;
    return FkFFPacket::make(avPacket, FkFFUtils::makeMediaType(type), timebase);
}

FkResult FkFFMediaContext::seek(int64_t timeInUS) {
    auto desc = _getVideoTrack();
    if (desc.trackId >= 0) {
        timeInUS = av_rescale_q_rnd(timeInUS,
                                    AV_TIME_BASE_Q,
                                    ctx->streams[desc.trackId]->time_base,
                                    AV_ROUND_NEAR_INF);
    } else {
        auto tracks = getTracks();
        if (!tracks.empty()) {
            timeInUS = av_rescale_q_rnd(timeInUS,
                                        AV_TIME_BASE_Q,
                                        ctx->streams[tracks[0].trackId]->time_base,
                                        AV_ROUND_NEAR_INF);
        }
    }
    int ret = avformat_seek_file(ctx, -1, INT64_MIN,
                                 timeInUS, INT64_MAX,
                                 timeInUS == 0 ? AVSEEK_FLAG_ANY : AVSEEK_FLAG_BACKWARD);
    return ret >= 0 ? FK_OK : FK_FAIL;
}
