/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_CODEC_FKABSFRAME_H
#define FK_CODEC_FKABSFRAME_H

#include "FkObject.h"
#include "FkBuffer.h"

FK_SUPER_CLASS(FkAbsFrame, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsFrame)
public:
    FkAbsFrame(size_t size);

    virtual ~FkAbsFrame();

    virtual uint8_t *data();

    virtual size_t size();

private:
    std::shared_ptr<FkBuffer> buf = nullptr;
};


#endif //FK_CODEC_FKABSFRAME_H
