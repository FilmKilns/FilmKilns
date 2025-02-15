/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-13 13:23:03
*/

#ifndef FK_IMAGE_FKLAYERSETROTATEPROTO_H
#define FK_IMAGE_FKLAYERSETROTATEPROTO_H

#include "FkRationalProto.h"

FK_SUPER_CLASS(FkLayerSetRotateProto, FkRationalProto) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerSetRotateProto)

public:
    FkLayerSetRotateProto();

    FkLayerSetRotateProto(const FkLayerSetRotateProto &o);

    virtual ~FkLayerSetRotateProto();

public:
    FkID layer = FK_ID_NONE;
};

#endif //FK_IMAGE_FKLAYERSETROTATEPROTO_H