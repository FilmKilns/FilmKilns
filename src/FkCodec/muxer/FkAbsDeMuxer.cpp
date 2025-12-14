/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAbsDeMuxer.h"

FK_IMPL_CLASS_TYPE(FkAbsDeMuxer, FkObject)

FkAbsDeMuxer::FkAbsDeMuxer() : FkObject() {

}

FkAbsDeMuxer::~FkAbsDeMuxer() {

}

FkResult FkAbsDeMuxer::configure(const std::string &_filePath, const std::string &_type) {
    this->filePath = _filePath;
    this->type = _type;
    return FK_OK;
}