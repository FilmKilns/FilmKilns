//
// Created by lmy on 2023/10/13.
//
#pragma once

#include "FkObject.h"
#include "FkCodecDefinition.h"
#include "FkTime.h"
#include "FkAbsPacket.h"
#include <any>

class FkFFMediaContext;

FK_SUPER_CLASS(FkMediaTrack, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkMediaTrack)
public:
    struct Desc {
        int trackId = -1;
        kMediaType type = kMediaType::NONE;
        FkTime duration = FkTime::zero();
    };

public:
    FkMediaTrack(const std::shared_ptr<FkFFMediaContext> &context, int trackId);

    virtual ~FkMediaTrack();

    FkMediaTrack(const FkMediaTrack &o) = delete;

    virtual std::shared_ptr<FkAbsPacket> grab(const std::shared_ptr<FkAbsPacket> &input) = 0;
};
