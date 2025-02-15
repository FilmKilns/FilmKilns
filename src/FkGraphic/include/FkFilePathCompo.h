/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2022-3-12 9:37:18
*/

#ifndef FK_GRAPHIC_FKFILEPATHCOMPO_H
#define FK_GRAPHIC_FKFILEPATHCOMPO_H

#include "FkStrCompo.h"

FK_SUPER_CLASS(FkFilePathCompo, FkStrCompo) {
FK_DEF_CLASS_TYPE_FUNC(FkFilePathCompo)

public:
    FkFilePathCompo(std::string &str);

    FkFilePathCompo(const FkFilePathCompo &o);

    virtual ~FkFilePathCompo();
};

#endif //FK_GRAPHIC_FKFILEPATHCOMPO_H