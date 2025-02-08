/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUSEQUENCE_H
#define HWVC_ANDROID_ALUSEQUENCE_H

#include "Unit.h"
#include "AlMediaTrack.h"
#include "AlIdentityCreator.h"
#include "AlVector.h"

al_def_unit(AlUSequence, Unit) {
public:
    AlUSequence(const std::string alias);

    virtual ~AlUSequence();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

private:
    bool _onHeartbeat(AlMessage *msg);

    bool _onAddTrack(AlMessage *msg);

    bool _onRemoveTrack(AlMessage *msg);

    bool _onSetTrackDuration(AlMessage *msg);

    bool _onSeek(AlMessage *msg);

    bool _onTimelineEnd(AlMessage *msg);

    AlMediaClip *_findClip(AlID id);

    AlMediaTrack *_findTrackByClip(AlID clipID);

    void _findClipsByTime(AlMediaTrack::kType type,
                          AlVector<std::shared_ptr<AlMediaClip>> &array,
                          int64_t timeInUS);

    void _notifyTimeline();

    void _beatAudioClips(int64_t timeInUS, bool isSeek = false);

    void _beatVideoClips(int64_t timeInUS, bool isSeek = false);

    void _notifyTrackUpdate(const AlMediaTrack *track);

private:
    AlIdentityCreator creator;
    std::map<int32_t, std::unique_ptr<AlMediaTrack>> tracks;
};


#endif //HWVC_ANDROID_ALUSEQUENCE_H
