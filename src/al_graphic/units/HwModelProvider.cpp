/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/Unit.h"
#include "HwPair.h"

const int HwModelProvider::EVENT_PUT_INT32 = KID('M', 'O', 'P', 0x01);
const int HwModelProvider::EVENT_PUT_INT64 = KID('M', 'O', 'P', 0x02);
const int HwModelProvider::EVENT_PUT_STRING = KID('M', 'O', 'P', 0x03);
const int HwModelProvider::EVENT_PUT_OBJECT = KID('M', 'O', 'P', 0x04);

HwModelProvider::HwModelProvider(string alias) : Unit(alias) {
    registerEvent(EVENT_PUT_INT32, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutInt32));
    registerEvent(EVENT_PUT_INT64, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutInt64));
    registerEvent(EVENT_PUT_STRING, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutString));
    registerEvent(EVENT_PUT_OBJECT, reinterpret_cast<EventFunc>(&HwModelProvider::eventPutObject));
}

HwModelProvider::~HwModelProvider() {

}

bool HwModelProvider::onDestroy(AlMessage *msg) {
    return true;
}

bool HwModelProvider::eventPutInt32(AlMessage *msg) {
    HwPair<string, int32_t> *p = static_cast<HwPair<string, int32_t> *>(msg->obj);
    if (p) {
        bundle.putInt32(p->key(), p->value());
    }
    return true;
}

bool HwModelProvider::eventPutInt64(AlMessage *msg) {
    HwPair<string, int64_t> *p = static_cast<HwPair<string, int64_t> *>(msg->obj);
    if (p) {
        bundle.putInt64(p->key(), p->value());
    }
    return true;
}

bool HwModelProvider::eventPutString(AlMessage *msg) {
    HwPair<string, string> *p = static_cast<HwPair<string, string> *>(msg->obj);
    if (p) {
        bundle.putString(p->key(), p->value());
    }
    return true;
}

bool HwModelProvider::eventPutObject(AlMessage *msg) {
    HwPair<string, Object *> *p = static_cast<HwPair<string, Object *> *>(msg->obj);
    if (p) {
        bundle.putObject(p->key(), p->value());
    }
    return true;
}

const int32_t HwModelProvider::getInt32(string key) {
    return bundle.getInt32(key);
}

const int64_t HwModelProvider::getInt64(string key) {
    return bundle.getInt64(key);
}

const string HwModelProvider::getString(string key) {
    return bundle.getString(key);
}

Object *HwModelProvider::getObject(string key) {
    return bundle.getObject(key);
}

bool HwModelProvider::onCreate(AlMessage *msg) {
    return true;
}
