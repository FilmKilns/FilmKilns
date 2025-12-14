/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkPacket.h"
#include "FkCodecDefinition.h"
#include "FkString.h"

FK_IMPL_CLASS_TYPE(FkPacket, FkObject)

std::shared_ptr<FkPacket> FkPacket::wrap2(AVPacket *pkt, const AVRational time_base) {
    auto p = new FkPacket();
    p->buf = FkBuffer::wrap(pkt->data, pkt->size);
    AVRational rational{1, 1000000};
    p->pts = av_rescale_q_rnd(pkt->pts,
                              time_base,
                              rational,
                              AV_ROUND_NEAR_INF);
    p->dts = av_rescale_q_rnd(pkt->dts,
                              time_base,
                              rational,
                              AV_ROUND_NEAR_INF);
    p->duration = av_rescale_q_rnd(pkt->duration,
                                   time_base,
                                   rational,
                                   AV_ROUND_NEAR_INF);
    p->flags = pkt->flags;
    return std::shared_ptr<FkPacket>(p);
}

std::shared_ptr<FkPacket> FkPacket::wrap2(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags) {
    return std::shared_ptr<FkPacket>(FkPacket::wrap(buf, size, pts, dts, flags));
}

std::shared_ptr<FkPacket> FkPacket::wrap(const std::shared_ptr<FkBuffer> &_buf, int64_t _pts, int64_t _dts, int32_t _flags) {
    FkPacket *p = new FkPacket();
    p->buf = _buf;
    p->pts = _pts;
    p->dts = _dts;
    p->duration = 1;
    p->flags = _flags;
    return std::shared_ptr<FkPacket>(p);
}

FkPacket *FkPacket::wrap(AVPacket *pkt) {
    FkPacket *p = new FkPacket();
    p->buf = FkBuffer::wrap(pkt->data, pkt->size);
    p->pts = pkt->pts;
    p->dts = pkt->dts;
    p->duration = pkt->duration;
    p->flags = pkt->flags;
    return p;
}

FkPacket *FkPacket::wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags) {
    FkPacket *p = new FkPacket();
    if (buf && size > 0) {
        p->buf = FkBuffer::wrap(buf, size);
    }
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = flags;
    return p;
}

FkPacket *FkPacket::create(size_t size, int64_t pts, int64_t dts) {
    FkPacket *p = new FkPacket();
    p->buf = FkBuffer::alloc(size);
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = 0;
    return p;
}

std::shared_ptr<FkPacket> FkPacket::clone(FkPacket *pkt) {
    auto p = std::shared_ptr<FkPacket>(
            FkPacket::create(pkt->buf->size(), pkt->getPts(), pkt->getDts()));
    p->buf->put(pkt->data(), pkt->size());
    p->buf->rewind();
    p->pts = pkt->pts;
    p->dts = pkt->dts;
    p->duration = pkt->duration;
    p->flags = pkt->flags;
    p->type = pkt->type;
    return p;
}

std::shared_ptr<FkPacket> FkPacket::clone(const std::shared_ptr<FkPacket> &pkt) {
    return clone(pkt.get());
}

FkPacket::FkPacket() : FkObject() {

}

FkPacket::~FkPacket() {
    buf = nullptr;
    pts = INT64_MIN;
    dts = INT64_MIN;
    duration = INT64_MIN;
    flags = 1;
    type = kMediaType::NONE;
}

bool FkPacket::ref(AVPacket **pkt) {
    if (!pkt || !*pkt) {
        return false;
    }
    (*pkt)->data = data();
    (*pkt)->size = size();
    (*pkt)->pts = getPts();
    (*pkt)->dts = getDts();
    (*pkt)->duration = getDuration();
    (*pkt)->flags = flags;
    return true;
}

uint8_t *FkPacket::data()  { return buf->data(); }

size_t FkPacket::size() const { return buf->size(); }

int64_t FkPacket::getPts() const { return pts; }

int64_t FkPacket::getDts() const { return dts; }

int64_t FkPacket::getDuration() const { return duration; }

void FkPacket::setDuration(int64_t _duration) { this->duration = _duration; }

int32_t FkPacket::getFlags() const {
    return flags;
}

void FkPacket::setType(kMediaType _type) {
    type = _type;
}

kMediaType FkPacket::getType() const {
    return type;
}

std::string FkPacket::getFlagsStr() const {
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

std::string FkPacket::toString() {
    FkString str("Flags=");
    str.append(getFlagsStr());
    str.append(", Data(0,4)=");
    str.append(data()[0]);
    str.append(",");
    str.append(data()[1]);
    str.append(",");
    str.append(data()[2]);
    str.append(",");
    str.append(data()[3]);
    return str.str();
}
