/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:35
*/

#include "FkComponent.h"

FK_IMPL_CLASS_TYPE(FkComponent, FkObject)

FkComponent::FkComponent() : FkObject() {

}

FkComponent::FkComponent(const FkComponent &o) : FkObject(o) {

}

FkComponent::~FkComponent() {

}

std::shared_ptr<FkComponent> FkComponent::clone() {
    return nullptr;
}