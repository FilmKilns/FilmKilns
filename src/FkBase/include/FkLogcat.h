/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2022-3-26 22:10:11
*/

#ifndef FK_BASE_FKLOGCAT_H
#define FK_BASE_FKLOGCAT_H

#include <string>

class FkLogcat {
public:
    virtual ~FkLogcat() {};

    static void v(const std::string &TAG, const std::string fmt, ...);

    static void d(const std::string &TAG, const std::string fmt, ...);

    static void i(const std::string &TAG, const std::string fmt, ...);

    static void e(const std::string &TAG, const std::string fmt, ...);

    static void w(const std::string &TAG, const std::string fmt, ...);

private:
    FkLogcat() {};

    FkLogcat(const FkLogcat &o) = delete;

};

#endif //FK_BASE_FKLOGCAT_H