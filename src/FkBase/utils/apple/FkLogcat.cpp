/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2022-3-26 22:10:11
*/

#include "FkLogcat.h"

void FkLogcat::v(const std::string &TAG, const std::string fmt, ...) {
#ifdef __AL_DEBUG__
    va_list args;
    va_start(args, fmt);
    vprintf(fmt.c_str(), args);
    va_end(args);
#endif
}

void FkLogcat::d(const std::string &TAG, const std::string fmt, ...) {
#ifdef __AL_DEBUG__
    va_list args;
    va_start(args, fmt);
    vprintf(fmt.c_str(), args);
    va_end(args);
#endif
}

void FkLogcat::i(const std::string &TAG, const std::string fmt, ...) {
#ifdef __AL_DEBUG__
    va_list args;
    va_start(args, fmt);
    vprintf(fmt.c_str(), args);
    va_end(args);
#endif
}

void FkLogcat::e(const std::string &TAG, const std::string fmt, ...) {
#ifdef __AL_DEBUG__
    va_list args;
    va_start(args, fmt);
    vprintf(fmt.c_str(), args);
    va_end(args);
#endif
}

void FkLogcat::w(const std::string &TAG, const std::string fmt, ...) {
#ifdef __AL_DEBUG__
    va_list args;
    va_start(args, fmt);
    vprintf(fmt.c_str(), args);
    va_end(args);
#endif
}