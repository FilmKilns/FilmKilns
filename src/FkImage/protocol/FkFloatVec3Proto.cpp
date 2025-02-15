/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 20:28:34
*/

#include "FkFloatVec3Proto.h"

FK_IMPL_CLASS_TYPE(FkFloatVec3Proto, FkProtocol)

FkFloatVec3Proto::FkFloatVec3Proto()
        : FkProtocol(), value(0.0f, 0.0f, 0.0f) {

}

FkFloatVec3Proto::FkFloatVec3Proto(const FkFloatVec3Proto &o)
        : FkProtocol(o), value(o.value) {

}

FkFloatVec3Proto::~FkFloatVec3Proto() {

}