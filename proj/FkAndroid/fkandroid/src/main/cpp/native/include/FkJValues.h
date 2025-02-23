/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-22 22:24:54
*/
#pragma once

#include "FkJniDefinition.h"

template<typename J, typename C>
class FkJValue {
public:
    FkJValue() {};

    FkJValue(const FkJValue &o) {};

    virtual ~FkJValue() {};

    J jVal() {
        return _jVal;
    };

    C cVal() {
        return _cVal;
    };

protected:
    J _jVal = nullptr;
    C _cVal;
};

class FkJString : public FkJValue<jstring, std::string> {
public:
    static std::shared_ptr<FkJString> create(JNIEnv *env, jstring obj);

    FkJString(JNIEnv *env, jstring obj);

    FkJString(const FkJString &o);

    virtual ~FkJString();
};

class FkJBuffer : public FkJValue<jbyteArray, std::shared_ptr<FkBuffer>> {
public:
    static std::shared_ptr<FkJBuffer> create(JNIEnv *env, jbyteArray obj);

    FkJBuffer(JNIEnv *env, jbyteArray obj);

    FkJBuffer(const FkJBuffer &o);

    virtual ~FkJBuffer();
};
