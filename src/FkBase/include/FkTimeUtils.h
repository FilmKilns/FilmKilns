/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKTIMEUTILS_H
#define FK_BASE_FKTIMEUTILS_H

#include "FkObject.h"
#include <sys/time.h>

class FkTimeUtils {
public:
    struct Cost {
    public:
        Cost(const std::string &tag);
        ~Cost();
    private:
        std::string tag;
        int64_t startTime = 0;
    };

public:
    static int64_t getCurrentTimeUS();

    ~FkTimeUtils() {};

private:
    FkTimeUtils() = delete;

    FkTimeUtils(const FkTimeUtils &o) = delete;
};


#endif //FK_BASE_FKTIMEUTILS_H
