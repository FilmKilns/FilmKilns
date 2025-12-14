/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKFFENCODER_H
#define FK_CODEC_FKFFENCODER_H

#include "FkAbsVideoEncoder.h"
#include "FkAbsMediaFrame.h"
#include "FkAbsMuxer.h"
#include "FkCodec.h"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

FK_SUPER_CLASS(FkFFEncoder, FkAbsVideoEncoder) {
FK_DEF_CLASS_TYPE_FUNC(FkCodec)

public:
    FkFFEncoder(const FkAbsEncoder::Desc &desc);

    virtual ~FkFFEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string _profile) override;

    virtual void setPreset(std::string _preset) override;

    virtual bool prepare(std::string _path, int w, int h, FkSampleFormat _audioFormat) override;

    virtual FkResult write(FkAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool initialize();

private:
    std::string path;
    int width = 0;
    int height = 0;
    int32_t bitrate = 0;
    std::string profile;
    std::string preset;
    FkSampleFormat audioFormat = FkSampleFormat::NONE;
    int32_t aTrack = FkAbsMuxer::TRACK_NONE, vTrack = FkAbsMuxer::TRACK_NONE;
    FkAbsMuxer *muxer = nullptr;
    FkCodec *vCodec = nullptr;
    FkCodec *aCodec = nullptr;
    bool firstVideoFrameWrite = false;
    int64_t sampleCount = 0;
    std::mutex lock;

    int32_t quality = 18;

    int64_t countOfFrame = 0;
    int64_t lastTime = 0;
    int64_t countOfTime = 0;
};

#ifdef __cplusplus
}
#endif


#endif //FK_CODEC_FKFFENCODER_H
