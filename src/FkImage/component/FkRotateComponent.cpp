/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRotateComponent.h"

FK_IMPL_CLASS_TYPE(FkRotateComponent, FkGraphicComponent)

FkRotateComponent::FkRotateComponent() : FkGraphicComponent() {


}

FkRotateComponent::FkRotateComponent(const FkRotateComponent &o)
        : FkGraphicComponent(o), value(o.value) {


}

FkRotateComponent::~FkRotateComponent() {

}
