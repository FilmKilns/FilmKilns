/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)

* CREATE TIME: 2024-02-25 15:36:45
*/

#include "FkLayerSetProjectionProto.h"

FK_IMPL_CLASS_TYPE(FkLayerSetProjectionProto, FkProtocol)

FkLayerSetProjectionProto::FkLayerSetProjectionProto(FkID srcLayerId, FkID dstLayerId)
        : FkProtocol(), srcLayerId(srcLayerId), dstLayerId(dstLayerId) {

}

FkLayerSetProjectionProto::FkLayerSetProjectionProto(const FkLayerSetProjectionProto &o)
        : FkProtocol(o), srcLayerId(o.srcLayerId), dstLayerId(o.dstLayerId) {

}

FkLayerSetProjectionProto::~FkLayerSetProjectionProto() {

}