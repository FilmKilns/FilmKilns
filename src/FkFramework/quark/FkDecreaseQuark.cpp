/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkDecreaseQuark.h"
#include "FkCalculatePrt.h"

FK_IMPL_CLASS_TYPE(FkDecreaseQuark, FkQuark)

FkDecreaseQuark::FkDecreaseQuark() : FkQuark() {

}

FkDecreaseQuark::~FkDecreaseQuark() {

}

void FkDecreaseQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkCalculatePrt, FkDecreaseQuark::_onDecrease);

}

FkResult FkDecreaseQuark::_onDecrease(std::shared_ptr<FkProtocol> p) {
    auto cp = std::static_pointer_cast<FkCalculatePrt>(p);
    cp->number -= 1;
    return FK_OK;
}