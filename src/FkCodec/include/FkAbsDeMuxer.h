/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "FkObject.h"

FK_SUPER_CLASS(FkAbsDeMuxer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsDeMuxer)

public:
    FkAbsDeMuxer();

    virtual ~FkAbsDeMuxer();

    virtual FkResult configure(const std::string &filePath, const std::string &type);

    virtual FkResult start() = 0;

protected:
    std::string filePath;
    std::string type;
};

