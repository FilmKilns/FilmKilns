/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-5 11:10:40
*/

#ifndef FK_GRAPHIC_FKNEWBMPTEXPROTO_H
#define FK_GRAPHIC_FKNEWBMPTEXPROTO_H

#include "FkNewTexProto.h"

FK_SUPER_CLASS(FkNewBmpTexProto, FkNewTexProto) {
FK_DEF_CLASS_TYPE_FUNC(FkNewBmpTexProto)

public:
    FkNewBmpTexProto();

    FkNewBmpTexProto(const FkNewBmpTexProto &o);

    virtual ~FkNewBmpTexProto();
};

#endif //FK_GRAPHIC_FKNEWBMPTEXPROTO_H