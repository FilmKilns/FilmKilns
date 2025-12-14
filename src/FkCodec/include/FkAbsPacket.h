//
// Created by lmy on 2023/10/14.
//
#pragma once

#include "FkObject.h"
#include "FkTime.h"
#include "FkBuffer.h"
#include "FkCodecDefinition.h"
#include "FkSampleFormat.h"

FK_SUPER_CLASS(FkAbsPacket, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsPacket)

public:
    static std::string toFlagsStr(int32_t flags);

public:
    FkAbsPacket();

    FkAbsPacket(const FkAbsPacket &o) = delete;

    virtual ~FkAbsPacket();

    virtual FkTime getPts() const = 0;

    virtual FkTime getDts() const = 0;

    virtual FkTime getDuration() const = 0;

    virtual std::shared_ptr<FkBuffer> getBuffer() const = 0;

    virtual int32_t getFlags() const = 0;

    virtual kMediaType getType() const = 0;

    virtual std::shared_ptr<FkAbsPacket> clone() = 0;

    virtual void *getOrigin() const = 0;

    virtual FkSampleFormat getSampleFormat() const = 0;

    std::string toString() override;
};
