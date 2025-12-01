/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFloatVec2.h"

FK_IMPL_CLASS_TYPE(FkFloatVec2, FkVec2)

FkFloatVec2::FkFloatVec2(float x, float y) : FkVec2<float>(x, y) {

}

FkFloatVec2::FkFloatVec2(const FkFloatVec2 &o) : FkVec2<float>(o) {

}

FkFloatVec2::~FkFloatVec2() {

}

bool FkFloatVec2::isNan() {
    return isnan(x) || isnan(y);
}