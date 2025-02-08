/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFFENCODER_H
#define HWVC_ANDROID_ALFFENCODER_H

#include "AlAbsVideoEncoder.h"
#include "HwResult.h"
#include "HwAbsMediaFrame.h"
#include "HwAbsMuxer.h"
#include "AlCodec.h"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

AL_CLASS AlFFEncoder AL_EXTEND AlAbsVideoEncoder {
public:
    AlFFEncoder(const AlAbsEncoder::Desc &desc);

    virtual ~AlFFEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string profile) override;

    virtual void setPreset(std::string preset) override;

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool initialize();

private:
    string path;
    int width = 0;
    int height = 0;
    int32_t bitrate = 0;
    std::string profile;
    std::string preset;
    HwSampleFormat audioFormat = HwSampleFormat::NONE;
    int32_t aTrack = HwAbsMuxer::TRACK_NONE, vTrack = HwAbsMuxer::TRACK_NONE;
    HwAbsMuxer *muxer = nullptr;
    AlCodec *vCodec = nullptr;
    AlCodec *aCodec = nullptr;
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


#endif //HWVC_ANDROID_ALFFENCODER_H
