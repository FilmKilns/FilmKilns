/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWBITMAPINFO_H
#define HWVC_ANDROID_HWBITMAPINFO_H

#include "Object.h"
#include "AlColorFormat.h"
#include "AlRational.h"

al_class AlBitmapInfo al_extend Object {
public:
    static int bitsPerPixel(AlColorFormat colorFormat);

public:
    AlBitmapInfo();

    AlBitmapInfo(const AlBitmapInfo &o);

    virtual ~AlBitmapInfo();

    bool isNull();

    void reset();

    void dump();

public:
    int32_t width = 0;
    int32_t height = 0;
    /// Bits per color channel.
    uint32_t depth = 8;
    AlColorFormat colorFormat = AlColorFormat::NONE;
    ///表示需要旋转多少弧度才能正向像是，顺时针方向为正向(CW)
    AlRational rotation = AlRational();
};


#endif //HWVC_ANDROID_HWBITMAPINFO_H
