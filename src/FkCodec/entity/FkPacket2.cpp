//
// Created by lmy on 2023/10/14.
//

#include "FkPacket2.h"

FK_IMPL_CLASS_TYPE(FkPacket2, FkObject)

std::shared_ptr<FkPacket2> FkPacket2::make(size_t bufSize, kMediaType type, FkTime pts) {
    return make(FkBuffer::alloc(bufSize), type, pts);
}

std::shared_ptr<FkPacket2> FkPacket2::make(const std::shared_ptr<FkBuffer> _buf,
                                           kMediaType type,
                                           FkTime pts) {
    auto pkt = std::make_shared<FkPacket2>();
    pkt->buf = _buf;
    pkt->type = type;
    pkt->pts = pts;
    return pkt;
}

FkPacket2::FkPacket2() : FkAbsPacket(),
                         pts(0, {1, 1}),
                         dts(0, {1, 1}),
                         duration(0, {1, 1}) {
}


FkPacket2::~FkPacket2() {
}

FkTime FkPacket2::getPts() const {
    return pts;
}

FkTime FkPacket2::getDts() const {
    return dts;
}

FkTime FkPacket2::getDuration() const {
    return duration;
}

void FkPacket2::setDuration(FkTime _duration) {
    duration = _duration;
}

std::shared_ptr<FkBuffer> FkPacket2::getBuffer() const {
    return buf;
}

int32_t FkPacket2::getFlags() const {
    return flags;
}

void FkPacket2::setFlags(int32_t _flags) {
    flags = _flags;
}

kMediaType FkPacket2::getType() const {
    return type;
}

std::shared_ptr<FkAbsPacket> FkPacket2::clone() {
    auto _pkt = make(getBuffer()->size(), type, pts);
    _pkt->getBuffer()->put(getBuffer()->data(), getBuffer()->size());
    _pkt->getBuffer()->rewind();
    _pkt->dts = dts;
    _pkt->duration = duration;
    _pkt->flags = flags;
    _pkt->type = type;
    return _pkt;
}

void *FkPacket2::getOrigin() const {
    return nullptr;
}

FkSampleFormat FkPacket2::getSampleFormat() const {
    return FkSampleFormat::NONE;
}
