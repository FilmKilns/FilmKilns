//
// Created by lmy on 2023/9/27.
//
#pragma once

#include "FkObject.h"
#include "FkFFUtils.h"
#include "FkFFMediaTrack.h"
#include "FkAbsPacket.h"
#include <map>

FK_SUPER_CLASS(FkFFMediaContext, FkObject), public std::enable_shared_from_this<FkFFMediaContext> {
FK_DEF_CLASS_TYPE_FUNC(FkFFMediaContext)

    friend class FkFFMediaTrack;

public:
    FkFFMediaContext(const std::string &path);

    FkFFMediaContext(const FkFFMediaContext &o) = delete;

    virtual ~FkFFMediaContext();

    std::vector<FkMediaTrack::Desc> getTracks();

    std::shared_ptr<FkMediaTrack> openTrack(int trackId);

    std::shared_ptr<FkAbsPacket> grab();

    std::shared_ptr<FkAbsPacket> grab(int trackId);

    FkResult seek(int64_t timeInUS);

private:
    void _destroy();

    void _checkBsf(int trackId);

    FkMediaTrack::Desc _getVideoTrack();

private:
    AVCodecParameters *_getCodecParameters(int trackId);

private:
    AVFormatContext *ctx = nullptr;
    std::string path;
    AVPacket *avPacket = nullptr;
    std::map<int32_t, bool> cnfGrabStatus;
    AVBSFContext *bsfContext = nullptr;
};

