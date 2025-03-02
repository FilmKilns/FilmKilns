/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:49
*/

#ifndef FK_FRAMEWORK_FKENTITY_H
#define FK_FRAMEWORK_FKENTITY_H

#include "FkObject.h"
#include "FkComponent.h"
#include <vector>
#include <map>

#define FK_FIND_COMPO(ENTITY, NAME) \
ENTITY->findComponent<NAME>(FK_CLASS_TYPE(NAME))                            \

FK_SUPER_CLASS(FkEntity, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkEntity)

public:
    FkEntity();

    FkEntity(const FkEntity &o);

    virtual ~FkEntity();

    FkResult addComponent(const std::shared_ptr<FkComponent> &comp);

    FkResult addComponents(const std::vector<std::shared_ptr<FkComponent>> &vec);

    FkResult findComponents(std::vector<std::shared_ptr<FkComponent>> &vec,
                            const FkClassType &classType);

    template<class T>
    std::shared_ptr<T> findComponent(const FkClassType &classType) {
        std::vector<std::shared_ptr<FkComponent>> vec;
        if (FK_OK != findComponents(vec, classType)) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<T>(vec[0]);
    }

    FkResult copyComponentFrom(const std::shared_ptr<FkEntity> &src, const FkClassType &classType);

    virtual std::string toString() override;

private:
    std::map<size_t, std::shared_ptr<FkComponent>> components;
    std::string desc = "";
};

#endif //FK_FRAMEWORK_FKENTITY_H