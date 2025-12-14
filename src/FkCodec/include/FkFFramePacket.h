//
// Created by lmy on 2023/10/15.
//
#pragma once

#include "FkAbsPacket.h"
#include "FkCodecDefinition.h"

FK_SUPER_CLASS(FkFFramePacket, FkAbsPacket) {
FK_DEF_CLASS_TYPE_FUNC(FkFFramePacket)

public:
    static std::shared_ptr<FkFFramePacket> make(AVFrame *frame, kMediaType type, AVRational avTimeBase);

    static std::shared_ptr<FkFFramePacket> makeReplace(AVFrame *frame, const std::shared_ptr<FkFFramePacket> &src);

public:
    FkFFramePacket();

    virtual ~FkFFramePacket();

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
    AVFrame *frame = nullptr;
    AVRational avTimeBase = AV_TIME_BASE_Q;
    kMediaType type = kMediaType::NONE;
};
