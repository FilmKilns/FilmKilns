/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicLayerQuark.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkColorCompo.h"
#include "FkSizeCompo.h"
#include "FkRenderRequestPrt.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkScaleTypeComponent.h"
#include "FkImageSourceCompo.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkDrawPointProto.h"
#include "FkPointFCompo.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkImageContext.h"
#include "FkBitmapCompo.h"
#include "FkFilePathCompo.h"
#include "FkQueryLayersProto.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"
#include "FkCropProto.h"
#include "FkRemoveLayerProto.h"
#include "FkReadPixelsProto.h"
#include "FkScaleTypeProto.h"
#include "FkBitmapDefinition.h"
#include "FkDrawPathProto.h"
#include "FkUpdateLayerModelProto.h"
#include "FkMeshPath.h"
#include "FkQueryLayerProto.h"
#include "FkLayerSetProjectionProto.h"
#include "FkCropLayerProto.h"
#include "FkCropComponent.h"
#include "FkSetZIndexProto.h"
#include "FkLayerSetSourceProto.h"
#include "FkLayerSetBackgroundColorProto.h"
#include "FkBackgroundColorCompo.h"
#include "FkAddPluginProto.h"
#include "FkPluginCompo.h"
#include "FkLayerSetVisibilityProto.h"
#include "FkLayerSetPropertiesProto.h"
#include <cmath>

FK_IMPL_CLASS_TYPE(FkGraphicLayerQuark, FkQuark)

FkGraphicLayerQuark::FkGraphicLayerQuark() : FkQuark() {

}

FkGraphicLayerQuark::~FkGraphicLayerQuark() {

}

void FkGraphicLayerQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicLayerQuark::_onNewLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicLayerQuark::_onUpdateLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkRemoveLayerProto, FkGraphicLayerQuark::_onRemoveLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicLayerQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicLayerQuark::_onPostTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetTransProto, FkGraphicLayerQuark::_onSetTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicLayerQuark::_onPostScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetScaleProto, FkGraphicLayerQuark::_onSetScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicLayerQuark::_onPostRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetRotateProto, FkGraphicLayerQuark::_onSetRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicLayerQuark::_onWithLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasurePointProto, FkGraphicLayerQuark::_onWithLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicLayerQuark::_onDrawPoint);
    FK_PORT_DESC_QUICK_ADD(desc, FkQueryLayersProto, FkGraphicLayerQuark::_onQueryLayers);
    FK_PORT_DESC_QUICK_ADD(desc, FkQueryLayerProto, FkGraphicLayerQuark::_onQueryLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkCropProto, FkGraphicLayerQuark::_onCrop);
    FK_PORT_DESC_QUICK_ADD(desc, FkCropLayerProto, FkGraphicLayerQuark::_onCropLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkReadPixelsProto, FkGraphicLayerQuark::_onWithLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkScaleTypeProto, FkGraphicLayerQuark::_onUpdateScaleType);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPathProto, FkGraphicLayerQuark::_onDrawPath);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateLayerModelProto, FkGraphicLayerQuark::_onUpdateLayerWithModel);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetProjectionProto, FkGraphicLayerQuark::_onSetProjection);
    FK_PORT_DESC_QUICK_ADD(desc, FkSetZIndexProto, FkGraphicLayerQuark::_onSetZIndex);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetSourceProto, FkGraphicLayerQuark::_onUpdateLayerWithSource);
    FK_PORT_DESC_QUICK_ADD(desc, FkAddPluginProto, FkGraphicLayerQuark::_onUpdateLayerWithPlugin);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetBackgroundColorProto, FkGraphicLayerQuark::_onSetBackgroundColor);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetVisibilityProto, FkGraphicLayerQuark::_onSetVisibility);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetPropertiesProto, FkGraphicLayerQuark::_onSetProperties);
}

FkResult FkGraphicLayerQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkGraphicLayerQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkGraphicLayerQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicLayerQuark::onStop() {
    _clearLayers();
    return FkQuark::onStop();
}

void FkGraphicLayerQuark::_removeLayer(const std::shared_ptr<FkGraphicLayer> &layer) {
    auto pluginCompo = FK_FIND_COMPO(layer, FkPluginCompo);
    if (pluginCompo && !pluginCompo->value->describeRequirements().contains(
            FkPluginRequirements::Type::THREAD_GL)) {
        pluginCompo->value->onDestroy();
    }
    auto context = std::dynamic_pointer_cast<FkImageContext>(getContext());
    FkAssert(context != nullptr, FK_NPE);
    auto renderEngine = context->getRenderEngine();
    FkAssert(renderEngine != nullptr, FK_NPE);
    renderEngine->removeMaterial(layer->material);
}

void FkGraphicLayerQuark::_clearLayers() {
    for (auto &itr: layers) {
        auto layer = itr.second;
        _removeLayer(layer);
    }
    layers.clear();
}

bool FkGraphicLayerQuark::_isExistLayer(FkID id) {
    return layers.find(id) != layers.end();
}

FkID FkGraphicLayerQuark::_generateId(FkID expectId) {
    FkAssert(FK_ID_NONE != expectId, FK_ID_NONE);
    FkAssert(!_isExistLayer(expectId), FK_ID_NONE);
    return expectId;
}

std::shared_ptr<FkGraphicLayer> FkGraphicLayerQuark::_findLayer(FkID layerId) {
    auto itr = layers.find(layerId);
    if (layers.end() == itr) {
        return nullptr;
    }
    return itr->second;
}

FkResult FkGraphicLayerQuark::_onNewLayer(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGraphicNewLayerPrt, p);
    proto->layer = newLayerEntity();
    if (proto->layer) {
        proto->layer->id = _generateId(proto->expectId);
        layers.emplace(std::make_pair(proto->layer->id, proto->layer));
        return FK_OK;
    }
    return FK_FAIL;
}

std::shared_ptr<FkGraphicLayer> FkGraphicLayerQuark::newLayerEntity() {
    auto layer = std::make_shared<FkGraphicLayer>();
    layer->addComponent(std::make_shared<FkTransComponent>());
    layer->addComponent(std::make_shared<FkScaleComponent>());
    layer->addComponent(std::make_shared<FkRotateComponent>());

    auto context = std::dynamic_pointer_cast<FkImageContext>(getContext());
    FkAssert(context != nullptr, nullptr);
    auto renderEngine = context->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    layer->material = renderEngine->addMaterial();
    layer->id = layer->material->id();
    return layer;
}

FkResult FkGraphicLayerQuark::_onUpdateLayer(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGraphicUpdateLayerPrt, p);
    auto itr = layers.find(proto->layer->id);
    if (layers.end() == itr) {
        _withCanvasSize(proto);
        return FK_SKIP;
    }
    FkResult ret = FK_OK;
    auto bmpCompo = FK_FIND_COMPO(proto->layer, FkBitmapCompo);
    auto layer = itr->second;
    auto layerColor = _updateLayerColor(proto, layer);
    bool isSwappedWH = bmpCompo != nullptr && bmpCompo->bmp != nullptr && bmpCompo->bmp->isSwappedWH();
    auto reqSizeCompo = FK_FIND_COMPO(proto->layer, FkSizeCompo);
    if (reqSizeCompo == nullptr) {
        reqSizeCompo = std::make_shared<FkSizeCompo>(FkSize(0, 0));
    }
    auto layerSize = _updateLayerSize(proto, layer, reqSizeCompo->size, isSwappedWH);

    if (!layerSize.isZero()) {
        auto context = std::dynamic_pointer_cast<FkImageContext>(getContext());
        FkAssert(context != nullptr, FK_NPE);
        auto renderEngine = context->getRenderEngine();
        FkAssert(renderEngine != nullptr, FK_NPE);
        if (bmpCompo) {
            auto buf = FkBuffer::alloc(bmpCompo->bmp->getByteSize());
            memcpy(buf->data(), bmpCompo->bmp->getPixels(), buf->capacity());
            ret = renderEngine->updateMaterialWithBitmap(layer->material,
                                                         layerSize,
                                                         buf);
            FkAssert(FK_OK == ret, ret);
            _updateLayerByEncodeOrigin(layer, (int32_t) bmpCompo->bmp->getEncodedOrigin());
            layer->copyComponentFrom(proto->layer, FkFilePathCompo_Class::type);
        } else {
            ret = renderEngine->updateMaterial(layer->material,
                                               layerSize,
                                               layerColor);
            FkAssert(FK_OK == ret, ret);
        }
    }
    return ret;
}

FkResult FkGraphicLayerQuark::_onRemoveLayer(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRemoveLayerProto, p);
    if (proto->layerId == Fk_CANVAS_ID) {
        _clearLayers();
        return FK_OK;
    }
    auto itr = layers.find(proto->layerId);
    if (itr != layers.end()) {
        auto layer = itr->second;
        _removeLayer(layer);
        layers.erase(itr);
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkGraphicLayerQuark::_onRenderRequest(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderRequestPrt, p);
    for (auto &itr: layers) {
        auto layer = itr.second;
        if (layer->visibility != kVisibility::VISIBLE) {
            continue;
        }
        std::shared_ptr<FkGraphicLayer> copyLayer = nullptr;
        if (layer->projLayerId != FK_ID_NONE) {
            auto srcLayer = _findLayer(layer->projLayerId);
            if (srcLayer) {
                copyLayer = std::make_shared<FkGraphicLayer>(*srcLayer);
                copyLayer->id = layer->id;
                copyLayer->projLayerId = layer->projLayerId;
                copyLayer->zIndex = layer->zIndex;
                copyLayer->copyComponentFrom(layer, FkTransComponent_Class::type);
                copyLayer->copyComponentFrom(layer, FkRotateComponent_Class::type);
                copyLayer->copyComponentFrom(layer, FkScaleComponent_Class::type);
                copyLayer->copyComponentFrom(layer, FkCropComponent_Class::type);
                copyLayer->copyComponentFrom(layer, FkPluginCompo_Class::type);
            }
        } else {
            copyLayer = std::make_shared<FkGraphicLayer>(*layer);
        }
        if (copyLayer) {
            proto->req->layers.emplace_back(copyLayer);
        }
    }
    std::sort(proto->req->layers.begin(), proto->req->layers.end(),
              [](const std::shared_ptr<FkGraphicLayer> &l,
                 const std::shared_ptr<FkGraphicLayer> &r) {
                  return l->zIndex < r->zIndex;
              });
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostTranslate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostTransProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkTransComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value += proto->value;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetTranslate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetTransProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkTransComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value = proto->value;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostScale(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostScaleProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkScaleComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value.x *= proto->value.x;
    comp->value.y *= proto->value.y;
    comp->value.z *= proto->value.z;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetScale(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetScaleProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkScaleComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value = proto->value;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onPostRotate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostRotateProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkRotateComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    auto result = comp->value.toDouble() + proto->value.toDouble();
    comp->value.num = result * 100000000;
    comp->value.den = 100000000;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetRotate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetRotateProto, p);
    auto itr = layers.find(proto->layer);
    FkAssert(layers.end() != itr, FK_FAIL);

    auto comp = FK_FIND_COMPO(itr->second, FkRotateComponent);
    FkAssert(nullptr != comp, FK_FAIL);
    comp->value = proto->value;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onWithLayer(std::shared_ptr<FkProtocol> p) {
    if (FK_INSTANCE_OF(p, FkReadPixelsProto)) {
        FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkReadPixelsProto, p);
        auto itr = layers.find(proto->layerId);
        if (itr != layers.end()) {
            proto->layer = std::make_shared<FkGraphicLayer>(*itr->second);
        }
    } else {
        FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkMeasureTransProto, p);
        if (proto->layerId != Fk_CANVAS_ID) {
            auto itr = layers.find(proto->layerId);
            FkAssert(layers.end() != itr, FK_FAIL);
            proto->layer = std::make_shared<FkGraphicLayer>(*itr->second);
        }
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onDrawPoint(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkDrawPointProto, p);
    auto itr = layers.find(proto->layer);
    if (layers.end() != itr) {
        auto sizeCompo = FK_FIND_COMPO(itr->second, FkSizeCompo);
        auto comp = std::make_shared<FkPointFCompo>();
        comp->value.x = proto->value.x * 1.0f / (sizeCompo->size.getWidth() / 2.0f);
        comp->value.y = proto->value.y * 1.0f / (sizeCompo->size.getHeight() / 2.0f);
        comp->value.z = 0.0f;
        comp->size = proto->size;
        comp->color = proto->color;
        itr->second->addComponent(comp);
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onQueryLayers(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkQueryLayersProto, p);
    for (auto &itr: layers) {
        auto layer = itr.second;
        std::shared_ptr<FkGraphicLayer> copyLayer = nullptr;
        if (layer->projLayerId != FK_ID_NONE) {
            auto srcLayer = _findLayer(layer->projLayerId);
            if (srcLayer) {
                copyLayer = std::make_shared<FkGraphicLayer>(*srcLayer);
                copyLayer->id = layer->id;
                copyLayer->projLayerId = layer->projLayerId;
                copyLayer->copyComponentFrom(layer, FkSizeCompo_Class::type);
            }
        } else {
            copyLayer = std::make_shared<FkGraphicLayer>(*layer);
        }
        if (copyLayer) {
            proto->layers.emplace_back(copyLayer);
        }
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onQueryLayer(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkQueryLayerProto, p);
    auto itr = layers.find(proto->layerId);
    if (layers.end() != itr) {
        auto layer = itr->second;
        proto->layer = std::make_shared<FkGraphicLayer>(*layer);
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onCrop(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkCropProto, p);
    auto itr = layers.find(proto->layerId);
    if (layers.end() != itr) {
        auto layer = itr->second;
        auto src = FK_FIND_COMPO(layer, FkScaleComponent);
        auto dst = FK_FIND_COMPO(proto->canvas, FkScaleComponent);
        if (src && dst) {
            dst->value.x *= src->value.x;
            dst->value.y *= src->value.y;
            dst->value.z *= src->value.z;
            src->value.set(1.0f);
        }
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkGraphicLayerQuark::_onCropLayer(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkCropLayerProto, p);
    auto itr = layers.find(proto->layerId);
    if (layers.end() != itr) {
        auto layer = itr->second;
        auto compo = FK_FIND_COMPO(layer, FkCropComponent);
        if (compo) {
            compo->rect = proto->rect;
        } else {
            compo = std::make_shared<FkCropComponent>(proto->rect);
            layer->addComponent(compo);
        }
        return FK_OK;
    }
    return FK_FAIL;
}

FkFloatVec3 FkGraphicLayerQuark::_calcScaleType(FkSize &src,
                                                FkSize &dst,
                                                kScaleType scaleType) {
    auto scale = FkGraphicLayer::calcScaleWithScaleType(src, dst, scaleType);
    return FkFloatVec3(scale, scale, 1.0f);
}

FkColor &FkGraphicLayerQuark::_updateLayerColor(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto,
                                                std::shared_ptr<FkGraphicLayer> &layer) {
    auto colorComp = FK_FIND_COMPO(proto->layer, FkColorCompo);
    if (nullptr != colorComp) {
        layer->addComponent(colorComp);
    } else {
        colorComp = std::make_shared<FkColorCompo>(FkColor::black());
    }
    return colorComp->color;
}

FkSize &FkGraphicLayerQuark::_updateLayerSize(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto,
                                              std::shared_ptr<FkGraphicLayer> &layer,
                                              FkSize reqSize,
                                              bool isSwappedWH) {
    auto layerSizeComp = FK_FIND_COMPO(layer, FkSizeCompo);
    if (layerSizeComp == nullptr) {
        layerSizeComp = std::make_shared<FkSizeCompo>(FkSize(0, 0));
        layer->addComponent(layerSizeComp);
    }
    if (!reqSize.isZero()) {
        layerSizeComp->size = reqSize;
    }
    auto scaleComp = FK_FIND_COMPO(layer, FkScaleComponent);
    if (!proto->winSize.isZero() &&  nullptr != scaleComp) {
        auto src = layerSizeComp->size;
        if (isSwappedWH) {
            src.swap();
        }
        scaleComp->value = _calcScaleType(src, proto->winSize, proto->scaleType);
    }
    return layerSizeComp->size;
}

void FkGraphicLayerQuark::_withCanvasSize(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto) {
    auto itr = layers.find(Fk_CANVAS_ID);
    if (layers.end() != itr) {
        auto canvas = itr->second;
        auto canvasSizeCompo = FK_FIND_COMPO(canvas, FkSizeCompo);
        if (nullptr != canvasSizeCompo) {
            proto->winSize = canvasSizeCompo->size;
        }
    }
}

FkResult FkGraphicLayerQuark::_onUpdateScaleType(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkScaleTypeProto, p);
    auto itr = layers.find(proto->layerId);
    if (itr == layers.end()) {
        return FK_SOURCE_NOT_FOUND;
    }
    auto layer = itr->second;
    auto scaleComp = FK_FIND_COMPO(layer, FkScaleComponent);
    auto sizeCompo = FK_FIND_COMPO(layer, FkSizeCompo);
    if (!proto->winSize.isZero() &&  scaleComp != nullptr &&  sizeCompo != nullptr) {
        scaleComp->value = _calcScaleType(sizeCompo->size, proto->winSize, proto->scaleType);
        return FK_OK;
    }
    return FK_FAIL;
}

void FkGraphicLayerQuark::_updateLayerByEncodeOrigin(std::shared_ptr<FkGraphicLayer> &layer,
                                                     int32_t decodedOrigin) {
    auto scaleCompo = FK_FIND_COMPO(layer, FkScaleComponent);
    auto rotateCompo = FK_FIND_COMPO(layer, FkRotateComponent);
    switch ((FkEncodedOrigin) decodedOrigin) {
        case FkEncodedOrigin::kTopRight: { // Reflected across y-axis
            scaleCompo->value.y = -scaleCompo->value.y;
            break;
        }
        case FkEncodedOrigin::kBottomRight: { // Rotated 180
            rotateCompo->value = FkRational(-M_PI * 100000000 , 100000000);
            break;
        }
        case FkEncodedOrigin::kBottomLeft: { // Reflected across x-axis
            scaleCompo->value.x = -scaleCompo->value.x;
            break;
        }
        case FkEncodedOrigin::kLeftTop: { // Reflected across x-axis, Rotated 90 CCW. Swapped width height
            scaleCompo->value.x = -scaleCompo->value.x;
            rotateCompo->value = FkRational(M_PI * 50000000 , 100000000);
            break;
        }
        case FkEncodedOrigin::kRightTop: { // Rotated 90 CW. Swapped width height
            rotateCompo->value = FkRational(-M_PI * 50000000 , 100000000);
            break;
        }
        case FkEncodedOrigin::kRightBottom: { // Reflected across x-axis, Rotated 90 CW. Swapped width height
            scaleCompo->value.x = -scaleCompo->value.x;
            rotateCompo->value = FkRational(-M_PI * 50000000 , 100000000);
            break;
        }
        case FkEncodedOrigin::kLeftBottom: { // Rotated 90 CCW. Swapped width height
            rotateCompo->value = FkRational(M_PI * 50000000 , 100000000);
            break;
        }

    }
}

FkResult FkGraphicLayerQuark::_onDrawPath(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkDrawPathProto, p);
    auto itr = layers.find(proto->layerId);
    if (itr == layers.end()) {
        return FK_SOURCE_NOT_FOUND;
    }
    auto layer = itr->second;
    if (curPathCompo == nullptr && proto->isFinish) {
        return FK_INVALID_STATE;
    }
    float sensitivity = proto->scaleOfSensitivity;
    auto sizeCompo = FK_FIND_COMPO(layer, FkSizeCompo);
    auto scaleCompo = FK_FIND_COMPO(layer, FkScaleComponent);
    if (scaleCompo) {
        sensitivity *= scaleCompo->value.x;
    }
    sensitivity = 7.0f / sensitivity;
    if (curPathCompo == nullptr) {
        FkAssert(proto->paint != nullptr, FK_INVALID_STATE);
        auto path = std::make_shared<FkMeshPath>(proto->paint->strokeWidth,
                                                 std::min(sensitivity, 1.0f));
        curPathCompo = std::make_shared<FkPathCompo>(path, FkColor::makeFrom(proto->paint->color));
        layer->addComponent(curPathCompo);
    }
    if (proto->isFinish) {
        curPathCompo->finish();
        curPathCompo = nullptr;
    } else {
        curPathCompo->addPoint(proto->point.x, proto->point.y);
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetProjection(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetProjectionProto, p);
    auto srcLayer = _findLayer(proto->srcLayerId);
    FkAssert(srcLayer != nullptr, FK_FAIL);
    auto dstLayer = _findLayer(proto->dstLayerId);
    FkAssert(dstLayer != nullptr, FK_FAIL);
    dstLayer->projLayerId = srcLayer->id;
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onUpdateLayerWithModel(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkUpdateLayerModelProto, p);
    auto itr = layers.find(proto->modelInterface->getLayerID());
    FkAssert(layers.end() != itr, FK_FAIL);
    auto layer = itr->second;

    auto transCompo = FK_FIND_COMPO(layer, FkTransComponent);
    if (nullptr != transCompo) {
        auto trans = proto->modelInterface->getTranslate();
        transCompo->value = FkDoubleVec2(trans.x, trans.y);
    }
    auto scaleCompo = FK_FIND_COMPO(layer, FkScaleComponent);
    if (nullptr != scaleCompo) {
        scaleCompo->value = proto->modelInterface->getScale();
    }
    auto rotateCompo = FK_FIND_COMPO(layer, FkRotateComponent);
    if (nullptr != rotateCompo) {
        rotateCompo->value = proto->modelInterface->getRotation();
    }
    std::vector<std::shared_ptr<FkPath>> paths;
    std::vector<std::shared_ptr<FkPaint>> paints;
    auto size = proto->modelInterface->getPaintPaths(paths, paints);
    if (size == paths.size() && size == paints.size()) {
        for (int i = 0; i < size; ++i) {
            auto compo = std::make_shared<FkPathCompo>(paths[i], FkColor::makeFrom(paints[i]->color));
            layer->addComponent(compo);
        }
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetZIndex(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkSetZIndexProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer) {
        layer->zIndex = proto->zIndex;
        return FK_OK;
    }
    return FK_SKIP;
}

FkResult FkGraphicLayerQuark::_onUpdateLayerWithSource(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetSourceProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer && proto->source) {
        auto compo = std::make_shared<FkImageSourceCompo>();
        compo->value = proto->source;
        layer->addComponent(compo);
        if (!proto->filePath.empty()) {
            layer->addComponent(std::make_shared<FkFilePathCompo>(proto->filePath));
        }
        FK_CAST_NULLABLE_PTR_RETURN_INT(proto0, FkGraphicUpdateLayerPrt, p);
        _updateLayerSize(proto0, layer,proto->source->getSize(), true);

        auto context = std::dynamic_pointer_cast<FkImageContext>(getContext());
        FkAssert(context != nullptr, FK_NPE);
        auto renderEngine = context->getRenderEngine();
        FkAssert(renderEngine != nullptr, FK_NPE);
        auto ret = renderEngine->updateMaterialWithSource(layer->material, proto->source);
        FkAssert(FK_OK == ret, ret);
        return ret;
    }
    return FK_SKIP;
}

FkResult FkGraphicLayerQuark::_onUpdateLayerWithPlugin(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkAddPluginProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer) {
        auto compo = std::make_shared<FkPluginCompo>();
        compo->value = proto->value;
        if (!compo->value->describeRequirements().contains(FkPluginRequirements::Type::THREAD_GL)) {
            compo->value->onCreate();
        }
        layer->addComponent(compo);
    }
    return FK_SKIP;
}

FkResult FkGraphicLayerQuark::_onSetBackgroundColor(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetBackgroundColorProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer) {
        layer->addComponent(std::make_shared<FkBackgroundColorCompo>(*proto->value));
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetVisibility(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetVisibilityProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer) {
        layer->visibility = proto->visibility;
    }
    return FK_OK;
}

FkResult FkGraphicLayerQuark::_onSetProperties(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetPropertiesProto, p);
    auto layer = _findLayer(proto->layerId);
    if (layer && !proto->properties.empty()) {
        for (auto &itr : proto->properties) {
            if (itr.first == "ZIndex") {
                layer->zIndex = itr.second.getInt32();
            } else if (itr.first == "Visibility") {
                layer->visibility = static_cast<kVisibility>(itr.second.getInt32());
            }
        }
    }
    return FK_OK;
}