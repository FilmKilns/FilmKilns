/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKPORTDESC_H
#define FK_FRAMEWORK_FKPORTDESC_H

#include "FkPort.h"
#include <unordered_map>
#include <list>
#include <mutex>

#define FK_PORT_DESC_ADD(desc, port, PRT, method) \
    desc->add(port, std::static_pointer_cast<FkProtocol>(std::make_shared<PRT>()), \
        reinterpret_cast<FkPort::PortFunc>(&method)) \

#define FK_PORT_DESC_QUICK_ADD(desc, PRT, method) \
    desc->add(0, {FK_CLASS_TYPE(PRT).getId(), FK_CLASS_TYPE(PRT).getName()}, \
        reinterpret_cast<FkPort::PortFunc>(&method)) \

#define FK_PORT_DELIVERY(desc, PRT, CLASS) \
    desc->add(0, std::static_pointer_cast<FkProtocol>(std::make_shared<PRT>()), \
        reinterpret_cast<FkPort::PortFunc>(&CLASS::dispatchNext)) \

FK_SUPER_CLASS(FkPortDesc, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkPortDesc)

public:
    FkPortDesc();

    FkPortDesc(const FkPortDesc &o) = delete;

    virtual ~FkPortDesc();

    FkResult add(uint32_t port, FkProtocol::Desc desc, FkPort::PortFunc func);

    std::shared_ptr<FkPort> find(FkProtocol::Type type);

    FkResult query(std::list<FkProtocol::Desc> &protocols);

    void clear();

private:
    std::unordered_map<FkProtocol::Type, std::shared_ptr<FkPort>> ports;
    std::mutex mtx;
};


#endif //FK_FRAMEWORK_FKPORTDESC_H
