/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_ABSENCODER_H
#define FK_CODEC_ABSENCODER_H

#include "FkObject.h"
#include "FkCodec.h"
#include "FkAbsMediaFrame.h"


FK_SUPER_CLASS(FkAbsEncoder, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsEncoder)

public:
    FK_ENUM kType : int {
        SOFT = 0,
        HARD,
        HARD_ENC_TEX
    };

    class Desc {
    public:
        Desc();

        Desc(FkCodec::kID id, kType type);

        Desc(const Desc &o);

        virtual ~Desc();

    public:
        FkCodec::kID aID;
        FkCodec::kID vID;
        kType type;
    };

public:
    FkAbsEncoder(const FkAbsEncoder::Desc &desc);

    virtual ~FkAbsEncoder();

    virtual FkResult write(FkAbsMediaFrame *frame) = 0;

    virtual bool stop() = 0;

    virtual void release() = 0;

    virtual FkAbsEncoder::Desc getCodecDesc();

private:
    FkAbsEncoder(const FkAbsEncoder &o) : FkObject() {};

private:
    FkAbsEncoder::Desc desc;
};


#endif //FK_CODEC_ABSENCODER_H
