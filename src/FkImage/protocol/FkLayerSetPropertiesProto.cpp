/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2025-05-03 16:33:00
*/

#include "FkLayerSetPropertiesProto.h"

FK_IMPL_CLASS_TYPE(FkLayerSetPropertiesProto, FkProtocol)

FkLayerSetPropertiesProto::FkLayerSetPropertiesProto() : FkProtocol(), properties({}) {

}

FkLayerSetPropertiesProto::FkLayerSetPropertiesProto(const FkLayerSetPropertiesProto &o)
        : FkProtocol(o), layerId(o.layerId), properties(o.properties) {

}

FkLayerSetPropertiesProto::~FkLayerSetPropertiesProto() {

}