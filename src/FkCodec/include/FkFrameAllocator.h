/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_CODEC_FKFRAMEALLOCATOR_H
#define FK_CODEC_FKFRAMEALLOCATOR_H

#include "FkObject.h"
#include "FkSourcesAllocator.h"
#include "FkAbsMediaFrame.h"
#include "FkSimpleLock.h"
#include <set>


#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
}
#endif

FK_SUPER_CLASS(FkFrameAllocator, FkSourcesAllocator) {
FK_DEF_CLASS_TYPE_FUNC(FkFrameAllocator)

public:
    FkFrameAllocator();

    virtual ~FkFrameAllocator();

    /**
     * Copy frame
     */
    FkAbsMediaFrame *ref(AVFrame *avFrame);

    /**
     * Copy frame
     */
    FkAbsMediaFrame *ref(FkAbsMediaFrame *src);

    FkAbsMediaFrame *ref(uint8_t *buf, int nbSample);

    bool recycle(FkSources **entity) override;

    void printInfo();

private:

    FkAbsMediaFrame *refVideo(AVFrame *avFrame);

    FkAbsMediaFrame *refAudio(AVFrame *avFrame);

    void copyInfo(FkAbsMediaFrame *dest, AVFrame *src);

    bool isRef(FkAbsMediaFrame *frame);

private:
    FkSimpleLock refLock;
    FkSimpleLock unRefLock;
    std::set<FkAbsMediaFrame *> refQueue;
    std::set<FkAbsMediaFrame *> unRefQueue;
    int count = 0;
};


#endif //FK_CODEC_FKFRAMEALLOCATOR_H
