/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWFRAMEALLOCATOR_H
#define HARDWAREVIDEOCODEC_HWFRAMEALLOCATOR_H

#include "Object.h"
#include "HwSourcesAllocator.h"
#include "HwAbsMediaFrame.h"
#include "SimpleLock.h"
#include "Logcat.h"
#include <set>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
}
#endif

class HwFrameAllocator : public HwSourcesAllocator {
public:
    HwFrameAllocator();

    ~HwFrameAllocator();

    /**
     * Copy frame
     */
    HwAbsMediaFrame *ref(AVFrame *avFrame);

    /**
     * Copy frame
     */
    HwAbsMediaFrame *ref(HwAbsMediaFrame *src);

    HwAbsMediaFrame *ref(uint8_t *buf, int nbSample);

    bool recycle(HwSources **entity);

    void printInfo();

private:

    HwAbsMediaFrame *refVideo(AVFrame *avFrame);

    HwAbsMediaFrame *refAudio(AVFrame *avFrame);

    void copyInfo(HwAbsMediaFrame *dest, AVFrame *src);

    bool isRef(HwAbsMediaFrame *frame);

private:
    SimpleLock refLock;
    SimpleLock unRefLock;
    set<HwAbsMediaFrame *> refQueue;
    set<HwAbsMediaFrame *> unRefQueue;
    int count = 0;
};


#endif //HARDWAREVIDEOCODEC_HWFRAMEALLOCATOR_H
