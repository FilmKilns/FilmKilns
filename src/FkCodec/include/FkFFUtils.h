/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_FFUTILS_H
#define HARDWAREVIDEOCODEC_FFUTILS_H

#include "FkObject.h"
#include <pthread.h>
#include "FkCodecDefinition.h"
#include "FkBuffer.h"
#include <mutex>

static std::once_flag onceFlag;

FK_SUPER_CLASS(FkFFUtils, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFFUtils)

public:
    static int init();

    static int avSamplesCopy(AVFrame *dest, AVFrame *src);

    static void attachJvm(void *vm);

    static int exec(std::string cmd);

    static int trackInfo(std::string &file);

    static void showInfo();

    static kMediaType makeMediaType(AVMediaType type);

    /// ff_avc_write_annexb_extradata
    static std::shared_ptr<FkBuffer> transAvcExtraData2AnneXB(const std::shared_ptr<FkBuffer> &src);

    /**
     * Convert to AVSampleFormat
     */
    static AVSampleFormat convertAudioFrameFormat(kFrameFormat format);

    /**
     * Convert to HwFrameFormat
     */
    static kFrameFormat convert2AudioFrameFormat(AVSampleFormat format);

    /**
     * For audio
     */
    static int getBytesPerSample(kFrameFormat format);

private:
};

#endif //HARDWAREVIDEOCODEC_FFUTILS_H
