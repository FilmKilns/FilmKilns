/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 23:50:58
*/

#include "FkEmptyQuark.h"

FK_IMPL_CLASS_TYPE(FkEmptyQuark, FkQuark)

FkEmptyQuark::FkEmptyQuark() : FkQuark() {

}

FkEmptyQuark::~FkEmptyQuark() {

}

void FkEmptyQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}