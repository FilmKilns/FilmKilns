/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFFUtils.h"
#include "FkBuffer.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include "fftools/ffmpeg.h"
#include "libavutil/intreadwrite.h"
#ifdef __cplusplus
}
#endif

#define TAG "FkFFUtils"

FK_IMPL_CLASS_TYPE(FkFFUtils, FkObject)

int FkFFUtils::init() {
    std::call_once(onceFlag, []() {
        av_register_all();
        avcodec_register_all();
    });
    return 0;
}

int FkFFUtils::avSamplesCopy(AVFrame *dest, AVFrame *src) {
    av_free(dest->data[0]);
    av_samples_alloc(dest->data,
                     &dest->linesize[0],
                     src->channels,
                     src->nb_samples,
                     AVSampleFormat(src->format), 0);
    dest->format = src->format;
    memcpy(dest->data[0], src->data[0], src->linesize[0]);
    return 0;
}

void FkFFUtils::attachJvm(void *vm) {
    av_jni_set_java_vm(vm, NULL);
}

static void parseArgv(std::string cmd, std::vector<std::string> &vec) {
    size_t pos = cmd.find(" ");
    std::string arg;
    while (pos != std::string::npos) {
        if (pos > 0 && cmd[pos - 1] == '\\') {
            arg += cmd.substr(0, pos - 1);
            arg += " ";
        } else {
            arg += cmd.substr(0, pos);
            vec.push_back(arg);
            arg = "";
        }
        cmd = cmd.substr(pos + 1);
        pos = cmd.find(" ");
    }
    if (cmd.size() + arg.length()) {
        vec.push_back(arg + cmd);
    }
}

int FkFFUtils::exec(std::string cmd) {
    std::vector<std::string> vec;
    parseArgv(cmd, vec);

    int argc = static_cast<int>(vec.size());
    char **argv = new char *[argc + 1];
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[vec[i].size() + 1];;
        strcpy(argv[i], vec[i].c_str());
    }
    argv[argc] = nullptr;
    int ret = _exec(argc, argv);
    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
    return ret;
}

int FkFFUtils::trackInfo(std::string &file) {
    init();
    int info = 0;
    auto *ctx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&ctx, file.c_str(), nullptr, nullptr) != 0) {
        FkLogE(TAG, "failed. Couldn't open input stream.");
        avformat_free_context(ctx);
        return info;
    }

    auto track = av_find_best_stream(ctx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (track >= 0) {
        info |= 0x1;
    }
    track = av_find_best_stream(ctx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (track >= 0) {
        info |= 0x2;
    }
    avformat_close_input(&ctx);
    avformat_free_context(ctx);
    return info;
}

static void showEncoderInfo() {
    std::string msg;
    AVInputFormat *aif = av_iformat_next(nullptr);
    msg.append("ENCODERS: ");
    while (aif != nullptr) {
        msg.append(aif->name);
        msg.append(", ");
        aif = aif->next;
    }
    FkLogI(TAG, "%s", msg.c_str());
}

static void showDecoderInfo() {
    std::string msg;
    msg.append("DECODERS: ");
    AVOutputFormat *aof = av_oformat_next(nullptr);
    while (aof != nullptr) {
        msg.append(aof->name);
        msg.append(", ");
        aof = aof->next;
    }
    FkLogI(TAG, "%s", msg.c_str());
}

void FkFFUtils::showInfo() {
    init();
    FkLogI(TAG, "FFmpeg ver %d.%d.%d",
           LIBSWRESAMPLE_VERSION_MAJOR,
           LIBSWRESAMPLE_VERSION_MINOR,
           LIBSWRESAMPLE_VERSION_MICRO);
    showEncoderInfo();
    showDecoderInfo();
    std::string msg;
    msg.append("CODEC: ");
    AVCodec *codec = nullptr;
    while ((codec = av_codec_next(codec))) {
        msg.append(codec->name);
        msg.append(", ");
    }
    FkLogI(TAG, "%s", msg.c_str());
}

static bool _openTrack(AVFormatContext *ctx, int track, AVCodecContext **context) {
    AVCodecParameters *avCodecParameters = ctx->streams[track]->codecpar;
    AVCodec *codec = NULL;
    if (AV_CODEC_ID_H264 == avCodecParameters->codec_id) {
        codec = avcodec_find_decoder_by_name("h264_mediacodec");
        if (NULL == codec) {
            FkLogI(TAG, "Selected AV_CODEC_ID_H264.");
            codec = avcodec_find_decoder(avCodecParameters->codec_id);
        }
    } else {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
    }
    if (NULL == codec) {
        FkLogE(TAG, "Couldn't find codec.");
        return false;
    }
    //打开编码器
    *context = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*context, avCodecParameters);
    if (avcodec_open2(*context, codec, NULL) < 0) {
        FkLogE(TAG, "Couldn't open codec.");
        return false;
    }
    std::string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    FkLogI(TAG, "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
           avCodecParameters->format, avCodecParameters->frame_size);
    return true;
}

static float _getAudioLevel(AVFrame *frame) {
    float result = 0.f;
    if (frame) {
        auto fmt = frame->format;
        auto left = FkBuffer::wrap(frame->data[0], frame->linesize[0]);
        std::shared_ptr<FkBuffer> right = nullptr;
        if (frame->linesize[1] > 0) {
            right = FkBuffer::wrap(frame->data[1], frame->linesize[1]);
        }
        switch (fmt) {
            case AV_SAMPLE_FMT_S16: {
                return left->getShort() * 1.f / 0x7FFF;
            }
            case AV_SAMPLE_FMT_FLTP: {
                result = left->getFloat();
                break;
            }
        }
    }
    return result;
}

kMediaType FkFFUtils::makeMediaType(AVMediaType type) {
    switch (type) {
        case AVMediaType::AVMEDIA_TYPE_VIDEO:
            return kMediaType::VIDEO;
        case AVMediaType::AVMEDIA_TYPE_AUDIO:
            return kMediaType::AUDIO;
    }
    return kMediaType::NONE;
}

std::shared_ptr<FkBuffer> FkFFUtils::transAvcExtraData2AnneXB(const std::shared_ptr<FkBuffer> &src) {
    auto *in = src->data();
    auto size0 = src->size();
    auto *size = &size0;

    uint16_t sps_size, pps_size;

    if (*size >= 4 && (AV_RB32(in) == 0x00000001 || AV_RB24(in) == 0x000001))
        return src;
    if (*size < 11 || in[0] != 1)
        return nullptr;

    sps_size = AV_RB16(&in[6]);
    if (11 + sps_size > *size)
        return nullptr;
    pps_size = AV_RB16(&in[9 + sps_size]);
    if (11 + sps_size + pps_size > *size)
        return nullptr;
    int out_size = 8 + sps_size + pps_size;
    auto dst = FkBuffer::alloc(out_size);
    uint8_t *out = dst->data();
    AV_WB32(&out[0], 0x00000001);
    memcpy(out + 4, &in[8], sps_size);
    AV_WB32(&out[4 + sps_size], 0x00000001);
    memcpy(out + 8 + sps_size, &in[11 + sps_size], pps_size);
    *size = out_size;
    return dst;
}

AVSampleFormat FkFFUtils::convertAudioFrameFormat(kFrameFormat format) {
    if (format >= kFrameFormat::SAMPLE_U8 && format < kFrameFormat::SAMPLE_END) {
        return static_cast<AVSampleFormat>(static_cast<int>(format) -
                                           static_cast<int>(kFrameFormat::SAMPLE_U8));
    }
    return AV_SAMPLE_FMT_NONE;
}

kFrameFormat FkFFUtils::convert2AudioFrameFormat(AVSampleFormat format) {
    if (format >= AV_SAMPLE_FMT_U8 && format < AV_SAMPLE_FMT_NB) {
        return static_cast<kFrameFormat>(static_cast<int>(format) +
                                         static_cast<int>(kFrameFormat::SAMPLE_U8));
    }
    return kFrameFormat::NONE;
}

int FkFFUtils::getBytesPerSample(kFrameFormat format) {
    return av_get_bytes_per_sample(convertAudioFrameFormat(format));
}
