/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUCanvas.h"
#include "HwTexture.h"
#include "AlBitmapFactory.h"
#include "AlTexManager.h"
#include "ObjectBox.h"

#define TAG "AlUCanvas"

AlUCanvas::AlUCanvas(const string &alias) : Unit(alias) {
    al_reg_msg(EVENT_CANVAS_RESIZE, AlUCanvas::onResize);
    al_reg_msg(EVENT_LAYER_RENDER_CLEAR, AlUCanvas::onClear);
    al_reg_msg(EVENT_LAYER_RENDER_DRAW, AlUCanvas::onDraw);
    al_reg_msg(EVENT_LAYER_RENDER_SHOW, AlUCanvas::onShow);
    al_reg_msg(EVENT_CANVAS_SAVE, AlUCanvas::onSave);
    al_reg_msg(MSG_CANVAS_SET_BG, AlUCanvas::onSetBackground);
    al_reg_msg(EVENT_IMAGE_CODEC_ENCODE_NOTIFY, AlUCanvas::onEncodeFinish);
}

AlUCanvas::~AlUCanvas() {
}

bool AlUCanvas::onCreate(AlMessage *msg) {
    return true;
}

bool AlUCanvas::onDestroy(AlMessage *msg) {
    mCanvas.release();
    return true;
}

bool AlUCanvas::onResize(AlMessage *m) {
    AlLogI(TAG, "");
    auto *size = m->getObj<AlSize *>();
    if (nullptr == size) {
        return true;
    }
    _update(size->width, size->height, 0);
    return true;
}

bool AlUCanvas::onClear(AlMessage *m) {
    AlLogD(TAG, "");
    mCanvas.clear();
    mDrawCount = 0;
    postEvent(AlMessage::obtain(EVENT_CANVAS_CLEAR_DONE));
    return false;
}

bool AlUCanvas::onDraw(AlMessage *m) {
    if (nullptr == m->obj) {
        return true;
    }
    AlLogD(TAG, "%d", m->arg1);
    AlImageLayerDrawModel *description = dynamic_cast<AlImageLayerDrawModel *>(m->obj);
    _newDefaultCanvas(description->getLayerSize());
    _draw(description);
    ++mDrawCount;
    _notifyDrawDone();
    if (!mOutputPath.empty()) {
        auto *msg = AlMessage::obtain(EVENT_IMAGE_CODEC_ENCODE, mCanvas.getOutput());
        msg->arg1 = EVENT_CANVAS_SAVE;
        msg->desc = mOutputPath;
        postMessage(msg);
    }
    return true;
}

bool AlUCanvas::onShow(AlMessage *m) {
    AlLogD(TAG, "");
    auto tex = mCanvas.getOutput();
    if (nullptr == tex) {
        AlLogE(TAG, "Empty canvas");
        return true;
    }
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_DRAW_TEX);
    msg->obj = tex;
    postEvent(msg);
    return true;
}

bool AlUCanvas::onSave(AlMessage *m) {
    AlLogI(TAG, "");
    mOutputPath = m->desc;
    return true;
}

void AlUCanvas::_newDefaultCanvas(AlSize size) {
    if (size.width <= 0 || size.height <= 0) {
        return;
    }
    if (mCanvas.getWidth() > 0 && mCanvas.getHeight() > 0) {
        return;
    }
    _update(size.width, size.height, 0);
}

void AlUCanvas::_draw(AlImageLayerDrawModel *description) {
    mCanvas.draw(description);
}

void AlUCanvas::_update(int32_t width, int32_t height, int32_t color) {
    mCanvas.update(width, height, color);
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE_CANVAS_NOTIFY);
    msg->arg1 = mCanvas.getWidth();
    msg->arg2 = mCanvas.getHeight();
    postEvent(msg);
}

void AlUCanvas::_notifyDrawDone() {
    auto *msg = AlMessage::obtain(EVENT_CANVAS_DRAW_DONE);
    msg->arg1 = mDrawCount;
    postEvent(msg);
}

bool AlUCanvas::onSetBackground(AlMessage *m) {
    AlImageCanvas::kBGType type = static_cast<AlImageCanvas::kBGType>(m->arg1);
    mCanvas.setBGType(type);
    return true;
}

bool AlUCanvas::onEncodeFinish(AlMessage *msg) {
    switch (msg->arg1) {
        case EVENT_CANVAS_SAVE: {
            auto *m = AlMessage::obtain(EVENT_CANVAS_SAVE_FINISH);
            m->arg2 = msg->arg2;
            m->desc = msg->desc;
            postMessage(m);
            break;
        }
    }
    return true;
}
