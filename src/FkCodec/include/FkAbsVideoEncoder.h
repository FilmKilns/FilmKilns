/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKABSVIDEOENCODER_H
#define FK_CODEC_FKABSVIDEOENCODER_H

#include "FkAbsEncoder.h"
#include "FkSampleFormat.h"

FK_SUPER_CLASS(FkAbsVideoEncoder, FkAbsEncoder) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsVideoEncoder)

public:
    FkAbsVideoEncoder(const FkAbsEncoder::Desc &desc);

    virtual ~FkAbsVideoEncoder();

    virtual bool prepare(std::string path, int width, int height, FkSampleFormat audioFormat) = 0;

    virtual void setBitrate(int32_t rate) = 0;

    virtual void setProfile(std::string profile) = 0;

    virtual void setPreset(std::string preset) = 0;

    virtual void setRotation(int32_t rotation) {}

    virtual FkResult addMuxer(std::string path, const std::shared_ptr<FkBundle> &bundle, std::function<void(int64_t, int byteSize)> callback);

    virtual FkResult removeMuxer(std::string path);
};


#endif //FK_CODEC_FKABSVIDEOENCODER_H
