/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "../include/HwAudioPlayer.h"
#include "../include/log.h"
#include "../include/ObjectBox.h"
#include "../include/TimeUtils.h"

#define TAG "HwAudioPlayer"

void bufferQueueCallback(SLAndroidSimpleBufferQueueItf slBufferQueueItf, void *context) {
    HwAudioPlayer *player = static_cast<HwAudioPlayer *>(context);
    player->bufferEnqueue(slBufferQueueItf);
}

HwAudioPlayer::HwAudioPlayer(uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(HwAudioDeviceMode::Normal,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(nullptr);
}

HwAudioPlayer::HwAudioPlayer(HwAudioDeviceMode mode,
                             uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(mode,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(nullptr);
}

HwAudioPlayer::HwAudioPlayer(SLEngine *engine,
                             HwAudioDeviceMode mode,
                             uint16_t channels,
                             uint32_t sampleRate,
                             uint16_t format,
                             uint32_t samplesPerBuffer) : SLAudioDevice(mode,
                                                                        channels,
                                                                        sampleRate,
                                                                        format,
                                                                        samplesPerBuffer) {
    initialize(engine);
}

void HwAudioPlayer::initialize(SLEngine *engine) {
    this->silenceData = new uint8_t[getBufferByteSize()];
    memset(silenceData, 0, getBufferByteSize());
    this->engine = engine;
    uint32_t bufSize = getBufferByteSize() * 16;
    switch (mode) {
        case HwAudioDeviceMode::LowLatency:
            bufSize = getBufferByteSize() * 3;
            break;
        case HwAudioDeviceMode::Normal:
            bufSize = getBufferByteSize() * 16;
            break;
        case HwAudioDeviceMode::HighLatency:
            bufSize = getBufferByteSize() * 32;
    }
    this->fifo = new HwFIFOBuffer(bufSize);
    LOGI("Create HwAudioPlayer, channels=%d, sampleHz=%d, minBufferSize=%d, format=%d",
         this->channels,
         this->sampleRate,
         this->samplesPerBuffer,
         this->format);
    mixObject = nullptr;
    playObject = nullptr;
    playItf = nullptr;
    HwResult ret = this->createEngine();
    if (Hw::SUCCESS != ret) {
        LOGE("HwAudioPlayer create failed");
    }

}

HwAudioPlayer::~HwAudioPlayer() {
    LOGI("HwAudioPlayerer");
    stop();
    if (silenceData) {
        delete[]silenceData;
        silenceData = nullptr;
    }
}

HwResult HwAudioPlayer::createEngine() {
    if (!engine) {
        ownEngine = true;
        engine = new SLEngine();
        if (!engine || !engine->valid()) {
            LOGE("HwAudioPlayer create failed");
            stop();
            return Hw::FAILED;
        }
    }

    SLresult result = (*engine->getEngine())->CreateOutputMix(engine->getEngine(), &mixObject, 0,
                                                              nullptr, nullptr);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("CreateOutputMix failed!");
        return Hw::FAILED;
    }
    result = (*mixObject)->Realize(mixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("OutputMix Realize failed!");
        return Hw::FAILED;
    }
    return createBufferQueueAudioPlayer();
}

HwResult HwAudioPlayer::start() {
    (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
    uint32_t bufSize = getBufferByteSize();
    uint8_t buffer[bufSize];
    memset(buffer, 0, bufSize);
    write(buffer, bufSize);
    bufferEnqueue(bufferQueueItf);
    SLresult result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Recorder SetRecordState start failed!");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult HwAudioPlayer::createBufferQueueAudioPlayer() {
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

    SLAndroidDataFormat_PCM_EX format_pcm = {SL_DATAFORMAT_PCM,
                                             channels,
                                             sampleRate * 1000,
                                             format,
                                             format,
                                             getChannelMask(),
                                             SL_BYTEORDER_LITTLEENDIAN};
    //在 Android 5.0（API 级别 21）及更高版本上运行的应用可以使用单精度浮点格式向音频播放器提供数据。
//    SLAndroidDataFormat_PCM_EX format_pcm = {SL_ANDROID_DATAFORMAT_PCM_EX,
//                                             channels,
//                                             sampleRate * 1000,
//                                             SL_PCMSAMPLEFORMAT_FIXED_32,
//                                             SL_PCMSAMPLEFORMAT_FIXED_32,
//                                             getChannelMask(),
//                                             SL_BYTEORDER_LITTLEENDIAN,
//                                             SL_ANDROID_PCM_REPRESENTATION_FLOAT};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX,
                                          mixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
    /*
     * create fast path audio player: SL_IID_BUFFERQUEUE and SL_IID_VOLUME
     * and other non-signal processing interfaces are ok.
     */
    SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    SLresult result = (*engine->getEngine())->CreateAudioPlayer(engine->getEngine(),
                                                                &playObject,
                                                                &audioSrc,
                                                                &audioSnk,
                                                                sizeof(ids) / sizeof(ids[0]),
                                                                ids,
                                                                req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("CreateAudioPlayer failed! ret=%d", result);
        return Hw::FAILED;
    }
    result = (*playObject)->Realize(playObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Player Realize failed!");
        return Hw::FAILED;
    }
    result = (*playObject)->GetInterface(playObject, SL_IID_PLAY, &playItf);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Player GetInterface failed!");
        return Hw::FAILED;
    }
    result = (*playObject)->GetInterface(playObject, SL_IID_BUFFERQUEUE, &bufferQueueItf);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Player GetInterface buffer queue failed!");
        return Hw::FAILED;
    }
    result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Player SetPlayState stop failed!");
        return Hw::FAILED;
    }
    result = (*bufferQueueItf)->RegisterCallback(bufferQueueItf,
                                                 bufferQueueCallback,
                                                 this);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Player RegisterCallback failed!");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

static int64_t ttime = 0;

void HwAudioPlayer::bufferEnqueue(SLAndroidSimpleBufferQueueItf slBufferQueueItf) {
//    if (!recycler) {
//        return;
//    }
//    auto *buffer = recycler->take();
//    if (buffer) {
//        (*slBufferQueueItf)->Enqueue(bufferQueueItf, buffer->ptr, getBufferByteSize());
//    }
//    recycler->recycle(buffer);
    //-----------------------
//    ObjectBox *buffer = nullptr;
//    if (pcmList.size() > 0) {
//        buffer = pcmList.front();
//        pcmList.pop();
//    } else {
//        buffer = new ObjectBox(new uint8_t[getBufferByteSize()]);
//        memset(buffer->ptr, 0, getBufferByteSize());
//    }
//    (*slBufferQueueItf)->Enqueue(bufferQueueItf, buffer->ptr, getBufferByteSize());
//    delete buffer;
    //-----------------------
    if (!fifo) {
        return;
    }
//    Logcat::i("HWVC", "HwAudioPlayer::bufferEnqueue cost %lld", getCurrentTimeUS() - ttime);
    ttime = getCurrentTimeUS();
    HwBuffer *buf = fifo->take(getBufferByteSize());
    if (buf) {
        (*slBufferQueueItf)->Enqueue(bufferQueueItf, buf->data(), buf->size());
        delete buf;
        return;
    }
    AlLogW(TAG, "Play silence data.");
    (*slBufferQueueItf)->Enqueue(bufferQueueItf, silenceData, getBufferByteSize());
}

HwResult HwAudioPlayer::write(uint8_t *buffer, size_t size) {
    return write(buffer, size, 0);
}

HwResult HwAudioPlayer::write(uint8_t *buffer, size_t size, int timeOut) {
//    ObjectBox *cache = recycler->takeCache();
//    if (!cache) {
//        LOGE("Cache invalid");
//        return Hw::FAILED;
//    }
//    memcpy(cache->ptr, buffer, size);
//    recycler->offer(cache);
    //------------------
//    uint8_t *data = new uint8_t[size];
//    memcpy(data, buffer, size);
//    pcmList.push(new ObjectBox(data));
    //-----------------
    size_t ret = fifo->push(buffer, size, timeOut);
    if (0 == ret) {
#if defined(__AL_DEBUG__)
        start();
        AlLogE(TAG, "failed, try flush & restart.");
#endif
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

void HwAudioPlayer::flush() {
    if (fifo) {
        fifo->flush();
    }
}

void HwAudioPlayer::stop() {
    AlLogI(TAG, "");
    if (playObject) {
        SLresult result = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_STOPPED);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("Player SetPlayState stop failed!");
        }
        (*playObject)->Destroy(playObject);
        playObject = nullptr;
        bufferQueueItf = nullptr;
        playItf = nullptr;
    }
    if (nullptr != mixObject) {
        (*mixObject)->Destroy(mixObject);
        mixObject = nullptr;
    }
    destroyEngine();
    if (fifo) {
        delete fifo;
        fifo = nullptr;
    }
}

void HwAudioPlayer::destroyEngine() {
    if (ownEngine && engine) {
        delete engine;
        engine = nullptr;
        ownEngine = false;
    }
}

size_t HwAudioPlayer::getCacheSamples() {
    if (nullptr == fifo) {
        return 0;
    }
    return fifo->size() / getChannels() / getFormat();
}
