/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAbsMuxer.h"

FK_IMPL_CLASS_TYPE(FkAbsMuxer, FkObject)

const std::string FkAbsMuxer::TYPE_MP4 = "MP4";
const std::string FkAbsMuxer::TYPE_FLV = "FLV";
const std::string FkAbsMuxer::TYPE_TS = "TS";
const int32_t FkAbsMuxer::TRACK_NONE = -1;

FkAbsMuxer::FkAbsMuxer() : FkObject() {

}

FkAbsMuxer::~FkAbsMuxer() {

}

FkResult FkAbsMuxer::configure(std::string _filePath, std::string _type) {
    this->filePath = _filePath;
    this->type = _type;
    return FK_OK;
}