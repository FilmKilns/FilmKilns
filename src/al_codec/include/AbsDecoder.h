/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_ABSDECODER_H
#define HARDWAREVIDEOCODEC_ABSDECODER_H

#include "Object.h"
#include "HwResult.h"
#include "HwAbsMediaFrame.h"
#include <string>

using namespace std;

class AbsDecoder : public Object {
public:
    AL_ENUM kSeekMode : int {
        BACKWARD = 0,
        EXACT
    };

public:
    AbsDecoder();

    virtual ~AbsDecoder();

    virtual bool prepare(string path)=0;

    virtual HwResult grab(HwAbsMediaFrame **frame)=0;

    virtual void seek(int64_t us, AbsDecoder::kSeekMode mode = AbsDecoder::kSeekMode::BACKWARD) = 0;

    virtual int64_t getDuration()=0;

    virtual void start()=0;

    virtual void pause()=0;

    virtual void stop()=0;
};
namespace Hw {
    const HwResult MEDIA_WAIT = HwResult(R_KID('M', 'D', 'A', 0x1));
    const HwResult MEDIA_EOF = Hw::IO_EOF;
    const HwResult MEDIA_SUCCESS = Hw::SUCCESS;
}


#endif //HARDWAREVIDEOCODEC_ABSDECODER_H
