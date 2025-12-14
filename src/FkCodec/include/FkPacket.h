/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWPACKET_H
#define HWVC_ANDROID_HWPACKET_H

#include "FkObject.h"
#include "FkBuffer.h"
#include "FkCodecDefinition.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"

#ifdef __cplusplus
}
#endif

/**
 *
 * For frame data after encoded. Like AVPacket
 */
FK_SUPER_CLASS(FkPacket, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkPacket)

public:
    static std::shared_ptr<FkPacket> wrap2(AVPacket *pkt, const AVRational time_base);

    static std::shared_ptr<FkPacket> wrap2(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags = 0);

    static std::shared_ptr<FkPacket> wrap(const std::shared_ptr<FkBuffer> &_buf, int64_t _pts, int64_t _dts, int32_t _flags = 0);

    static FkPacket *wrap(AVPacket *pkt);

    static FkPacket *wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags = 0);

    static FkPacket *create(size_t size, int64_t pts, int64_t dts);

    static std::shared_ptr<FkPacket> clone(FkPacket *pkt);

    static std::shared_ptr<FkPacket> clone(const std::shared_ptr<FkPacket> &pkt);

public:
    virtual ~FkPacket();

    FkPacket(const FkPacket &o) = delete;

    uint8_t *data();

    size_t size() const;

    int64_t getPts() const;

    int64_t getDts() const;

    int64_t getDuration() const;

    void setDuration(int64_t _duration);

    bool ref(AVPacket **pkt);

    int32_t getFlags() const;

    std::string getFlagsStr() const;

    std::string toString() override;

    void setType(kMediaType _type);

    kMediaType getType() const;

private:
    FkPacket();

private:
    std::shared_ptr<FkBuffer> buf = nullptr;
    //Time in US
    int64_t pts = INT64_MIN, dts = INT64_MIN;
    int64_t duration = 1;
    int32_t flags = 0;
    kMediaType type = kMediaType::NONE;
};


#endif //HWVC_ANDROID_HWPACKET_H
