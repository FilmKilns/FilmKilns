/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2022-4-19 13:35:25
*/

#include "FkRect.h"

FK_IMPL_CLASS_TYPE(FkRect, FkObject)
FK_IMPL_CLASS_TYPE(FkIntRect, FkRect)
FK_IMPL_CLASS_TYPE(FkFloatRect, FkRect)

FkIntRect::FkIntRect(int32_t left, int32_t top, int32_t right, int32_t bottom)
        : FkRect<int32_t>(left, top, right, bottom) {

}

FkIntRect::FkIntRect(const FkIntRect &o) : FkRect<int32_t>(o) {

}

FkIntRect::~FkIntRect() {

}

FkFloatRect::FkFloatRect(float left, float top, float right, float bottom)
        : FkRect<float>(left, top, right, bottom) {

}

FkFloatRect::FkFloatRect(const FkFloatRect &o) : FkRect<float>(o) {

}

FkFloatRect::~FkFloatRect() {

}