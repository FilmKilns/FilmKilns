/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAbsVideoEncoder.h"

FK_IMPL_CLASS_TYPE(FkAbsVideoEncoder, FkAbsEncoder)

FkAbsVideoEncoder::FkAbsVideoEncoder(const FkAbsEncoder::Desc &desc) : FkAbsEncoder(desc) {

}

FkAbsVideoEncoder::~FkAbsVideoEncoder() {

}

FkResult FkAbsVideoEncoder::addMuxer(std::string path, const std::shared_ptr<FkBundle> &bundle, std::function<void(int64_t, int byteSize)> callback) {
    return FK_FAIL;
}

FkResult FkAbsVideoEncoder::removeMuxer(std::string path) {
    return FK_FAIL;
}