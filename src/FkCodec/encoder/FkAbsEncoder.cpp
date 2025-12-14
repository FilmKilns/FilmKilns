/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkAbsEncoder.h"

FK_IMPL_CLASS_TYPE(FkAbsEncoder, FkObject)

FkAbsEncoder::Desc::Desc()
        : vID(FkCodec::kID::NONE), aID(FkCodec::kID::NONE),
          type(FkAbsEncoder::kType::SOFT) {

}

FkAbsEncoder::Desc::Desc(FkCodec::kID id, FkAbsEncoder::kType type)
        : vID(id), aID(FkCodec::kID::AAC), type(type) {

}

FkAbsEncoder::Desc::Desc(const Desc &o) : vID(o.vID), aID(o.aID), type(o.type) {

}

FkAbsEncoder::Desc::~Desc() {

}

FkAbsEncoder::FkAbsEncoder(const FkAbsEncoder::Desc &desc) : FkObject(), desc(desc) {

}

FkAbsEncoder::~FkAbsEncoder() {

}

FkAbsEncoder::Desc FkAbsEncoder::getCodecDesc() {
    return desc;
}
