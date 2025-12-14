//
// Created by lmy on 2023/10/14.
//
#pragma once

#include "FkAbsPacket.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"

#ifdef __cplusplus
}
#endif

FK_SUPER_CLASS(FkFFPacket, FkAbsPacket) {
FK_DEF_CLASS_TYPE_FUNC(FkFFPacket)

public:
    static std::shared_ptr<FkFFPacket> make(AVPacket *pkt, kMediaType type, AVRational avTimeBase);

public:
    FkFFPacket(AVPacket *pkt, kMediaType type, AVRational avTimeBase);

    virtual ~FkFFPacket();

    virtual FkTime getPts() const override;

    virtual FkTime getDts() const override;

    virtual FkTime getDuration() const override;

    virtual std::shared_ptr<FkBuffer> getBuffer() const override;

    virtual int32_t getFlags() const override;

    virtual kMediaType getType() const override;

    virtual std::shared_ptr<FkAbsPacket> clone() override;

    virtual void *getOrigin() const override;

    virtual FkSampleFormat getSampleFormat() const override;

private:
    AVPacket *pkt = nullptr;
    AVRational avTimeBase = AV_TIME_BASE_Q;
    kMediaType type = kMediaType::NONE;
};
