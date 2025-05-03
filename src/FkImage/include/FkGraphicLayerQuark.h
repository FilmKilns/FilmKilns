/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICLAYERQUARK_H
#define FK_GRAPHIC_FKGRAPHICLAYERQUARK_H

#include "FkQuark.h"
#include "FkGraphicLayer.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkSize.h"
#include "FkColor.h"
#include "FkPathCompo.h"
#include <list>
#include <map>

FK_SUPER_CLASS(FkGraphicLayerQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicLayerQuark)

public:
    FkGraphicLayerQuark();

    FkGraphicLayerQuark(const FkGraphicLayerQuark &o) = delete;

    virtual ~FkGraphicLayerQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onNewLayer(std::shared_ptr<FkProtocol> p);

    FkResult _onWithLayer(std::shared_ptr<FkProtocol> p);

    FkResult _onUpdateScaleType(std::shared_ptr<FkProtocol> &p);

    FkResult _onDrawPath(std::shared_ptr<FkProtocol> &p);

    FkResult _onSetProjection(const std::shared_ptr<FkProtocol> &p);

protected:
    std::shared_ptr<FkGraphicLayer> newLayerEntity();

    FkResult _onUpdateLayer(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onRemoveLayer(std::shared_ptr<FkProtocol> &p);

    virtual FkResult _onRenderRequest(const std::shared_ptr<FkProtocol> &p);

    virtual FkResult _onPostTranslate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetTranslate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onPostScale(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetScale(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onPostRotate(std::shared_ptr<FkProtocol> p);

    virtual FkResult _onSetRotate(std::shared_ptr<FkProtocol> p);

    FkResult _onDrawPoint(std::shared_ptr<FkProtocol> p);

    FkResult _onQueryLayers(std::shared_ptr<FkProtocol> p);

    FkResult _onQueryLayer(std::shared_ptr<FkProtocol> &p);

    virtual FkResult _onCrop(std::shared_ptr<FkProtocol> &p);

    virtual FkResult _onCropLayer(const std::shared_ptr<FkProtocol> &p);

    FkResult _onUpdateLayerWithModel(std::shared_ptr<FkProtocol> &p);

    FkResult _onSetZIndex(const std::shared_ptr<FkProtocol> &p);

    FkResult _onUpdateLayerWithSource(const std::shared_ptr<FkProtocol> &p);

    FkResult _onUpdateLayerWithPlugin(const std::shared_ptr<FkProtocol> &p);

    FkResult _onSetBackgroundColor(const std::shared_ptr<FkProtocol> &p);

    FkResult _onSetVisibility(const std::shared_ptr<FkProtocol> &p);

    FkResult _onSetProperties(const std::shared_ptr<FkProtocol> &p);

private:
    bool _isExistLayer(FkID id);

    FkID _generateId(FkID expectId);

    std::shared_ptr<FkGraphicLayer> _findLayer(FkID layerId);

    FkFloatVec3 _calcScaleType(FkSize &src, FkSize &dst, kScaleType scaleType);

    FkColor &_updateLayerColor(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto, std::shared_ptr<FkGraphicLayer> &layer);

    FkSize &_updateLayerSize(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto,
                             std::shared_ptr<FkGraphicLayer> &layer,
                             FkSize reqSize,
                             bool isSwappedWH);

    void _withCanvasSize(std::shared_ptr<FkGraphicUpdateLayerPrt> &proto);

    void _updateLayerByEncodeOrigin(std::shared_ptr<FkGraphicLayer> &layer, int32_t decodedOrigin);

    void _removeLayer(const std::shared_ptr<FkGraphicLayer> &layer);

    void _clearLayers();

protected:
    std::map<FkID, std::shared_ptr<FkGraphicLayer>> layers;
    std::shared_ptr<FkPathCompo> curPathCompo = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICLAYERQUARK_H
