//
// Created by lmy on 2023/10/14.
//

#include "FkFFPacket.h"

FK_IMPL_CLASS_TYPE(FkFFPacket, FkObject)

std::shared_ptr<FkFFPacket> FkFFPacket::make(AVPacket *pkt,
                                             kMediaType type,
                                             AVRational avTimeBase) {
    return std::make_shared<FkFFPacket>(pkt, type, avTimeBase);
}

FkFFPacket::FkFFPacket(AVPacket *pkt, kMediaType type, AVRational avTimeBase) : FkAbsPacket() {
    this->pkt = av_packet_alloc();
    this->type = type;
    this->avTimeBase = avTimeBase;
    av_packet_ref(this->pkt, pkt);
}

FkFFPacket::~FkFFPacket() {
    av_packet_unref(this->pkt);
    this->pkt = nullptr;
}

FkTime FkFFPacket::getPts() const {
    return {this->pkt->pts, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

FkTime FkFFPacket::getDts() const {
    return {this->pkt->dts, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

FkTime FkFFPacket::getDuration() const {
    return {this->pkt->duration, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

std::shared_ptr<FkBuffer> FkFFPacket::getBuffer() const {
    return FkBuffer::wrap(this->pkt->data, this->pkt->size);
}

int32_t FkFFPacket::getFlags() const {
    return this->pkt->flags;
}

kMediaType FkFFPacket::getType() const {
    return type;
}

std::shared_ptr<FkAbsPacket> FkFFPacket::clone() {
    auto _pkt = av_packet_clone(this->pkt);
    auto result = std::make_shared<FkFFPacket>(_pkt, type, avTimeBase);
    av_packet_unref(_pkt);
    return result;
}

void *FkFFPacket::getOrigin() const {
    return pkt;
}

FkSampleFormat FkFFPacket::getSampleFormat() const {
    return FkSampleFormat::NONE;
}
