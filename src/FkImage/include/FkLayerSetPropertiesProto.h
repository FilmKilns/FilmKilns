/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2025-05-03 16:33:00
*/

#ifndef FK_IMAGE_FKLAYERSETPROPERTIESPROTO_H
#define FK_IMAGE_FKLAYERSETPROPERTIESPROTO_H

#include "FkProtocol.h"
#include "FkValue.h"
#include <unordered_map>

FK_SUPER_CLASS(FkLayerSetPropertiesProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerSetPropertiesProto)

public:
    FkLayerSetPropertiesProto();

    FkLayerSetPropertiesProto(const FkLayerSetPropertiesProto &o);

    virtual ~FkLayerSetPropertiesProto();

public:
    FkID layerId = FK_ID_NONE;
    std::unordered_map<std::string, FkValue> properties;
};

#endif //FK_IMAGE_FKLAYERSETPROPERTIESPROTO_H