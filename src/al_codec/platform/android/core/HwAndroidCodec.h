/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWANDROIDCODEC_H
#define HWVC_ANDROID_HWANDROIDCODEC_H

#include "AlCodec.h"
#include "HwBuffer.h"
#include "media/NdkMediaCodec.h"

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

/**
 * Hardware codec for android.
 *
 * Request key frame:
 * Bundle params = new Bundle();
 * params.putInt(MediaCodec.PARAMETER_KEY_REQUEST_SYNC_FRAME, 0);
 * mMediaCodec.setParameters(params);
 *
 */
class HwAndroidCodec : public AlCodec {
public:
    static AlCodec *createDecoder(AlCodec::kID id);

public:
    HwAndroidCodec(AlCodec::kID id);

    virtual ~HwAndroidCodec();

    virtual HwResult configure(AlBundle &format) override;

    virtual HwResult start() override;

    /**
     * @param packet AvPacket.
     */
    virtual HwResult process(HwAbsMediaFrame **frame, HwPacket **pkt) override;

    virtual void flush() override;

private:
    void release();

    HwResult push(uint8_t *data, size_t size, int64_t pts);

    HwResult pop(int32_t waitInUS);

private:
    const int COLOR_FormatYUV420Flexible = 0x7F420888;
    const int COLOR_FormatYUV420Planar = 19;
    const int COLOR_FormatYUV420SemiPlanar = 21;
    const int BUFFER_FLAG_KEY_FRAME = 1;
    const int BUFFER_FLAG_CODEC_CONFIG = 2;

    bool isEncodeMode = true;
    AMediaCodec *codec = nullptr;
    HwBuffer *keyFrameBuf = nullptr;
    HwPacket *hwPacket = nullptr;
    HwAbsMediaFrame *outFrame = nullptr;
    int32_t fps = 25;
    int32_t stride = 0;
};


#endif //HWVC_ANDROID_HWANDROIDCODEC_H
