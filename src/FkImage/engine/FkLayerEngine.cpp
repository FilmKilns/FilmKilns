/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerEngine.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
#include "FkSetSurfacePrt.h"
#include "FkRenderRequestPrt.h"
#include "FkQuerySizeProto.h"
#include "FkIntVec2.h"
#include "FkFloatVec3.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkDrawPointProto.h"
#include "FkQueryLayersProto.h"
#include "FkRenderEngineCompo.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"
#include "FkCropProto.h"
#include "FkMath.h"
#include "FkQueryWinSizeProto.h"
#include "FkRemoveLayerProto.h"
#include "FkScaleTypeProto.h"
#include "FkReadPixelsProto.h"
#include "FkDrawPathProto.h"
#include "FkUpdateLayerModelProto.h"
#include "FkImageContext.h"
#include "FkRotateComponent.h"
#include "FkLayerSetProjectionProto.h"
#include "FkCropLayerProto.h"
#include "FkSetZIndexProto.h"
#include "FkLayerSetSourceProto.h"
#include "FkRenderEngine.h"
#include "FkLayerSetBackgroundColorProto.h"
#include "FkAddPluginProto.h"
#include "FkLayerSetVisibilityProto.h"
#include "FkAnyCompo.h"
#include "FkFileUtils.h"
#include "FkLayerSetPropertiesProto.h"

#define TAG "FkLayerEngine"

const FkID FkLayerEngine::MSG_NOTIFY_RENDER = 0x100;

FK_IMPL_CLASS_TYPE(FkLayerEngine, FkEngine)

FkLayerEngine::FkLayerEngine(std::shared_ptr<FkEngine> &renderEngine, std::string name)
        : FkEngine(name), renderEngine(renderEngine) {

    client = std::make_shared<FkLocalClient>();
    molecule = std::make_shared<FkGraphicMolecule>();
}

FkLayerEngine::~FkLayerEngine() {

}

FkResult FkLayerEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    FkAssert(ret == FK_OK, ret);
    ret = renderEngine->create();
    FkAssert(ret == FK_OK, ret);
    auto proto = std::make_shared<FkOnCreatePrt>();
    proto->context = std::make_shared<FkImageContext>();
    proto->context->addComponent(std::make_shared<FkRenderEngineCompo>(renderEngine));
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    FkAssert(ret == FK_OK, ret);
    ret = client->quickSend<FkOnDestroyPrt>(molecule);
    FkAssert(ret == FK_OK, ret);
    return renderEngine->destroy();
}

FkResult FkLayerEngine::onStart() {
    auto ret = FkEngine::onStart();
    FkAssert(ret == FK_OK, ret);
    ret = renderEngine->start();
    FkAssert(ret == FK_OK, ret);
    return client->quickSend<FkOnStartPrt>(molecule);
}

FkResult FkLayerEngine::onStop() {
    auto ret = FkEngine::onStop();
    FkAssert(ret == FK_OK, ret);
    ret = client->quickSend<FkOnStopPrt>(molecule);
    FkAssert(ret == FK_OK, ret);
    ret = renderEngine->stop();
    if (ret == FK_OK) {
        std::lock_guard<std::mutex> guard(mtx);
        lastMaxLayerId = 0;
        layerIds.clear();
    }
    return ret;
}

FkResult FkLayerEngine::setSurface(std::shared_ptr<FkGraphicWindow> win, int32_t scaleType) {
    FkTimeUtils::Cost cost("setSurface");
    if (FK_INSTANCE_OF(renderEngine, FkRenderEngine)) {
        auto render = std::dynamic_pointer_cast<FkRenderEngine>(renderEngine);
        render->updateWindow(win);
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setSurface));
    msg->sp = win;
    msg->arg1 = scaleType;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setSurface(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkSetSurfacePrt>();
    if (msg->sp) {
        proto->win = std::dynamic_pointer_cast<FkGraphicWindow>(msg->sp);
    }
    auto ret = client->with(molecule)->send(proto);

    auto type = static_cast<kScaleType>(msg->arg1);
    auto scaleProto = std::make_shared<FkScaleTypeProto>(Fk_CANVAS_ID, type);
    client->with(molecule)->send(scaleProto);
    return ret;
}

FkResult FkLayerEngine::notifyRender() {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_notifyRender));
    msg->what = MSG_NOTIFY_RENDER;
    msg->flags = FkMessage::FLAG_UNIQUE;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_notifyRender(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRenderRequestPrt>();
    proto->req = std::make_shared<FkRenderRequest>();
    return client->quickSend(proto, molecule);
}

FkID FkLayerEngine::newLayer(FkID expectId) {
    if (expectId == FK_ID_NONE) {
        std::lock_guard<std::mutex> guard(mtx);
        ++lastMaxLayerId;
        expectId = lastMaxLayerId;
    } else {
        lastMaxLayerId = std::max(lastMaxLayerId, expectId);
    }
    {
        std::lock_guard<std::mutex> guard(mtx);
        auto itr = layerIds.find(expectId);
        if (itr != layerIds.end()) { // Repeat
            FkLogW(TAG, "Repeat layerId=%d. Skip generate", expectId);
            return FK_INVALID_PARAMETERS;
        }
        layerIds.emplace(std::make_pair(expectId, expectId));
    }
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_newLayer));
    msg->arg1 = expectId;
    auto ret = sendMessage(msg);
    if (ret == FK_OK) {
        return expectId;
    }
    return ret;
}

FkResult FkLayerEngine::_newLayer(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkGraphicNewLayerPrt>();
    proto->expectId = msg->arg1;
    return client->with(molecule)->send(proto);
}

FkID FkLayerEngine::newLayerWithColor(FkSize size, FkColor color, FkID expectId) {
    auto id = newLayer(expectId);
    if (FK_ID_NONE != id) {
        auto colorCom = std::make_shared<FkColorCompo>(color);
        auto sizeCom = std::make_shared<FkSizeCompo>(size);
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        layer->addComponent(colorCom);
        layer->addComponent(sizeCom);
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithColor));
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkID FkLayerEngine::setProjectionLayer(FkID layerId, FkID srcLayerId) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setLayerProjection));
    msg->arg1 = srcLayerId;
    msg->arg2 = layerId;
    sendMessage(msg);
    return layerId;
}

FkResult FkLayerEngine::_updateLayerWithColor(std::shared_ptr<FkMessage> msg) {
    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    auto prt = std::make_shared<FkGraphicUpdateLayerPrt>();
    prt->layer = layer;
    prt->scaleType = kScaleType::CENTER_INSIDE;
    auto sizeCom = FK_FIND_COMPO(prt->layer, FkSizeCompo);
    if (sizeCom) {
        setCanvasSizeInternal(sizeCom->size, true);
    }
    return client->with(molecule)->send(prt);
}

FkID FkLayerEngine::newLayerWithSource(const std::shared_ptr<FkAbsImageSource> &source) {
    auto id = newLayer(FK_ID_NONE);
    if (FK_ID_NONE != id) {
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithSource));
        msg->arg1 = id;
        msg->sp = source;
        sendMessage(msg);
    }
    return id;
}

FkResult FkLayerEngine::_updateLayerWithSource(const std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(source, FkAbsImageSource, msg->sp);
    auto layerId = msg->arg1;
    auto proto = std::make_shared<FkLayerSetSourceProto>(layerId);
    proto->source = source;
    proto->scaleType = kScaleType::CENTER_INSIDE;
    proto->filePath = msg->arg3;
    auto ret = getClient()->with(getMolecule())->send(proto);
    auto size = source->getSize();
    if (!size.isZero()) {
        setCanvasSizeInternal(size, true);
    }
    return ret;
}

FkResult FkLayerEngine::removeLayer(FkID layer) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_removeLayer));
    msg->arg1 = layer;
    auto ret = sendMessage(msg);
    if (ret == FK_OK) {
        std::lock_guard<std::mutex> guard(mtx);
        if (layer == Fk_CANVAS_ID) {
            lastMaxLayerId = 0;
            layerIds.clear();
            return ret;
        }
        auto itr = layerIds.find(layer);
        if (itr != layerIds.end()) {
            layerIds.erase(itr);
        }
    }
    return ret;
}

FkResult FkLayerEngine::_removeLayer(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkRemoveLayerProto>(msg->arg1);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setLayerZIndex(FkID layerId, int32_t zIndex) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setLayerIndexZ));
    msg->arg1 = layerId;
    msg->arg2 = zIndex;
    return sendMessage(msg);

}

FkResult FkLayerEngine::_setLayerIndexZ(const std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkSetZIndexProto>(msg->arg1, msg->arg2);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setCanvasSize(FkSize size) {
    FkAssert(!size.isZero(), FK_FAIL);
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setCanvasSize));
    msg->sp = std::make_shared<FkSize>(size);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setCanvasSize(std::shared_ptr<FkMessage> msg) {
    FkAssert(nullptr != msg->sp, FK_NPE);
    auto size = std::dynamic_pointer_cast<FkSize>(msg->sp);
    return setCanvasSizeInternal(*size, false);
}

FkResult FkLayerEngine::setCanvasSizeInternal(FkSize &size, bool isInitialize) {
    auto queryProto = std::make_shared<FkQuerySizeProto>();
    auto ret = client->with(molecule)->send(queryProto);
    FkAssert(FK_OK == ret, FK_FAIL);
    if (isInitialize && !queryProto->value.isZero()) {
        return FK_FAIL;
    }
    if (size == queryProto->value) {
        return FK_FAIL;
    }
    FkLogI(TAG, "setCanvasSizeInternal %dx%d", size.getWidth(), size.getHeight());

    auto sizeComp = std::make_shared<FkSizeCompo>(size);

    auto layer = std::make_shared<FkGraphicLayer>();
    layer->id = Fk_CANVAS_ID;
    layer->addComponent(sizeComp);
    auto updateProto = std::make_shared<FkGraphicUpdateLayerPrt>();
    updateProto->layer = layer;
    updateProto->scaleType = kScaleType::CENTER_INSIDE;
    return client->with(molecule)->send(updateProto);
}

FkResult FkLayerEngine::postTranslate(FkID layer, int32_t dx, int32_t dy) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postTranslate));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkIntVec2>(dx, dy);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postTranslate(std::shared_ptr<FkMessage> msg) {
    auto point = std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto measure = std::make_shared<FkMeasureTransProto>();
    measure->layerId = msg->arg1;
    measure->value = FkDoubleVec2(point->x, point->y);
    auto ret = client->quickSend(measure, molecule);
    FkAssert(FK_OK == ret, FK_FAIL);
    auto proto = std::make_shared<FkLayerPostTransProto>();
    proto->layer = measure->layerId;
    proto->value = measure->value;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::setTranslate(FkID layer, int32_t x, int32_t y) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setTranslate));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkIntVec2>(x, y);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setTranslate(std::shared_ptr<FkMessage> msg) {
    auto point = std::dynamic_pointer_cast<FkIntVec2>(msg->sp);
    auto proto = std::make_shared<FkLayerSetTransProto>();
    proto->layer = msg->arg1;
    proto->value = FkDoubleVec2(point->x, point->y);;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postScale(FkID layer, float dx, float dy) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postScale));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkFloatVec3>(dx, dy, 1.0f);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postScale(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkLayerPostScaleProto>();
    proto->layer = msg->arg1;
    proto->value = *Fk_POINTER_CAST(FkFloatVec3, msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::setScale(FkID layer, float x, float y) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setScale));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkFloatVec3>(x, y, 1.0f);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setScale(std::shared_ptr<FkMessage> msg) {
    auto vec = std::dynamic_pointer_cast<FkFloatVec3>(msg->sp);
    auto proto = std::make_shared<FkLayerSetScaleProto>();
    proto->layer = msg->arg1;
    proto->value = *vec;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::postRotation(FkID layer, FkRational &rational) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_postRotation));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkRational>(rational);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_postRotation(std::shared_ptr<FkMessage> msg) {
    auto proto = std::make_shared<FkLayerPostRotateProto>();
    proto->layer = msg->arg1;
    proto->value = *std::dynamic_pointer_cast<FkRational>(msg->sp);
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::setRotation(FkID layer, FkRational &rational) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setRotation));
    msg->arg1 = layer;
    msg->sp = std::make_shared<FkRational>(rational);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setRotation(std::shared_ptr<FkMessage> msg) {
    FkAssert(msg->sp != nullptr, FK_FAIL);
    auto value = std::dynamic_pointer_cast<FkRational>(msg->sp);
    auto proto = std::make_shared<FkLayerSetRotateProto>();
    proto->layer = msg->arg1;
    proto->value = *value;
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::drawPoint(FkID layer, FkColor color, int32_t size, int32_t x, int32_t y) {
    auto proto = std::make_shared<FkDrawPointProto>();
    proto->layer = layer;
    proto->color = color;
    proto->value.x = x;
    proto->value.y = y;
    proto->size = size;
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPoint));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPoint(std::shared_ptr<FkMessage> msg) {
    auto proto = Fk_POINTER_CAST(FkDrawPointProto, msg->sp);
    auto measureProto = std::make_shared<FkMeasurePointProto>();
    measureProto->layerId = proto->layer;
    measureProto->value = FkDoubleVec2(proto->value.x, proto->value.y);
    auto ret = client->with(molecule)->send(measureProto);
    FkAssert(FK_OK == ret, FK_FAIL);
    //  -,- |
    //------0------->
    //      | +,+
    proto->value = measureProto->value;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::queryLayers(std::vector<std::shared_ptr<FkGraphicLayer>> &vec) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_queryLayers));
    msg->withPromise();
    auto ret = sendMessage(msg);
    if (FK_OK == ret && FK_OK == msg->getPromiseResult(vec)) {
        return FK_OK;
    }
    return ret;
}

FkResult FkLayerEngine::_queryLayers(std::shared_ptr<FkMessage> msg) {
    msg->setPromiseResult(queryLayers());
    return FK_OK;
}

std::vector<std::shared_ptr<FkGraphicLayer>> FkLayerEngine::queryLayers() {
    auto proto = std::make_shared<FkQueryLayersProto>();
    auto ret = client->with(molecule)->send(proto);
//    FkAssert(FK_OK != ret, proto->layers);
    return proto->layers;
}

FkResult FkLayerEngine::crop(FkIntRect &rect) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_crop));
    msg->sp = std::make_shared<FkIntRect>(rect);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_crop(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(rect, FkIntRect, msg->sp);
    auto queryProto = std::make_shared<FkQueryWinSizeProto>();
    auto ret = client->with(molecule)->send(queryProto);
    auto deltaX = (queryProto->winSize.getWidth() / 2 - (rect->right() + rect->left()) / 2);
    auto deltaY = (queryProto->winSize.getHeight() / 2 - (rect->bottom() + rect->top()) / 2);

    std::shared_ptr<FkGraphicLayer> canvas = nullptr;
    auto layers = queryLayers();
    for (auto &it : layers) {
        if (it->id == Fk_CANVAS_ID) {
            canvas = it;
            continue;
        }
        auto msg0 = FkMessage::obtain(0);
        msg0->arg1 = it->id;
        msg0->sp = std::make_shared<FkIntVec2>(deltaX, deltaY);
        _postTranslate(msg0);
    }

    std::vector<FkDoubleVec2> vec;
    vec.emplace_back(FkDoubleVec2(rect->left(), rect->top()));
    vec.emplace_back(FkDoubleVec2(rect->right(), rect->top()));
    vec.emplace_back(FkDoubleVec2(rect->right(), rect->bottom()));
    vec.emplace_back(FkDoubleVec2(rect->left(), rect->bottom()));
    for (auto &it : vec) {
        auto measureProto = std::make_shared<FkMeasurePointProto>();
        measureProto->layerId = Fk_CANVAS_ID;
        measureProto->value = it;
        ret = client->with(molecule)->send(measureProto);
        FkAssert(FK_OK == ret, FK_FAIL);
        it = measureProto->value;
    }
    auto leftTop = vec[0];
    auto rightTop = vec[1];
    auto rightBottom = vec[2];
    auto leftBottom = vec[3];
    FkSize size(FkMath::distance(leftTop, rightTop),
                FkMath::distance(leftTop, leftBottom));
    ret = setCanvasSizeInternal(size, false);
    FkRational canvasRotation(0, 1);
    if (FK_OK == ret && canvas) {
        auto rotateCompo = FK_FIND_COMPO(canvas, FkRotateComponent);
        canvasRotation = rotateCompo->value;
    }
    if (FK_OK == ret) {
        auto msg0 = FkMessage::obtain(0);
        msg0->arg1 = Fk_CANVAS_ID;
        msg0->sp = std::make_shared<FkRational>(0, 1);
        ret = _setRotation(msg0);
    }
    if (FK_OK == ret && canvasRotation.num != 0) {
        for (auto &it: layers) {
            if (it->id == Fk_CANVAS_ID) {
                continue;
            }
            auto msg0 = FkMessage::obtain(0);
            msg0->arg1 = it->id;
            msg0->sp = std::make_shared<FkRational>(canvasRotation);
            _postRotation(msg0);
        }
    }
    return ret;
}

FkResult FkLayerEngine::cropLayer(FkID layerId, FkIntRect &rect) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_cropLayer));
    msg->arg1 = layerId;
    msg->sp = std::make_shared<FkIntRect>(rect);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_cropLayer(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(rect, FkIntRect, msg->sp);
    auto proto = std::make_shared<FkCropLayerProto>(msg->arg1, *rect);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::readPixels(FkID layerId, FkIntVec2 &pos, FkSize &size,
                                   std::function<void(std::shared_ptr<FkBuffer>, FkSize)> finishCallback) {
    auto proto = std::make_shared<FkReadPixelsProto>();
    proto->layerId = layerId;
    proto->pos = pos;
    proto->size = size;
    proto->finishCallback = std::move(finishCallback);

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_readPixels));
    msg->arg1 = layerId;
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_readPixels(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkReadPixelsProto, msg->sp);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::exportImage(FkID layerId, const FkIntVec2 &pos, FkSize &size,
                                    const std::string &outputFilePath, FkResultCallback callback) {
    FkLogI(TAG, "exportImage: %s", outputFilePath.c_str());
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_exportImage));
    msg->arg1 = layerId;
    msg->arg2 = size.toInt64();
    msg->arg3 = outputFilePath;
    msg->sp = callback ? std::make_shared<FkAnyCompo>(callback) : nullptr;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_exportImage(const std::shared_ptr<FkMessage> &msg) {
    auto size = FkSize(msg->arg2);
    auto bmp= FkBitmap::create(size.getWidth(), size.getHeight());
    auto outputFilePath = msg->arg3;
    auto proto = std::make_shared<FkRenderRequestPrt>();
    FkResultCallback  resultCallback = nullptr;
    auto compo = std::dynamic_pointer_cast<FkAnyCompo>(msg->sp);
    if (compo && compo->any.has_value()) {
        resultCallback = std::any_cast<FkResultCallback>(compo->any);
    }
    proto->req = std::make_shared<FkRenderRequest>();
    proto->req->strategy = {true, size, kScaleType::DEFAULT, FkBuffer::wrap(bmp->getPixels(), bmp->getByteSize())};
    proto->req->strategy.doneFunc = [this, bmp, outputFilePath, resultCallback](){
        auto saveMsg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_saveImageFile));
        saveMsg->arg3 = outputFilePath;
        saveMsg->sp = std::make_shared<FkAnyCompo>(std::make_pair(bmp, resultCallback));
        if (sendMessage(saveMsg) != FK_OK) {
            if (resultCallback) {
                resultCallback(FK_FAIL);
            }
        }
    };
    return client->quickSend(proto, molecule);
}

FkResult FkLayerEngine::_saveImageFile(const std::shared_ptr<FkMessage> &msg) {
    auto compo = std::dynamic_pointer_cast<FkAnyCompo>(msg->sp);
    if (compo && compo->any.has_value()) {
        auto pair = std::any_cast<std::pair<std::shared_ptr<FkBitmap>, FkResultCallback>>(compo->any);
        auto filePath = msg->arg3;
        auto bmp = pair.first;
        auto resultCallback = pair.second;
        FkImage::Format fmt;
        std::string suffix = FkFileUtils::suffix(msg->arg3);
        if (suffix == ".jpeg" || suffix == ".jpg") {
            fmt = FkImage::Format::kJPEG;
        } else if (suffix == ".png") {
            fmt = FkImage::Format::kPNG;
        } else if (suffix == ".bmp") {
            fmt = FkImage::Format::kBMP;
        } else if (suffix == ".webp") {
            fmt = FkImage::Format::kWEBP;
        } else {
            fmt = FkImage::Format::kJPEG;
        }
        auto ret = bmp->write(msg->arg3, fmt, 85);
        if (resultCallback) {
            resultCallback(ret);
        }
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkLayerEngine::drawPath(FkID layerId,
                                 int32_t x, int32_t y,
                                 std::shared_ptr<FkPaint> &paint) {
    FkAssert(paint != nullptr && paint->strokeWidth > 0, FK_INVALID_DATA);
    FkDoubleVec2 point(x, y);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isFinish = false;
    proto->paint = paint;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_drawPath(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkDrawPathProto, msg->sp);
    auto measureProto = std::make_shared<FkMeasurePointProto>();
    measureProto->layerId = proto->layerId;
    measureProto->value = FkDoubleVec2(proto->point.x, proto->point.y);
    auto ret = client->with(molecule)->send(measureProto);
    FkAssert(FK_OK == ret, FK_FAIL);

    proto->point = measureProto->value;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::drawPathFinish(FkID layerId) {
    FkDoubleVec2 point(0, 0);
    auto proto = std::make_shared<FkDrawPathProto>(layerId, point);
    proto->isFinish = true;

    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_drawPath));
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::updateLayerWithModel(FkID layerId,
                                             std::shared_ptr<FkModelInterface> &modelInterface) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithModel));
    msg->arg1 = layerId;
    msg->sp = modelInterface;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_updateLayerWithModel(std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(modelInterface, FkModelInterface, msg->sp);
    auto proto = std::make_shared<FkUpdateLayerModelProto>(modelInterface);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::_setLayerProjection(const std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkLayerSetProjectionProto>(msg->arg1, msg->arg2);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setBackgroundColor(FkID layerId, FkColor color) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setBackgroundColor));
    msg->arg1 = layerId;
    msg->arg2 = color.toInt();
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setBackgroundColor(const std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkLayerSetBackgroundColorProto>(msg->arg1);
    proto->value = std::make_shared<FkColor>(FkColor::makeFrom(msg->arg2));
    return client->with(molecule)->send(proto);
}

FkID FkLayerEngine::newLayerWithPlugin(const std::shared_ptr<FkAbsPlugin> &plugin) {
    auto id = newLayer(FK_ID_NONE);
    if (FK_ID_NONE != id) {
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_updateLayerWithPlugin));
        msg->arg1 = id;
        msg->sp = std::move(plugin);
        sendMessage(msg);
    }
    return id;
}

FkResult FkLayerEngine::_updateLayerWithPlugin(const std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(plugin, FkAbsPlugin, msg->sp);
    auto proto = std::make_shared<FkAddPluginProto>();
    proto->layerId = msg->arg1;
    proto->value = plugin;
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setVisibility(FkID layerId, kVisibility visibility) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setVisibility));
    msg->arg1 = layerId;
    msg->arg2 = static_cast<int64_t>(visibility);
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setVisibility(const std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkLayerSetVisibilityProto>();
    proto->layerId = msg->arg1;
    proto->visibility = static_cast<kVisibility>(msg->arg2);
    return client->with(molecule)->send(proto);
}

FkResult FkLayerEngine::setLayerProperties(FkID layerId,
                                           const std::unordered_map<std::string, FkValue> &properties) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkLayerEngine::_setLayerProperties));
    auto proto = std::make_shared<FkLayerSetPropertiesProto>();
    proto->layerId = layerId;
    proto->properties = properties;
    msg->sp = proto;
    return sendMessage(msg);
}

FkResult FkLayerEngine::_setLayerProperties(const std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetPropertiesProto, msg->sp);
    return client->with(molecule)->send(proto);
}
