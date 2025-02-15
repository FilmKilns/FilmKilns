/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkWinSizeProto.h"

FK_IMPL_CLASS_TYPE(FkWinSizeProto, FkProtocol)

FkWinSizeProto::FkWinSizeProto() : FkProtocol(), winSize(0, 0) {

}

FkWinSizeProto::FkWinSizeProto(const FkWinSizeProto &o) : FkProtocol(o), winSize(o.winSize) {

}

FkWinSizeProto::~FkWinSizeProto() {

}