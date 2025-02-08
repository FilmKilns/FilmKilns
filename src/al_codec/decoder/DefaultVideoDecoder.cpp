/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "DefaultVideoDecoder.h"
#include "AlFFUtils.h"
#include <cassert>
#include "log.h"
#include "AlFFUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

DefaultVideoDecoder::DefaultVideoDecoder() : AbsVideoDecoder() {
    hwFrameAllocator = new HwFrameAllocator();
}

DefaultVideoDecoder::~DefaultVideoDecoder() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (audioFrame) {
        av_frame_free(&audioFrame);
        audioFrame = nullptr;
    }
    if (videoFrame) {
        av_frame_free(&videoFrame);
        videoFrame = nullptr;
    }
    if (aCodecContext) {
        avcodec_close(aCodecContext);
        aCodecContext = nullptr;
    }
    if (vCodecContext) {
        avcodec_close(vCodecContext);
        vCodecContext = nullptr;
    }
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (outHwFrame) {
        outHwFrame->recycle();
        outHwFrame = nullptr;
    }
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
}

bool DefaultVideoDecoder::prepare(string path) {
    this->path = path;
    AlFFUtils::init();
    pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&pFormatCtx, path.c_str(), NULL, NULL) != 0) {
        LOGE("Couldn't open input stream.");
        return false;
    }
    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("Couldn't find stream information.");
        return false;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (-1 == videoTrack &&
            AVMediaType::AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codecpar->codec_type) {
            videoTrack = i;
        }
        if (-1 == audioTrack &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codecpar->codec_type) {
            audioTrack = i;
        }
    }
    if (videoTrack < 0) {
        Logcat::e("HWVC", "DefaultVideoDecoder::prepare open video track failed.");
        return false;
    }
    if (!openTrack(videoTrack, &vCodecContext)) {
        LOGE("******** Open video track failed. *********");
        return false;
    }
    if (-1 != audioTrack && !openTrack(audioTrack, &aCodecContext)) {
        Logcat::i("HWVC", "******** Open audio track failed. *********");
    }
    LOGI("DefaultVideoDecoder::prepare(%d x %d, du=%lld/%lld channels=%d, sampleHz=%d, frameSize=%d)",
         width(), height(), getVideoDuration(), getAudioDuration(),
         getChannels(), getSampleHz(), aCodecContext ? aCodecContext->frame_size : 0);
    if (aCodecContext) {
        outSampleFormat = getBestSampleFormat(aCodecContext->sample_fmt);
        translator = new HwAudioTranslator(
                HwSampleFormat(HwAbsMediaFrame::convertToAudioFrameFormat(outSampleFormat),
                               getChannels(),
                               getSampleHz()),
                HwSampleFormat(
                        HwAbsMediaFrame::convertToAudioFrameFormat(aCodecContext->sample_fmt),
                        getChannels(),
                        getSampleHz()));
        audioFrame = av_frame_alloc();
    }
    //准备资源
    avPacket = av_packet_alloc();
    videoFrame = av_frame_alloc();
    return true;
}

void DefaultVideoDecoder::handleAction() {
    if (actionSeekInUs >= 0) {
        avcodec_flush_buffers(vCodecContext);
        if (aCodecContext) {
            avcodec_flush_buffers(aCodecContext);
        }
        int ret = avformat_seek_file(pFormatCtx, -1, INT64_MIN,
                                     actionSeekInUs, INT64_MAX,
                                     AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            LOGI("DefaultVideoDecoder::seek failed");
            return;
        }
        avcodec_flush_buffers(vCodecContext);
        if (aCodecContext) {
            avcodec_flush_buffers(aCodecContext);
        }
        LOGI("DefaultVideoDecoder::seek: %lld", actionSeekInUs);
        actionSeekInUs = -1;
    }
}
/**
 * Get an audio or a video frame.
 * @param frame 每次返回的地址可能都一样，所以获取一帧音视频后请立即使用，在下次grab之后可能会被释放
 */
HwResult DefaultVideoDecoder::grab(HwAbsMediaFrame **frame) {
    handleAction();
    while (true) {
        int ret = av_read_frame(pFormatCtx, avPacket);
        if (0 == ret) {
            if (videoTrack == avPacket->stream_index) {
                avcodec_send_packet(vCodecContext, avPacket);
            } else if (audioTrack == avPacket->stream_index) {
                avcodec_send_packet(aCodecContext, avPacket);
            }
        }
        av_packet_unref(avPacket);//Or av_free_packet?
//            switch (ret) {
//                case AVERROR(EAGAIN): {
//                    LOGI("you must read output with avcodec_receive_frame");
//                }
//                case AVERROR(EINVAL): {
//                    LOGI("codec not opened, it is an encoder, or requires flush");
//                    break;
//                }
//                case AVERROR(ENOMEM): {
//                    LOGI("failed to add packet to internal queue");
//                    break;
//                }
//                case AVERROR_EOF: {
//                    LOGI("eof");
//                    eof = true;
//                    break;
//                }
//                default:
//                    LOGI("avcodec_send_packet ret=%d", ret);
//            }
        if (AVERROR_EOF == ret) {
            eof = true;
        }
        //尝试去缓冲区中获取解码完成的视频帧
        if (0 == avcodec_receive_frame(vCodecContext, videoFrame)) {
            matchPts(videoFrame, videoTrack);
            if (outHwFrame) {
                outHwFrame->recycle();
            }
            outHwFrame = hwFrameAllocator->ref(videoFrame);
            *frame = outHwFrame;
            Logcat::i("HWVC", "DefaultVideoDecoder::grab video, %d x %d, %d",
                      videoFrame->width,
                      videoFrame->height,
                      videoFrame->format);
            av_frame_unref(videoFrame);
            return Hw::MEDIA_SUCCESS;
        }
        //如果没有视频帧，尝试去缓冲区中获取解码完成的音频帧
        if (aCodecContext && 0 == avcodec_receive_frame(aCodecContext, audioFrame)) {
            matchPts(audioFrame, audioTrack);
            if (outHwFrame) {
                outHwFrame->recycle();
                outHwFrame = nullptr;
            }
            outHwFrame = resample(audioFrame);
            *frame = outHwFrame;
            av_frame_unref(audioFrame);
            return Hw::MEDIA_SUCCESS;
        }
        //如果缓冲区中既没有音频也没有视频，并且已经读取完文件，则播放完了
        if (eof) {
            Logcat::i("HWVC", "DefaultVideoDecoder::grab end");
            return Hw::MEDIA_EOF;
        }
    }
}

HwAbsMediaFrame *DefaultVideoDecoder::resample(AVFrame *avFrame) {
    if (!translator) {
        return nullptr;
    }
    AVFrame *outFrame = nullptr;
    if (translator->translate(&outFrame, &avFrame)) {
        return hwFrameAllocator->ref(outFrame);
    }
    return nullptr;
}

int DefaultVideoDecoder::width() {
    if (!pFormatCtx) return 0;
    return pFormatCtx->streams[videoTrack]->codecpar->width;
}

int DefaultVideoDecoder::height() {
    if (!pFormatCtx) return 0;
    return pFormatCtx->streams[videoTrack]->codecpar->height;
}

bool DefaultVideoDecoder::openTrack(int track, AVCodecContext **context) {
    AVCodecParameters *avCodecParameters = pFormatCtx->streams[track]->codecpar;
    if (videoTrack == track) {
        LOGI("DefaultVideoDecoder(%s) %d x %d", path.c_str(), avCodecParameters->width,
             avCodecParameters->height);
    }
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
        LOGE("Couldn't find codec.");
        return false;
    }
    //打开编码器
    *context = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*context, avCodecParameters);
    if (avcodec_open2(*context, codec, NULL) < 0) {
        LOGE("Couldn't open codec.");
        return false;
    }
    string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    Logcat::i("HWVC", "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
              avCodecParameters->format, avCodecParameters->frame_size);
    return true;
}

void DefaultVideoDecoder::printCodecInfo() {
    char info[1024] = {0};
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%s[Dec]", info);
        } else {
            sprintf(info, "%s[Enc]", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }
    LOGI("%s", info);
}

void DefaultVideoDecoder::setOutSampleFormat(HwSampleFormat format) {

}

int DefaultVideoDecoder::getChannels() {
    return aCodecContext ? aCodecContext->channels : 0;
}

int DefaultVideoDecoder::getSampleHz() {
    return aCodecContext ? aCodecContext->sample_rate : 0;
}

AVSampleFormat DefaultVideoDecoder::getBestSampleFormat(AVSampleFormat in) {
    AVSampleFormat out = av_get_packed_sample_fmt(in);
    if (AV_SAMPLE_FMT_FLT == out || AV_SAMPLE_FMT_DBL == out) {
        out = AV_SAMPLE_FMT_S32;
    }
    return out;
}

int DefaultVideoDecoder::getSampleFormat() {
    return outSampleFormat;
}

int DefaultVideoDecoder::getSamplesPerBuffer() {
    return aCodecContext ? aCodecContext->frame_size : 0;
}

void DefaultVideoDecoder::matchPts(AVFrame *frame, int track) {
    frame->pts = av_rescale_q_rnd(frame->pts,
                                  pFormatCtx->streams[track]->time_base,
                                  outputRational,
                                  AV_ROUND_NEAR_INF);
}

void DefaultVideoDecoder::seek(int64_t us, AbsDecoder::kSeekMode mode) {
    actionSeekInUs = us;
    eof = false;
}

int64_t DefaultVideoDecoder::getVideoDuration() {
    if (videoDurationUs >= 0) {
        return videoDurationUs;
    }
    videoDurationUs = pFormatCtx->streams[videoTrack]->duration;
    videoDurationUs = av_rescale_q_rnd(videoDurationUs,
                                       pFormatCtx->streams[videoTrack]->time_base,
                                       outputRational,
                                       AV_ROUND_NEAR_INF);
    if (videoDurationUs < 0) {
        videoDurationUs = pFormatCtx->duration;
    }
    return videoDurationUs;
}

int64_t DefaultVideoDecoder::getAudioDuration() {
    if (audioTrack < 0) {
        audioDurationUs = 0;
    }
    if (audioDurationUs >= 0) {
        return audioDurationUs;
    }
    audioDurationUs = pFormatCtx->streams[audioTrack]->duration;
    audioDurationUs = av_rescale_q_rnd(audioDurationUs,
                                       pFormatCtx->streams[audioTrack]->time_base,
                                       outputRational,
                                       AV_ROUND_NEAR_INF);
    if (audioDurationUs < 0) {
        audioDurationUs = pFormatCtx->duration;
    }
    return audioDurationUs;
}

int64_t DefaultVideoDecoder::getDuration() {
    if (durationUs >= 0) {
        return durationUs;
    }
    durationUs = pFormatCtx->duration;
    if (durationUs <= 0) {
        durationUs = getAudioDuration();
    }
    if (durationUs <= 0) {
        durationUs = getVideoDuration();
    }
    return durationUs;
}

void DefaultVideoDecoder::start() {

}

void DefaultVideoDecoder::pause() {

}

void DefaultVideoDecoder::stop() {

}

int64_t DefaultVideoDecoder::getAudioStartTime() {
    return 0;
}

#ifdef __cplusplus
}
#endif