/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWFRAMEFORMAT_H
#define HWVC_ANDROID_HWFRAMEFORMAT_H

#include "AlColorFormat.h"
/**
 * YUV420P: YV12(YYYYYYYY VV UU)/YU12/I420(YYYYYYYY UU VV)
 * YUV420SP: NV12(YYYYYYYY UVUV)/NV21(YYYYYYYY VUVU)
 */
enum class HwFrameFormat : int {
    HW_FMT_NONE = -1,
    HW_IMAGE_RGB = static_cast<int>(AlColorFormat::RGB),
    HW_IMAGE_RGBA = static_cast<int>(AlColorFormat::RGBA),
    HW_IMAGE_YV12 = static_cast<int>(AlColorFormat::YV12),
    HW_IMAGE_NV12 = static_cast<int>(AlColorFormat::NV12),
    HW_IMAGE_END = static_cast<int>(AlColorFormat::END),    /** End flag of image formats. DO NOT USE if linking dynamically */

    HW_SAMPLE_U8 = 100,/** unsigned 8 bits */
    HW_SAMPLE_S16,     /** signed 16 bits */
    HW_SAMPLE_S32,     /** signed 32 bits */
    HW_SAMPLE_FLT,     /** float */
    HW_SAMPLE_DBL,     /** double */

    HW_SAMPLE_U8P,     /** unsigned 8 bits, planar */
    HW_SAMPLE_S16P,    /** signed 16 bits, planar */
    HW_SAMPLE_S32P,    /** signed 32 bits, planar */
    HW_SAMPLE_FLTP,    /** float, planar */
    HW_SAMPLE_DBLP,    /** double, planar */
    HW_SAMPLE_S64,     /** signed 64 bits */
    HW_SAMPLE_S64P,    /** signed 64 bits, planar */
    HW_SAMPLE_END,    /** End flag of sample formats. DO NOT USE if linking dynamically */
};

#endif //HWVC_ANDROID_HWFRAMEFORMAT_H
