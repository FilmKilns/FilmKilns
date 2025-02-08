/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWADBMEDIAFRAME_H
#define HARDWAREVIDEOCODEC_HWADBMEDIAFRAME_H

#include "HwAbsFrame.h"
#include "HwSourcesAllocator.h"
#include "HwFrameFormat.h"
#include "AlMediaDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/samplefmt.h"

#ifdef __cplusplus
}
#endif

AL_CLASS HwAbsMediaFrame AL_EXTEND HwSources, public HwAbsFrame {
public:
    /**
     * Convert to AVSampleFormat
     */
    static AVSampleFormat convertAudioFrameFormat(HwFrameFormat format);

    /**
     * Convert to HwFrameFormat
     */
    static HwFrameFormat convertToAudioFrameFormat(AVSampleFormat format);

    /**
     * For audio
     */
    static int getBytesPerSample(HwFrameFormat format);

    /**
     * Convert to AVPixelFormat
     */
    static AVPixelFormat convertVideoFrameFormat(HwFrameFormat format);

    /**
     * Convert to HwFrameFormat
     */
    static HwFrameFormat convertToVideoFrameFormat(AVPixelFormat format);

    /**
     * For video
     */
    static int getImageSize(HwFrameFormat format, int width, int height);

public:

    HwAbsMediaFrame(HwSourcesAllocator *allocator, HwFrameFormat format, size_t size);

    virtual ~HwAbsMediaFrame();

    void setFormat(HwFrameFormat format);

    HwFrameFormat getFormat();

    void setPts(int64_t pts);

    int64_t getPts();

    bool isVideo();

    bool isAudio();

    /**
     * us
     */
    virtual uint64_t duration()=0;

    virtual HwAbsMediaFrame *clone()=0;

    virtual void clone(HwAbsMediaFrame *src)=0;

    int32_t flags();

    void setFlags(int32_t flags);

private:
    HwFrameFormat format = HwFrameFormat::HW_FMT_NONE;
    /// Preview time in US.
    int64_t pts;
    int32_t _flags = 0;
};


#endif //HARDWAREVIDEOCODEC_HWADBMEDIAFRAME_H
