/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFFEncoder.h"
#include "FkVideoFrame.h"
#include "FkAudioFrame.h"
#include "FkFFMuxer.h"
#include "FkFFCodec.h"
#include "FkTimeUtils.h"
#include "FkFFUtils.h"

#define TAG "FkFFEncoder"

FK_IMPL_CLASS_TYPE(FkFFEncoder, FkAbsVideoEncoder)

FkFFEncoder::FkFFEncoder(const FkAbsEncoder::Desc &desc) : FkAbsVideoEncoder(desc) {

}

FkFFEncoder::~FkFFEncoder() {
    release();
}

void FkFFEncoder::setBitrate(int32_t rate) {
    this->bitrate = rate;
}

void FkFFEncoder::setProfile(std::string _profile) {
    this->profile = _profile;
}

void FkFFEncoder::setPreset(std::string _preset) {
    this->preset = _preset;
}

bool FkFFEncoder::prepare(std::string _path, int w, int h, FkSampleFormat _audioFormat) {
    this->path = _path;
    this->width = w;
    this->height = h;
    this->audioFormat = _audioFormat;
    return initialize();
}

bool FkFFEncoder::initialize() {
    FkFFUtils::init();
    /**
     * Video codec
     */
    const int32_t fps = 30;
    FkBundle bundle;
    bundle.put(FkCodec::KEY_FORMAT, static_cast<int32_t>(kFrameFormat::IMAGE_YV12));
    bundle.put(FkCodec::KEY_WIDTH, width);
    bundle.put(FkCodec::KEY_HEIGHT, height);
    bundle.put(FkCodec::KEY_BIT_RATE, this->bitrate);
    bundle.put(FkCodec::KEY_FPS, fps);
    bundle.put(FkCodec::KEY_PROFILE, this->profile);
    bundle.put(FkCodec::KEY_PRESET, this->preset);
    //For CRF
    bundle.put(FkCodec::KEY_QUALITY, quality);
    vCodec = new FkFFCodec(getCodecDesc().vID);
    if (FK_OK != vCodec->configure(bundle)) {
        FkLogE(TAG, "failed to configure video codec!");
        release();
        return false;
    }
    vCodec->start();
    FkLogI(TAG, vCodec->getFormat()->toString());
    if (audioFormat.valid()) {
        /**
         * Audio codec
         */
        FkBundle aBundle;
        aBundle.put(FkCodec::KEY_SAMPLE_RATE, (int32_t) audioFormat.getSampleRate());
        aBundle.put(FkCodec::KEY_CHANNELS, audioFormat.getChannels());
        aBundle.put(FkCodec::KEY_FORMAT, static_cast<int32_t>(audioFormat.getFormat()));
        aBundle.put(FkCodec::KEY_BIT_RATE, 64000);
        aCodec = new FkFFCodec(getCodecDesc().aID);
        if (FK_OK != aCodec->configure(aBundle)) {
            FkLogE(TAG, "failed to configure audio codec!");
            release();
            return false;
        }
        aCodec->start();
        FkLogI(TAG, aCodec->getFormat()->toString());
    } else {
        FkLogW(TAG, "Without audio track.");
    }
    muxer = new FkFFMuxer();
    if (FK_OK != muxer->configure(path, FkAbsMuxer::TYPE_MP4)) {
        FkLogE(TAG, "failed to configure muxer!");
        release();
        return false;
    }
    /**
     * Muxer start
     */
    vTrack = muxer->addTrack(vCodec->getFormat());
    if (FkAbsMuxer::TRACK_NONE == vTrack) {
        FkLogE(TAG, "failed to add video track!");
        release();
        return false;
    }
    if (aCodec) {
        aTrack = muxer->addTrack(aCodec->getFormat());
        if (FkAbsMuxer::TRACK_NONE == aTrack) {
            FkLogE(TAG, "failed to add audio track!");
            release();
            return false;
        }
    }
    if (FK_OK != muxer->start()) {
        FkLogE(TAG, "failed to start muxer!");
        release();
        return false;
    }
    return true;
}

FkResult FkFFEncoder::write(FkAbsMediaFrame *frame) {
    std::lock_guard<std::mutex> guard(lock);
    FkPacket *packet = nullptr;
    if (frame->isAudio() && aCodec && muxer) {
        // Ensure that the first frame is video.
//        sampleCount += aCodec->getFrameSize();
        if (!firstVideoFrameWrite) {
            return FK_FAIL;
        }
        do {
            aCodec->process(&frame, &packet);
            if (packet) {
                muxer->write(aTrack, packet);
            }
        } while (packet && packet->getFlags() & FK_CTL_FLAG_CONFIG);
        return FK_OK;
    } else if (frame->isVideo() && vCodec && muxer) {
        int64_t time = FkTimeUtils::getCurrentTimeUS();
        if (lastTime > 0) {
            countOfTime += (time - lastTime);
            ++countOfFrame;
            if (countOfFrame >= 100) {
                FkLogI(TAG, "fps %lld", (countOfFrame * 1000000 / countOfTime));
                countOfTime = 0;
                countOfFrame = 0;
            }
        }
        lastTime = time;
        do {
            vCodec->process(&frame, &packet);
            if (packet) {
                muxer->write(vTrack, packet);
            }
        } while (packet && packet->getFlags() & FK_CTL_FLAG_CONFIG);
        firstVideoFrameWrite = true;
        return FK_OK;
    }
    return FK_FAIL;
}

bool FkFFEncoder::stop() {
//    int ret;
//    int got_picture;
//    if (!(pVideoStream->codec->codec->capabilities & AV_CODEC_CAP_DELAY)) {
//        return true;
//    }
//    while (1) {
//        avPacket->data = NULL;
//        avPacket->size = 0;
//        av_init_packet(avPacket);
//        ret = avcodec_encode_video2(pVideoStream->codec, avPacket,
//                                    NULL, &got_picture);
//        av_packet_unref(avPacket);
//        if (ret < 0)
//            break;
//        if (!got_picture) {
//            break;
//        }
//        ret = av_write_frame(pFormatCtx, avPacket);
//        if (ret < 0)
//            break;
//    }
    return true;
}

void FkFFEncoder::release() {
    delete aCodec;
    aCodec = nullptr;
    delete vCodec;
    vCodec = nullptr;
    delete muxer;
    muxer = nullptr;
    aTrack = FkAbsMuxer::TRACK_NONE;
    vTrack = FkAbsMuxer::TRACK_NONE;
}
