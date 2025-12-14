/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAudioTranslator.h"
#include "FkAbsMediaFrame.h"

#define TAG "FkAudioTranslator"

FK_IMPL_CLASS_TYPE(FkAudioTranslator, FkObject)

FkAudioTranslator::FkAudioTranslator(FkSampleFormat outFormat, FkSampleFormat inFormat)
        : outFormat(outFormat),
          inFormat(inFormat),
          FkObject() {
    swrContext = swr_alloc_set_opts(swrContext,
                                    av_get_default_channel_layout(outFormat.getChannels()),
                                    FkAbsMediaFrame::convertAudioFrameFormat(outFormat.getFormat()),
                                    outFormat.getSampleRate(),
                                    av_get_default_channel_layout(inFormat.getChannels()),
                                    FkAbsMediaFrame::convertAudioFrameFormat(inFormat.getFormat()),
                                    inFormat.getSampleRate(),
                                    0, nullptr);
    if (!swrContext || 0 != swr_init(swrContext)) {
        FkLogE(TAG, "failed");
        if (swrContext) {
            swr_free(&swrContext);
            swrContext = nullptr;
        }
    }
    FkLogI(TAG, "(%d, %d, %d) <- (%d, %d, %d)",
           outFormat.getChannels(),
           FkAbsMediaFrame::convertAudioFrameFormat(outFormat.getFormat()),
           outFormat.getSampleRate(),
           inFormat.getChannels(),
           FkAbsMediaFrame::convertAudioFrameFormat(inFormat.getFormat()),
           inFormat.getSampleRate());

}

FkAudioTranslator::~FkAudioTranslator() {
    if (swrContext) {
        swr_close(swrContext);
        swr_free(&swrContext);
        swrContext = nullptr;
    }
    if (outFrame) {
        av_frame_free(&outFrame);
        outFrame = nullptr;
    }
}

bool FkAudioTranslator::translate(AVFrame **dest, AVFrame **src) {
    if (!swrContext || !src || !(*src)) {
        return false;
    }
    int nbSample = outFormat.getSampleRate() * (*src)->nb_samples / inFormat.getSampleRate();
    if (!outFrame || outFrame->nb_samples != nbSample ||
        outFrame->sample_rate != outFormat.getSampleRate() ||
        outFrame->channels != outFormat.getChannels()) {
        if (outFrame) {
            av_frame_free(&outFrame);
        }
        outFrame = av_frame_alloc();
        outFrame->nb_samples = nbSample;
        outFrame->format = FkAbsMediaFrame::convertAudioFrameFormat(outFormat.getFormat());
        outFrame->channels = outFormat.getChannels();
        outFrame->channel_layout = av_get_default_channel_layout(outFormat.getChannels());
        outFrame->sample_rate = outFormat.getSampleRate();
        outFrame->pts = 0;
    }
    if (0 == src[0]->channel_layout) {
        src[0]->channel_layout = av_get_default_channel_layout(inFormat.getChannels());
    }
    int ret = swr_convert_frame(swrContext, outFrame, src[0]);
    if (0 != ret) {
        FkLogE(TAG, "translator(%p) failed: %s", this, strerror(AVUNERROR(ret)));
        return false;
    }
    outFrame->pts = (*src)->pts;
    dest[0] = outFrame;
    return true;
}