/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKRESAMPLER_H
#define FK_CODEC_FKRESAMPLER_H

#include "FkObject.h"
#include "FkSampleFormat.h"

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

FK_SUPER_CLASS(FkAudioTranslator, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAudioTranslator)

public:
    FkAudioTranslator(FkSampleFormat outFormat, FkSampleFormat inFormat);

    virtual ~FkAudioTranslator();

    bool translate(AVFrame **dest, AVFrame **src);

private:
    SwrContext *swrContext = nullptr;
    FkSampleFormat inFormat;
    FkSampleFormat outFormat;
    AVFrame *outFrame = nullptr;
};


#endif //FK_CODEC_FKRESAMPLER_H
