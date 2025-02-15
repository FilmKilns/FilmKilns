/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICTEXMANAGER_H
#define FK_GRAPHIC_FKGRAPHICTEXMANAGER_H

#include "FkSource.h"
#include "FkDefinition.h"
#include "FkSize.h"
#include "FkColor.h"

FK_SUPER_CLASS(FkTexDescription, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkTexDescription)

public:
    static const uint64_t FLAG_NONE;
    static const uint64_t FLAG_ENABLE_STORAGE;

    FK_ENUM kWrapMode : int32_t {
        REPEAT = 0,
        EDGE,
        BORDER,
    };
    FK_ENUM kFilterMode : int32_t {
        NEAREST = 0,
        LINEAR,
    };
public:
    FkTexDescription();

    FkTexDescription(uint32_t target);

    FkTexDescription(const FkTexDescription &o);

    virtual ~FkTexDescription();

public:
    /// GL_TEXTURE_2D,GL_TEXTURE_EXTERNAL_OES
    uint32_t target = 0;
    /// GL_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER_EXT
    kWrapMode wrapMode = kWrapMode::REPEAT;
    /// GL_CLAMP_TO_BORDER_EXT mode color
    float borderColor[4] = {0.0f};
    /// GL_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER_EXT
    kFilterMode filterMode = kFilterMode::NEAREST;
    FkSize size;
    FkColor::kFormat fmt = FkColor::kFormat::NONE;
    uint64_t flags = FLAG_NONE;
};

FK_SUPER_CLASS(FkGraphicTexture, FkSource) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicTexture)

public:
    static uint32_t convertGLFormat(FkColor::kFormat fmt);

public:
    FkGraphicTexture(const FkTexDescription &desc);

    FkGraphicTexture(const FkGraphicTexture &o) = delete;

    virtual ~FkGraphicTexture();

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    virtual FkResult update(FkColor::kFormat fmt, int32_t width, int32_t height, uint8_t *pixels = nullptr);

    virtual FkResult update(uint8_t *pixels);

public:
    std::atomic_bool applied;
    FkTexDescription desc;
    uint32_t tex;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkGraphicAllocator, FkSourceAllocator)<FkGraphicTexture, FkTexDescription> {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicAllocator)

public:
    FkGraphicAllocator(int capacity);

    FkGraphicAllocator(const FkGraphicTexture &o) = delete;

    ~FkGraphicAllocator();

    virtual FkGraphicTexture *delegateAlloc(FkTexDescription &desc) override;

    virtual bool delegateEquals(FkTexDescription &desc, FkGraphicTexture *tex) override;

};


#endif //FK_GRAPHIC_FKGRAPHICTEXMANAGER_H
