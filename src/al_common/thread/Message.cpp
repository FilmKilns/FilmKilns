/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Message.h"
#include "../include/ObjectBox.h"

Message::Message(int32_t what, Runnable runnable)
        : Message(what, nullptr, QUEUE_MODE_NORMAL, runnable) {
}

Message::Message(int32_t what, Object *obj, Runnable runnable)
        : Message(what, obj, QUEUE_MODE_NORMAL, runnable) {
}

Message::Message(int32_t what, Object *obj, int16_t queueMode, Runnable runnable)
        : Object() {
    this->what = what;
    this->obj = obj;
    this->runnable = runnable;
    this->arg1 = 0;
    this->arg2 = 0;
    this->desc = "Undef";
    this->queueMode = queueMode;
}

Message::~Message() {
    this->runnable = nullptr;
    if (obj) {
        delete obj;
        obj = nullptr;
    }
}

void *Message::tyrUnBox() {
    ObjectBox *ob = dynamic_cast<ObjectBox *>(obj);
    return ob->ptr;
}