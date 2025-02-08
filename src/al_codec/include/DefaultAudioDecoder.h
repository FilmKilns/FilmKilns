/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H
#define HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H

#include "AbsAudioDecoder.h"
#include "HwAbsMediaFrame.h"
#include "HwFrameAllocator.h"
#include "HwAudioTranslator.h"
#include "AlBundle.h"

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

class DefaultAudioDecoder : public AbsAudioDecoder {
public:
    DefaultAudioDecoder();

    virtual ~DefaultAudioDecoder();

    virtual bool prepare(string path) override;

    void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) override;

    /**
     * @return 1: video, 2: audio, 0: failed
     */
    virtual HwResult grab(HwAbsMediaFrame **frame) override;

    virtual void setOutSampleFormat(HwSampleFormat format) override;

    virtual int getChannels() override;

    virtual int getSampleHz() override;

    virtual int getSampleFormat() override;

    virtual int getSamplesPerBuffer() override;

    virtual int64_t getAudioDuration() override;

    virtual int64_t getDuration() override;

    virtual int64_t getAudioStartTime() override;

    virtual void start() override;

    virtual void pause() override;

    virtual void stop() override;

private:
    bool openTrack(int track, AVCodecContext **context);

    AVSampleFormat getBestSampleFormat(AVSampleFormat in);

    HwAbsMediaFrame *resample(AVFrame *avFrame);

    void matchPts(AVFrame *frame, int track);

    int32_t _handleAction();

    void _checkFormat();

    HwAbsMediaFrame *_createEmptyFrame(int32_t flags);

private:
    string path;
    HwFrameAllocator *hwFrameAllocator = nullptr;
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext *aCodecContext = nullptr;
    HwAudioTranslator *translator = nullptr;
    int audioTrack = -1;
    AVPacket *avPacket = nullptr;
    AVFrame *audioFrame = nullptr;
    HwAbsMediaFrame *outHwFrame = nullptr;
    HwSampleFormat sFormat;
    HwSampleFormat oFormat;
    AVRational outputTimeBase = AVRational{1, 1000000};
    int64_t audioDurationUs = -1;
    int64_t durationUs = -1;
    bool eof = false;
    /** action */
    AlBundle mSeekAction;
};


#endif //HARDWAREVIDEOCODEC_DEFAULTAUDIODECODER_H
