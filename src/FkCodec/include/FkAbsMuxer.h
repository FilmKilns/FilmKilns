/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKABSMUXER_H
#define FK_CODEC_FKABSMUXER_H

#include "FkObject.h"
#include "FkCodec.h"
#include "FkPacket.h"

FK_SUPER_CLASS(FkAbsMuxer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsMuxer)

public:
    FkAbsMuxer();

    virtual ~FkAbsMuxer();

    virtual FkResult configure(std::string _filePath, std::string _type);

    virtual FkResult start()=0;

    virtual FkResult stop() = 0;

    /**
     * add an audio or video track.
     * @return audio or video track id
     */
    virtual int32_t addTrack(std::shared_ptr<FkBundle> format) = 0;

    /**
     * @param track track id
     * @return Hw::SUCCESS for success
     */
    virtual FkResult write(int32_t track, FkPacket *pkt)=0;

protected:
    std::string filePath;
    std::string type;

public:
    static const std::string TYPE_MP4;
    static const std::string TYPE_FLV;
    static const std::string TYPE_TS;
    static const int32_t TRACK_NONE;
};


#endif //FK_CODEC_FKABSMUXER_H
