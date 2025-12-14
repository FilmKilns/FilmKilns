//
// Created by lmy on 2023/10/14.
//

#include "FkAbsPacket.h"
#include "FkString.h"

FK_IMPL_CLASS_TYPE(FkAbsPacket, FkObject)

FkAbsPacket::FkAbsPacket() {

}

FkAbsPacket::~FkAbsPacket() {

}

std::string FkAbsPacket::toString() {
    FkString str("Flags=");
    str.append(toFlagsStr(getFlags()));
    str.append(", Data(0,4)=");
    str.append(getBuffer()->data()[0]);
    str.append(",");
    str.append(getBuffer()->data()[1]);
    str.append(",");
    str.append(getBuffer()->data()[2]);
    str.append(",");
    str.append(getBuffer()->data()[3]);
    return str.str();
}

std::string FkAbsPacket::toFlagsStr(int32_t flags) {
    std::string str;
    if(flags & FK_CTL_FLAG_CONFIG) {
        str.append("C");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_DISPOSABLE) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_TRUSTED) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_DISCARD) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_CORRUPT) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_KEY) {
        str.append("K");
    } else {
        str.append("_");
    }
    return str;
}
