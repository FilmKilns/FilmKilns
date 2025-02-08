/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMemory.h"

FK_IMPL_CLASS_TYPE(FkMemory, FkObject)

FkMemory::FkMemory() : FkObject() {

}

FkMemory::FkMemory(const FkMemory &o) : FkObject(o) {

}

FkMemory::~FkMemory() {

}