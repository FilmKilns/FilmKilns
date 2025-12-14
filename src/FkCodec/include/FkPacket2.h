//
// Created by lmy on 2023/10/14.
//
#pragma once

#include "FkAbsPacket.h"

FK_SUPER_CLASS(FkPacket2, FkAbsPacket) {
FK_DEF_CLASS_TYPE_FUNC(FkPacket2)

public:
    static std::shared_ptr<FkPacket2> make(size_t bufSize, kMediaType type, FkTime pts);

    static std::shared_ptr<FkPacket2> make(const std::shared_ptr<FkBuffer> _buf, kMediaType type, FkTime pts);

public:
    FkPacket2();

    virtual ~FkPacket2();

    virtual FkTime getPts() const override;

    virtual FkTime getDts() const override;

    virtual FkTime getDuration() const override;

    void setDuration(FkTime _duration);

    virtual std::shared_ptr<FkBuffer> getBuffer() const override;

    virtual int32_t getFlags() const override;

    void setFlags(int32_t _flags);

    virtual kMediaType getType() const override;

    virtual std::shared_ptr<FkAbsPacket> clone() override;

    virtual void *getOrigin() const override;

    virtual FkSampleFormat getSampleFormat() const override;

private:
    FkTime pts;
    FkTime dts;
    FkTime duration;
    kMediaType type = kMediaType::NONE;
    std::shared_ptr<FkBuffer> buf = nullptr;
    int32_t flags = 0;
};
