/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerFilter.h"
#include "AlPaintFilter.h"
#include "HwNormalFilter.h"
#include "AlAbsMFilterAction.h"
#include "AlMPaintAction.h"
#include "AlLayerPair.h"
#include "AlTexManager.h"
#include "AlMath.h"
#include "AlOrthMatrix.h"
#include "Al2DCoordinate.h"

#define TAG "AlULayerFilter"

AlULayerFilter::AlULayerFilter(string alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_FILTER_RENDER,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onDoFilterAction));
    registerEvent(EVENT_LAYER_REMOVE_CACHE_LAYER,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onRemoveLayer));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onCanvasSizeUpdate));
}

AlULayerFilter::~AlULayerFilter() {

}

bool AlULayerFilter::onCreate(AlMessage *msg) {
    copyFilter = new HwNormalFilter();
    copyFilter->prepare();
    paintFilter = new AlPaintFilter();
    paintFilter->prepare();
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete copyFilter;
    copyFilter = nullptr;
    delete paintFilter;
    paintFilter = nullptr;
    return true;
}

void AlULayerFilter::_translate(AlMatrix &mat, float transX, float transY, float &x, float &y) {
    AlVec4 src(x + transX, y + transY);
    AlVec4 dest = src * mat;
    x = dest.x;
    y = dest.y;
}

void AlULayerFilter::_transWin2Layer(AlImageLayerModel *model, float &x, float &y) {
    AlSize mWinSize(1440, 2872);
    AlSize mCanvasSize(1628, 2896);
    AlMatrix cMat, lMat;
    cMat.setScale(mWinSize.width / (float) mCanvasSize.width,
                  mWinSize.height / (float) mCanvasSize.height);

    _translate(cMat, 0, 0, x, y);
    y = -y;
    AlVec2 scale = model->getScale();
    AlRational rotation = model->getRotation();
    AlVec2 pos = model->getPosition();
    double alpha = -rotation.toFloat() * AlMath::PI;

    lMat.setScale(1 / scale.x, 1 / scale.y);
    lMat.setRotation(alpha);
    _translate(lMat, -pos.x, pos.y, x, y);
    AlLogI(TAG, "pos(%f, %f)", x, y);
}

void AlULayerFilter::_transCanvas2Layer(AlImageLayerModel *model, AlImageLayer *layer,
                                        float &x, float &y) {
    ///1080x1794
    AlVec2 vec(x, y);
    Al2DCoordinate srcCoord(aCanvasSize.width, aCanvasSize.height);
    Al2DCoordinate dstCoord(layer->getWidth(), layer->getHeight());
    dstCoord.setScale(model->getScale().x, model->getScale().y);
    dstCoord.setRotation(model->getRotation());
    dstCoord.setPosition(model->getPosition().x, model->getPosition().y);
    srcCoord.translate(&vec, &dstCoord);
    x = vec.x;
    y = vec.y;
    AlLogI(TAG, "pos(%f, %f), trans(%f, %f)", x, y, model->getPosition().x, model->getPosition().y);
}

void AlULayerFilter::_showDebugInfo(AlImageLayerModel *model, AlImageLayer *src, AlImageLayer *dst) {
    AlPointF lt(-0.016113f, 0.047887f);
    AlPointF rb(0.870605f, -0.364090f);
    AlPointF lb(lt.x, rb.y);
    AlPointF rt(rb.x, lt.y);
    _transCanvas2Layer(model, src, lt.x, lt.y);
    _transCanvas2Layer(model, src, rb.x, rb.y);
    _transCanvas2Layer(model, src, lb.x, lb.y);
    _transCanvas2Layer(model, src, rt.x, rt.y);
    std::vector<float> point(8);
    point[0] = lt.x;
    point[1] = lt.y;
    point[2] = rb.x;
    point[3] = rb.y;
    point[4] = lb.x;
    point[5] = lb.y;
    point[6] = rt.x;
    point[7] = rt.y;
    dynamic_cast<AlPaintFilter *>(paintFilter)->setPath(&point, true);
    glViewport(0, 0, dst->getWidth(), dst->getHeight());
    dynamic_cast<AlPaintFilter *>(paintFilter)->setColor(AlColor(0x00ff0000));
    dynamic_cast<AlPaintFilter *>(paintFilter)->setPaintSize(0.01f);
    paintFilter->draw(nullptr, dst->getTexture());
}

bool AlULayerFilter::onCanvasSizeUpdate(AlMessage *msg) {
    aCanvasSize.width = msg->arg1;
    aCanvasSize.height = static_cast<int>(msg->arg2);
    return true;
}

bool AlULayerFilter::onDoFilterAction(AlMessage *msg) {
    AlLogD(TAG, "layer size: %d", layers.size());
    AlLayerPair *pair = msg->getObj<AlLayerPair *>();
    if (pair->model->countFilterAction() <= 0) {
        _notifyDescriptor(pair->layer, pair->model, msg->arg1);
        return true;
    }
    auto *fLayer = _findLayer(pair->model, pair->layer);
//    _showDebugInfo(pair->model, pair->layer, fLayer);
    if (nullptr == fLayer) {
        _notifyDescriptor(pair->layer, pair->model, msg->arg1);
        return true;
    }
    AlLogD(TAG, "layer: %d", pair->model->getId());
    auto *actions = pair->model->getAllActions();
    auto itr = actions->begin();
    while (actions->end() != itr) {
        AlAbsMFilterAction *action = dynamic_cast<AlAbsMFilterAction *>(*itr);
        if (nullptr == action) {
            AlLogW(TAG, "Null action");
        }
        if (nullptr != action && typeid(AlMPaintAction) == typeid(*action)) {
            if (pair->layer->getWidth() != fLayer->getWidth()
                || pair->layer->getHeight() != fLayer->getHeight()) {
                fLayer->getTexture()->update(nullptr,
                                             pair->layer->getWidth(),
                                             pair->layer->getHeight(),
                                             GL_RGBA);
            }
            AlMPaintAction *nAction = dynamic_cast<AlMPaintAction *>(action);
            dynamic_cast<AlPaintFilter *>(paintFilter)->setColor(nAction->getColor());
            dynamic_cast<AlPaintFilter *>(paintFilter)->setPaintSize(nAction->getPaintSize());
            std::vector<float> *path = new std::vector<float>;
            nAction->getDiffPath(*path);
            dynamic_cast<AlPaintFilter *>(paintFilter)->setPath(path, true);
            path->clear();
            delete path;
            glViewport(0, 0, fLayer->getWidth(), fLayer->getHeight());
            dynamic_cast<AlPaintFilter *>(paintFilter)->setColor(AlColor(0x00ff0000));
            dynamic_cast<AlPaintFilter *>(paintFilter)->setPaintSize(0.01f);
            paintFilter->draw(nullptr, fLayer->getTexture());
        }
        ++itr;
    }
    _notifyDescriptor(fLayer, pair->model, msg->arg1);
    return true;
}

bool AlULayerFilter::onRemoveLayer(AlMessage *msg) {
    auto itr = layers.find(msg->arg1);
    if (layers.end() != itr) {
        auto *layer = itr->second;
        auto *tex = layer->getTexture();
        AlTexManager::instance()->recycle(&tex);
        delete layer;
        layers.erase(itr);
    }
    return true;
}

void AlULayerFilter::_notifyDescriptor(AlImageLayer *layer,
                                       AlImageLayerModel *model,
                                       int32_t flags) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, new AlLayerPair(layer, model));
    msg->arg1 = flags;
    msg->desc = "measure";
    postEvent(msg);
}

AlImageLayer *AlULayerFilter::_findLayer(AlImageLayerModel *model, AlImageLayer *layer) {
    if (nullptr == model) {
        return nullptr;
    }
    int32_t id = model->getId();
    auto itr = layers.find(id);
    if (layers.end() == itr) {
        auto *l = AlImageLayer::create(AlTexManager::instance()->alloc());
        if (l) {
            layers.insert(std::pair<int32_t, AlImageLayer *>(id, l));
            l->getTexture()->update(nullptr, layer->getWidth(), layer->getHeight(), GL_RGBA);
            glViewport(0, 0, layer->getWidth(), layer->getHeight());
            copyFilter->draw(layer->getTexture(), l->getTexture());
        }
        return l;
    }
    return itr->second;
}