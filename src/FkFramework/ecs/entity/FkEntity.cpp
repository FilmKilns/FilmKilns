/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:49
*/

#include "FkEntity.h"

FK_IMPL_CLASS_TYPE(FkEntity, FkObject)

FkEntity::FkEntity() : FkObject() {


}

FkEntity::FkEntity(const FkEntity &o) : FkObject() {
    for (auto &itr: o.components) {
        components.insert(std::make_pair(itr.first, itr.second));
    }
}

FkEntity::~FkEntity() {
    components.clear();
}

FkResult FkEntity::addComponent(const std::shared_ptr<FkComponent> &comp) {
    FkAssert(comp != nullptr, FK_NPE);
    components[comp->getClassType().getId()] = comp;
#if defined(__FK_DEBUG__)
    toString();
#endif
    return FK_OK;
}

FkResult FkEntity::addComponents(const std::vector<std::shared_ptr<FkComponent>> &vec) {
    for (auto &compo: vec) {
        addComponent(compo);
    }
#if defined(__FK_DEBUG__)
    toString();
#endif
    return FK_OK;
}

FkResult FkEntity::findComponents(std::vector<std::shared_ptr<FkComponent>> &vec,
                                  const FkClassType &classType) {
    for (auto &itr: components) {
        if (itr.second->getClassType().is(classType)) {
            vec.emplace_back(itr.second);
        }
    }
    return vec.empty() ? FK_FAIL : FK_OK;
}

FkResult FkEntity::copyComponentFrom(const std::shared_ptr<FkEntity> &src, const FkClassType &classType) {
    FkAssert(src != nullptr, FK_NPE);
    std::vector<std::shared_ptr<FkComponent>> vec;
    if (FK_OK == src->findComponents(vec, classType)) {
        auto compo = vec[0]->clone();
        if (compo == nullptr) {
            compo = vec[0];
        }
        return addComponent(compo);
    }
    return FK_SOURCE_NOT_FOUND;
}

std::string FkEntity::toString() {
    desc.clear();
    desc.append("[");
    for (auto &itr: components) {
        desc.append(itr.second->getClassType().getName());
        desc.append(",");
    }
    desc.append("]");
    return desc;
}