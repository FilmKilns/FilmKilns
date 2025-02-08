/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlUImageCodec.h"
#include "AlGImage.h"
#include "AlScreen.h"
#include "ObjectBox.h"
#include "AlLayerActionFactory.h"
#include "AlMCropAction.h"
#include "AlContext.h"
#include "AlCoordsTranslator.h"
#include "AlTarUtil.h"
#include "AlMath.h"
#include "AlRenderParams.h"
#include "AlPaintDesc.h"
#include "AlOperateScale.h"
#include "AlOperateRotate.h"
#include "AlOperateTrans.h"
#include "AlOperateAlpha.h"
#include "AlOperateCrop.h"
#include "AlFuture.h"

#define TAG "AlImageProcessor"

AlImageProcessor::AlImageProcessor() : AlAbsProcessor("AlImageProcessor") {
//    tar_t *archive = nullptr;
//    int count = tar_read_file("/sdcard/test.tar", &archive);
//    if (archive) {
//        for (int i = 0; i < count; ++i) {
//            Logcat::i(TAG, "%s", archive[i].name);
//        }
//        tar_free(archive);
//    }
    registerAnUnit(new AlScreen(ALIAS_OF_SCREEN));
    registerAnUnit(new AlUImageCodec(ALIAS_OF_IMAGE));
    registerAnUnit(new AlGImage(ALIAS_OF_IMAGE_GRAPH));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onCanvasUpdate));
    registerEvent(EVENT_LAYER_QUERY_ID_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onLayerQuery));
    registerEvent(EVENT_LAYER_EXPORT_FINISH,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onExportFinish));
    registerEvent(EVENT_LAYER_IMPORT_FINISH,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onImportFinish));
    registerEvent(EVENT_CANVAS_SAVE_FINISH,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onSaveFinish));
    registerEvent(EVENT_LAYER_QUERY_INFO_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onLayerInfoResult));
}

AlImageProcessor::~AlImageProcessor() {
    this->onSaveListener = nullptr;
}

void AlImageProcessor::onCreate() {
    AlAbsProcessor::onCreate();
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    this->aBaseCtx = AlEgl::offScreen(TAG);
    this->context = new AlContext();

    AlMessage *msg = AlMessage::obtain(MSG_CANVAS_SET_BG);
    msg->arg1 = static_cast<int32_t>(AlImageCanvas::kBGType::GRID);
    postEvent(msg);
//    this->putObject("AL_CONTEXT", this->context)
//            .to({ALIAS_OF_LAYER, ALIAS_OF_DESCRIPTOR, ALIAS_OF_CANVAS, ALIAS_OF_SCREEN});
}

void AlImageProcessor::onDestroy() {
    AlAbsProcessor::onDestroy();
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    delete this->context;
    this->context = nullptr;
    delete this->aBaseCtx;
    this->aBaseCtx = nullptr;
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    mWinSize.width = win->getWidth();
    mWinSize.height = win->getHeight();
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, AlRectLoc loc, AlColor color) {
    mCanvasSize.width = w;
    mCanvasSize.height = h;
    auto *msg = AlMessage::obtain(MSG_LAYER_RESIZE_CANVAS,
                                  new AlRectLoc(loc));
    msg->arg1 = w;
    msg->arg2 = h;
    postEvent(msg);
}

void AlImageProcessor::invalidate(int32_t flag) {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE, nullptr,
                                       AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg1 = flag;
    postEvent(msg);
}

int32_t AlImageProcessor::addLayer(const char *path) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_ADD);
    msg->arg1 = EVENT_LAYER_ADD;
    msg->desc = std::string(path);
    postEvent(msg);
    mQueryLock.wait();
    return mCurLayerId;
}

HwResult AlImageProcessor::removeLayer(int32_t id) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_REMOVE);
    msg->arg1 = id;
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::moveLayerIndex(int32_t id, int32_t index) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MOVE);
    msg->arg1 = id;
    msg->arg2 = index;
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setScale(int32_t id, AlRational scale) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_SCALE,
                                  new AlOperateScale(id, scale, AlPointF(0, 0)));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postScale(int32_t id, AlRational ds, AlPointF anchor) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_SCALE_POST,
                                  new AlOperateScale(id, ds, anchor));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setRotation(int32_t id, AlRational r) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ROTATE,
                                  new AlOperateRotate(id, r, AlPointF(0, 0)));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postRotation(int32_t id, AlRational dr, AlPointF anchor) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ROTATE_POST,
                                  new AlOperateRotate(id, dr, anchor));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_TRANS,
                                  new AlOperateTrans(id, x, y));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postTranslate(int32_t id, float dx, float dy) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_TRANS_POST,
                                  new AlOperateTrans(id, dx, dy));
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setAlpha(int32_t id, float alpha) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_TRANS_POST,
                                  new AlOperateAlpha(id, alpha));
    postEvent(msg);
    return Hw::SUCCESS;
}

int32_t AlImageProcessor::getLayer(float x, float y) {
    auto bundle = std::make_shared<AlFuture>();
    auto *msg = AlMessage::obtain(MSG_LAYER_QUERY_ID_FUTURE,
                                  new AlOperateTrans(0, x, y));
    msg->sp = bundle;
    postEvent(msg);
    return bundle->get(-1);
}

HwResult AlImageProcessor::cropLayer(int32_t id, float left, float top, float right, float bottom) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_CROP,
                                  new AlOperateCrop(id, left, top, right, bottom),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::cropCanvas(float left, float top, float right, float bottom) {
    auto *msg = AlMessage::obtain(EVENT_CANVAS_CROP,
                                  new AlOperateCrop(AlIdentityCreator::NONE_ID,
                                                    left, top, right, bottom),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::cancelCropLayer(int32_t id) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_CROP_CANCEL,
                                  new AlOperateCrop(id, 0, 0, 0, 0),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::save(std::string path) {
    auto *m = AlMessage::obtain(EVENT_CANVAS_SAVE);
    m->arg1 = EVENT_CANVAS_SAVE;
    m->desc = path;
    postMessage(m);
    AlRenderParams params;
    params.setRenderScreen(false);
    params.setTransparent(true);
    invalidate(params.toInt());
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::exportFile(std::string path) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_EXPORT);
    msg->desc = path;
    postEvent(msg);
    mExportLock.wait();
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::importFile(std::string path) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_IMPORT);
    msg->desc = path;
    postEvent(msg);
    invalidate();
    return Hw::SUCCESS;
}

void AlImageProcessor::setOnSaveListener(OnSaveListener listener) {
    this->onSaveListener = listener;
}

HwResult AlImageProcessor::ensureAlignCrop(int32_t id, AlRational r) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ALIGN_CROP,
                                  new AlOperateRotate(id, r, AlVec2(0, 0)),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::cancelAlignCrop(int32_t id) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ALIGN_CROP_CANCEL,
                                  new AlOperateRotate(id, AlRational(), AlVec2()),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::redo() {
    postEvent(AlMessage::obtain(EVENT_AIMAGE_REDO));
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::undo() {
    postEvent(AlMessage::obtain(EVENT_AIMAGE_UNDO));
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::paint(int32_t id, float x, float y, bool painting) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_PAINT);
    msg->obj = new AlPaintDesc(id, x, y, painting);
    postEvent(msg);
    return Hw::SUCCESS;
}

void AlImageProcessor::queryLayerInfo() {
    postEvent(AlMessage::obtain(EVENT_LAYER_QUERY_INFO));
}

bool AlImageProcessor::_onCanvasUpdate(AlMessage *msg) {
    mCanvasSize.width = msg->arg1;
    mCanvasSize.height = static_cast<int>(msg->arg2);
    return true;
}

bool AlImageProcessor::_onLayerQuery(AlMessage *msg) {
    AlLogI(TAG, "%d", msg->arg1);
    mCurLayerId = msg->arg1;
    mQueryLock.notify();
    return true;
}

bool AlImageProcessor::_onExportFinish(AlMessage *msg) {
    mExportLock.notify();
    return true;
}

bool AlImageProcessor::_onImportFinish(AlMessage *msg) {
    return true;
}

bool AlImageProcessor::_onSaveFinish(AlMessage *msg) {
    if (this->onSaveListener) {
        int ret = static_cast<int>(msg->arg2);
        std::string tip("Save finish.");
        if (Hw::SUCCESS.code != ret) {
            tip = "Save failed.";
        }
        this->onSaveListener(ret, tip.c_str(), msg->desc.c_str());
    }
    return true;
}

bool AlImageProcessor::_onLayerInfoResult(AlMessage *msg) {
    auto *box = msg->getObj<ObjectBox *>();
    if (this->onLayerInfoListener && box) {
        auto *models = static_cast<vector<AlImgLayerDescription *> *>(box->ptr);
        std::vector<int32_t> ids(models->size());
        std::vector<int32_t> ws(models->size());
        std::vector<int32_t> hs(models->size());
        for (int i = 0; i < models->size(); ++i) {
            ids[i] = (*models)[i]->getId();
            AlSize size = (*models)[i]->getSize();
            ws[i] = size.width;
            hs[i] = size.height;
        }
        this->onLayerInfoListener(ids, ws, hs);
        delete models;
    }
    return true;
}

void AlImageProcessor::setOnLayerInfoListener(AlImageProcessor::OnLayerInfoListener l) {
    this->onLayerInfoListener = l;
}
