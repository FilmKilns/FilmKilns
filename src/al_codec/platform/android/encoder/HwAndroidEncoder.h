/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDENCODER_H
#define HWVC_ANDROID_HWANDROIDENCODER_H

#include "AlAbsVideoEncoder.h"
#include "AlCodec.h"
#include "HwAbsMuxer.h"
#include "AlFPSMeasurer.h"
#include <mutex>


#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

class HwAndroidEncoder : public AlAbsVideoEncoder {
public:
    HwAndroidEncoder(const AlAbsEncoder::Desc &desc);

    virtual ~HwAndroidEncoder();

    virtual void setBitrate(int32_t rate) override;

    virtual void setProfile(std::string profile) override;

    virtual void setPreset(std::string preset) override;

    virtual bool prepare(string path, int width, int height, HwSampleFormat audioFormat) override;

    virtual HwResult write(HwAbsMediaFrame *frame) override;

    virtual bool stop() override;

    virtual void release() override;

private:
    bool configure();

    void flush();

private:
    string path;
    int width = 0, height = 0;
    HwSampleFormat audioFormat = HwSampleFormat::NONE;
    AlCodec *vCodec = nullptr;
    AlCodec *aCodec = nullptr;
    HwAbsMuxer *muxer = nullptr;
    int32_t vTrack = HwAbsMuxer::TRACK_NONE, aTrack = HwAbsMuxer::TRACK_NONE;
    bool firstVideoFrameWrite = false;
    std::mutex lock;
    AlFPSMeasurer fps;
};


#endif //HWVC_ANDROID_HWANDROIDENCODER_H
