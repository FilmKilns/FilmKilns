/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicCanvasQuark.h"
#include "FkSizeCompo.h"
#include "FkQuerySizeProto.h"
#include "FkMeasureTransProto.h"
#include "FkMeasurePointProto.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"
#include "FkGraphicNewLayerPrt.h"
#include "FkRenderRequestPrt.h"
#include "FkLayerSetTransProto.h"
#include "FkLayerSetRotateProto.h"
#include "FkLayerSetScaleProto.h"
#include "FkCropProto.h"
#include "FkRemoveLayerProto.h"
#include "FkDrawPathProto.h"
#include "FkUpdateLayerModelProto.h"
#include "FkLayerSetProjectionProto.h"
#include "FkCropLayerProto.h"
#include "FkLayerSetSourceProto.h"

FK_IMPL_CLASS_TYPE(FkGraphicCanvasQuark, FkGraphicLayerQuark)

FkGraphicCanvasQuark::FkGraphicCanvasQuark() : FkGraphicLayerQuark() {

}

FkGraphicCanvasQuark::~FkGraphicCanvasQuark() {

}

void FkGraphicCanvasQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FkGraphicLayerQuark::describeProtocols(desc);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewLayerPrt, FkGraphicCanvasQuark::_onDelivery);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicCanvasQuark::_onPostTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetTransProto, FkGraphicCanvasQuark::_onSetTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicCanvasQuark::_onPostScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetScaleProto, FkGraphicCanvasQuark::_onSetScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicCanvasQuark::_onPostRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetRotateProto, FkGraphicCanvasQuark::_onSetRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicCanvasQuark::_onWithCanvas);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasurePointProto, FkGraphicCanvasQuark::_onWithCanvas);
    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicCanvasQuark::_onQueryCanvasSize);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicCanvasQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPathProto, FkGraphicCanvasQuark::_onDrawPath);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateLayerModelProto, FkGraphicCanvasQuark::_onDelivery);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetProjectionProto, FkGraphicCanvasQuark::_onDelivery);
    FK_PORT_DESC_QUICK_ADD(desc, FkCropLayerProto, FkGraphicCanvasQuark::_onDelivery);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerSetSourceProto, FkGraphicCanvasQuark::_onQueryCanvasSize);

}

FkResult FkGraphicCanvasQuark::onCreate() {
    return FkGraphicLayerQuark::onCreate();
}

FkResult FkGraphicCanvasQuark::onDestroy() {
    return FkGraphicLayerQuark::onDestroy();
}

FkResult FkGraphicCanvasQuark::onStart() {
    _initializeCanvas();
    return FkGraphicLayerQuark::onStart();
}

FkResult FkGraphicCanvasQuark::onStop() {
    return FkGraphicLayerQuark::onStop();
}

FkResult FkGraphicCanvasQuark::_onRenderRequest(const std::shared_ptr<FkProtocol> &p) {
    auto sizeCompo = FK_FIND_COMPO(_getCanvas(), FkSizeCompo);
    if (sizeCompo == nullptr || sizeCompo->size.isZero()) {
        FkLogE(FK_DEF_TAG, "Empty canvas.");
        return FK_EMPTY_CANVAS;
    }
    return FkGraphicLayerQuark::_onRenderRequest(p);
}

FkResult FkGraphicCanvasQuark::_onPostTranslate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostTransProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onPostTranslate(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onSetTranslate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetTransProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onSetTranslate(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onPostScale(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostScaleProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onPostScale(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onSetScale(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetScaleProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onSetScale(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onPostRotate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerPostRotateProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onPostRotate(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onSetRotate(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkLayerSetRotateProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    auto ret = FkGraphicLayerQuark::_onSetRotate(p);
    if (FK_OK == ret) {
        ret = FK_DONE;
    }
    return ret;
}

FkResult FkGraphicCanvasQuark::_onQueryCanvasSize(std::shared_ptr<FkProtocol> &p) {
    if (FK_INSTANCE_OF(p, FkLayerSetSourceProto)) {
        auto proto = Fk_POINTER_CAST(FkLayerSetSourceProto, p);
        if (proto) {
            proto->winSize = _getCanvas()->getSize();
        }
        return FK_OK;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkQuerySizeProto, p);
    auto compo = FK_FIND_COMPO(_getCanvas(), FkSizeCompo);
    if (compo) {
        proto->value = compo->size;
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onWithCanvas(std::shared_ptr<FkProtocol> &p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    proto->canvas = std::make_shared<FkGraphicLayer>(*_getCanvas());
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onDrawPath(std::shared_ptr<FkProtocol> &p) {
    auto proto = Fk_POINTER_CAST(FkDrawPathProto, p);
    proto->scaleOfSensitivity = _getCanvas()->getScale().x;
    return FK_OK;
}

std::shared_ptr<FkGraphicLayer> FkGraphicCanvasQuark::_getCanvas() {
    auto itr = layers.find(Fk_CANVAS_ID);
    FkAssert(layers.end() != itr, nullptr);
    return itr->second;
}

FkResult FkGraphicCanvasQuark::_initializeCanvas() {
    if (layers.empty()) {
        auto canvas = newLayerEntity();
        auto ret = canvas->id != FK_ID_NONE;
        canvas->id = Fk_CANVAS_ID;
        layers.emplace(std::make_pair(Fk_CANVAS_ID, canvas));
        return ret ? FK_OK : FK_FAIL;
    }
    return FK_FAIL;
}

FkResult FkGraphicCanvasQuark::_onCrop(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkCropProto, p);
    proto->canvas = _getCanvas();
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onDelivery(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onRemoveLayer(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRemoveLayerProto, p);
    if (proto->layerId == Fk_CANVAS_ID) {
        auto ret = FkGraphicLayerQuark::_onRemoveLayer(p);
        _initializeCanvas();
        return ret;
    }
    return _onDelivery(p);
}
