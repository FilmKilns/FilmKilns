/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkImageEngine.h"
#include "FkGraphicLayer.h"
#include "FkBitmap.h"
#include "FkDefinition.h"
#include "FkSizeCompo.h"
#include "FkFilePathCompo.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkBitmapCompo.h"
#include "FkFileUtils.h"
#include "FkString.h"
#include "FkReadPixelsProto.h"
#include "FkAnyCompo.h"
#include "FkLayerSetSourceProto.h"

FK_IMPL_CLASS_TYPE(FkImageEngine, FkLayerEngine)

FkImageEngine::FkImageEngine(std::shared_ptr<FkEngine> &renderEngine,
                             std::string &workspace,
                             std::string name)
        : FkLayerEngine(renderEngine, name), workspace(workspace) {

}

FkImageEngine::~FkImageEngine() {
}

FkResult FkImageEngine::onCreate() {
    return FkLayerEngine::onCreate();
}

FkResult FkImageEngine::onDestroy() {
    auto ret = FkLayerEngine::onDestroy();
    workspace.clear();
    return ret;
}

FkResult FkImageEngine::onStart() {
    return FkLayerEngine::onStart();
}

FkResult FkImageEngine::onStop() {
    return FkLayerEngine::onStop();
}

FkID FkImageEngine::newLayerWithFile(std::string path, FkID expectId) {
    if (!FkFileUtils::exist(path)) {
        FkLogE(FK_DEF_TAG, "File not found: %s", path.c_str());
        return FK_ID_NONE;
    }
    auto id = newLayer(expectId);
    if (FK_ID_NONE != id) {
        auto layer = std::make_shared<FkGraphicLayer>();
        layer->id = id;
        auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageEngine::_updateLayerWithFile));
        msg->arg3 = path;
        msg->sp = layer;
        sendMessage(msg);
    }
    return id;
}

FkResult FkImageEngine::_updateLayerWithFile(std::shared_ptr<FkMessage> &msg) {
    auto layer = std::dynamic_pointer_cast<FkGraphicLayer>(msg->sp);
    auto src = msg->arg3;
    FkLogI(FK_DEF_TAG, "add layer: %s", src.c_str());
    std::string name = FkString("layer_")
            .append(layer->id)
            .append(FkFileUtils::suffix(src))
            .toString();
    auto ret = workspace.includeAsSource(src, name);
    if (FK_OK != ret) {
        FkLogI(FK_DEF_TAG, "Copy source file failed with ret=%d", ret);
        return FK_IO_FAIL;
    }
    auto filePath = workspace.newSourceFilePath(name);
    auto bmp = FkBitmap::from(filePath);
    FkAssert(nullptr != bmp, FK_EMPTY_DATA);
    FkSize layerSize(bmp->getWidth(), bmp->getHeight());
    FkSize canvasSize(layerSize);
    if (bmp->isSwappedWH()) {
        canvasSize.swap();
    }
    setCanvasSizeInternal(canvasSize, true);

    layer->addComponent(std::make_shared<FkBitmapCompo>(bmp));

    auto proto = std::make_shared<FkGraphicUpdateLayerPrt>();
    proto->layer = layer;
    proto->layer->addComponent(std::make_shared<FkSizeCompo>(layerSize));
    proto->layer->addComponent(std::make_shared<FkFilePathCompo>(name));
    return getClient()->with(getMolecule())->send(proto);
}

FkResult FkImageEngine::_updateLayerWithSource(const std::shared_ptr<FkMessage> &msg) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(source, FkAbsImageSource, msg->sp);
    auto layerId = msg->arg1;
    auto bmp = source->toBitmap();
    if (bmp) {
        std::string name = FkString("layer_")
                .append(layerId)
                .append(".jpg")
                .toString();
        auto filePath = workspace.newSourceFilePath(name);
        auto ret = bmp->write(filePath, FkImage::Format::kJPEG, 100);
        if (FK_OK != ret) {
            FkLogE(FK_DEF_TAG, "Copy source file failed with ret=%d", ret);
            return FK_IO_FAIL;
        }
        msg->arg3 = name;
    }
    return FkLayerEngine::_updateLayerWithSource(msg);
}

FkID FkImageEngine::save(FkID layerId, std::string file, FkResultCallback callback) {
    auto msg = FkMessage::obtain(FK_WRAP_FUNC(FkImageEngine::_save));
    msg->arg1 = layerId;
    msg->arg3 = file;
    msg->sp = callback ? std::make_shared<FkAnyCompo>(callback) : nullptr;
    return sendMessage(msg);
}

FkResult FkImageEngine::_save(std::shared_ptr<FkMessage> &msg) {
    auto proto = std::make_shared<FkReadPixelsProto>();
    proto->layerId = msg->arg1;
    auto ret = getClient()->with(getMolecule())->send(proto);
    if (FK_OK == ret) {
        if (proto->buf == nullptr) {
            ret = FK_EMPTY_DATA;
        } else {
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
            ret = FkBitmap::write(msg->arg3, fmt, proto->buf, proto->size, 80);
        }
    }
    auto compo = std::dynamic_pointer_cast<FkAnyCompo>(msg->sp);
    if (compo && compo->any.has_value()) {
        std::any_cast<FkResultCallback>(compo->any)(ret);
        return FK_OK;
    }
    return ret;
}
