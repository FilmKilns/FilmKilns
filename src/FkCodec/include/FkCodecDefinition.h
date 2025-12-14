/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKCODECDEFINITION_H
#define FK_CODEC_FKCODECDEFINITION_H

#include "FkColor.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libavcodec/jni.h"

#ifdef __cplusplus
}
#endif

/**
 * YUV420P: YV12(YYYYYYYY VV UU)/YU12/I420(YYYYYYYY UU VV)
 * YUV420SP: NV12(YYYYYYYY UVUV)/NV21(YYYYYYYY VUVU)
 */
FK_ENUM kFrameFormat : int {
    NONE = 0,
    IMAGE_RGB = static_cast<int>(FkColor::kFormat::RGB),
    IMAGE_RGBA = static_cast<int>(FkColor::kFormat::RGBA),
    IMAGE_YV12 = static_cast<int>(FkColor::kFormat::YV12),
    IMAGE_NV12 = static_cast<int>(FkColor::kFormat::NV12),
    IMAGE_END = static_cast<int>(FkColor::kFormat::END),    /** End flag of image formats. DO NOT USE if linking dynamically */

    SAMPLE_U8 = 100,/** unsigned 8 bits */
    SAMPLE_S16,     /** signed 16 bits */
    SAMPLE_S32,     /** signed 32 bits */
    SAMPLE_FLT,     /** float */
    SAMPLE_DBL,     /** double */

    SAMPLE_U8P,     /** unsigned 8 bits, planar */
    SAMPLE_S16P,    /** signed 16 bits, planar */
    SAMPLE_S32P,    /** signed 32 bits, planar */
    SAMPLE_FLTP,    /** float, planar */
    SAMPLE_DBLP,    /** double, planar */
    SAMPLE_S64,     /** signed 64 bits */
    SAMPLE_S64P,    /** signed 64 bits, planar */
    SAMPLE_END,    /** End flag of sample formats. DO NOT USE if linking dynamically */
};

FK_ENUM kMediaType : int {
    NONE = -1,
    VIDEO,
    AUDIO,
    AV,
};

/// Same with AV_PKT_FLAG_KEY/AV_PKT_FLAG_CORRUPT...
static constexpr int32_t FK_CTL_FLAG_KEY = 0x0001;
static constexpr int32_t FK_CTL_FLAG_CORRUPT = 0x0002;
static constexpr int32_t FK_CTL_FLAG_DISCARD = 0x0004;
static constexpr int32_t FK_CTL_FLAG_TRUSTED = 0x0008;
static constexpr int32_t FK_CTL_FLAG_DISPOSABLE = 0x0010;
static constexpr int32_t FK_CTL_FLAG_CONFIG = 0x0020;

static constexpr int32_t FK_CTL_FLAG_SEEK_DONE = 0x40000000;
static constexpr int32_t FK_CTL_FLAG_EOF = 0x80000000;

#endif //FK_CODEC_FKCODECDEFINITION_H
