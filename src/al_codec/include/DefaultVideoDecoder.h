/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_DEFAULTVIDEODECODER_H
#define HARDWAREVIDEOCODEC_DEFAULTVIDEODECODER_H

#include "AbsVideoDecoder.h"
#include "AbsAudioDecoder.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "HwAudioTranslator.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"


class DefaultVideoDecoder : public AbsVideoDecoder {
public:
    DefaultVideoDecoder();

    virtual ~DefaultVideoDecoder();

    virtual bool prepare(string path) override;

    virtual int width() override;

    virtual int height() override;

    virtual void setOutSampleFormat(HwSampleFormat format) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) override;

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame) override;

    virtual int64_t getVideoDuration() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    bool openTrack(int track, AVCodecContext **context);

    void printCodecInfo();

    HwAbsMediaFrame *resample(AVFrame *avFrame);

    AVSampleFormat getBestSampleFormat(AVSampleFormat in);

    void matchPts(AVFrame *frame, int track);

    void handleAction();

private:
    HwFrameAllocator *hwFrameAllocator = nullptr;
    string path;
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *vCodecContext = nullptr;
    AVCodecContext *aCodecContext = nullptr;
    HwAudioTranslator *translator = nullptr;
    int audioTrack = -1, videoTrack = -1;
    AVPacket *avPacket = nullptr;
    AVFrame *videoFrame = nullptr;
    AVFrame *audioFrame = nullptr;
    HwAbsMediaFrame *outHwFrame = nullptr;
    AVSampleFormat outSampleFormat = AV_SAMPLE_FMT_NONE;
    AVRational outputRational = AVRational{1, 1000000};
    int64_t videoDurationUs = -1;
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;
    bool eof = false;
    /** action */
    int64_t actionSeekInUs = -1;
};

#ifdef __cplusplus
}
#endif

#endif //HARDWAREVIDEOCODEC_DEFAULTVIDEODECODER_H
