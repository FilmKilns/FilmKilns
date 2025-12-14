/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_CODEC_FKABSMEDIAFRAME_H
#define FK_CODEC_FKABSMEDIAFRAME_H

#include "FkAbsFrame.h"
#include "FkSourcesAllocator.h"
#include "FkCodecDefinition.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/samplefmt.h"

#ifdef __cplusplus
}
#endif

FK_SUPER_CLASS(FkAbsMediaFrame, FkAbsFrame), public FkSources {
FK_DEF_CLASS_TYPE_FUNC(FkAbsMediaFrame)

public:
    /**
     * Convert to AVSampleFormat
     */
    static AVSampleFormat convertAudioFrameFormat(kFrameFormat format);

    /**
     * Convert to HwFrameFormat
     */
    static kFrameFormat convertToAudioFrameFormat(AVSampleFormat format);

    /**
     * For audio
     */
    static int getBytesPerSample(kFrameFormat format);

    /**
     * Convert to AVPixelFormat
     */
    static AVPixelFormat convertVideoFrameFormat(kFrameFormat format);

    /**
     * Convert to HwFrameFormat
     */
    static kFrameFormat convertToVideoFrameFormat(AVPixelFormat format);

    /**
     * For video
     */
    static int getImageSize(kFrameFormat format, int width, int height);

public:

    FkAbsMediaFrame(FkSourcesAllocator *allocator, kFrameFormat format, size_t size);

    virtual ~FkAbsMediaFrame();

    void setFormat(kFrameFormat _format);

    kFrameFormat getFormat();

    void setPts(int64_t _pts);

    int64_t getPts();

    bool isVideo();

    bool isAudio();

    /**
     * us
     */
    virtual uint64_t duration()=0;

    virtual FkAbsMediaFrame *clone()=0;

    virtual void clone(FkAbsMediaFrame *src)=0;

    int32_t flags();

    void setFlags(int32_t flags);

private:
    kFrameFormat format = kFrameFormat::NONE;
    /// Preview time in US.
    int64_t pts;
    int32_t _flags = 0;
};


#endif //FK_CODEC_FKABSMEDIAFRAME_H
