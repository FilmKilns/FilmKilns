/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKFFMUXER_H
#define FK_CODEC_FKFFMUXER_H

#include "FkAbsMuxer.h"
#include <vector>

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

FK_SUPER_CLASS(FkFFMuxer, FkAbsMuxer) {
FK_DEF_CLASS_TYPE_FUNC(FkFFMuxer)

private:
    enum class kState : int {
        IDL = 0,
        CONFIGURED,
        RUNNING
    };
public:
    FkFFMuxer();

    virtual ~FkFFMuxer();

    virtual FkResult configure(std::string _filePath, std::string _type) override;

    virtual FkResult start() override;

    virtual FkResult stop() override;

    virtual int32_t addTrack(std::shared_ptr<FkBundle> format) override;

    /**
     * @param packet AvPacket
     */
    virtual FkResult write(int32_t track, FkPacket *pkt) override;

private:
    void release();

    bool _configure(int32_t track, FkPacket *pkt);

    void setInt32Parameter(int32_t &param, int32_t value);

    void setInt64Parameter(int64_t &param, int64_t value);

    void _createFilter(AVStream *stream);

    void _destroyFilter();

private:
    kState state = kState::IDL;
    AVFormatContext *pFormatCtx = nullptr;
    std::vector<AVStream *> tracks;
    std::vector<bool> mTrackConfigured;
    AVPacket *avPacket = nullptr;
    int32_t rotation = 0;
    bool enableFastStart = false;
    AVBSFContext *bsfContext = nullptr;
};

#endif //FK_CODEC_FKFFMUXER_H
