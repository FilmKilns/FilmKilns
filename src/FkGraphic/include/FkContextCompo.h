/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 23:37:39
*/

#ifndef FK_GRAPHIC_FKCONTEXTCOMPO_H
#define FK_GRAPHIC_FKCONTEXTCOMPO_H

#include "FkComponent.h"
#include "FkGraphicWindow.h"
#include <EGL/egl.h>
#include <vector>

FK_SUPER_CLASS(FkContextCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkContextCompo)

public:
    FkContextCompo(int32_t glVersion, const std::string alias);

    FkContextCompo(const FkContextCompo &o) = delete;

    virtual ~FkContextCompo();

    FkResult makeCurrent();

    FkResult create(std::shared_ptr<FkContextCompo> context = nullptr,
                    std::shared_ptr<FkGraphicWindow> win = nullptr);

    FkResult update(std::shared_ptr<FkGraphicWindow> win);

    FkResult changeSurface(const std::shared_ptr<FkGraphicWindow> &win);

    void destroy();

    FkResult swapBuffers();

    int32_t getWidth();

    int32_t getHeight();

    bool isPBuffer();

private:
    EGLDisplay _createDisplay(EGLNativeDisplayType display_id);

    EGLConfig _createConfig(EGLDisplay display, const int *configSpec);

    EGLContext _createContext(EGLDisplay display, EGLConfig config, EGLContext sharedContext);

    EGLSurface _createPbufferSurface(EGLDisplay display, EGLConfig config);

    EGLSurface _createWindowSurface(EGLDisplay display, EGLConfig config, std::shared_ptr<FkGraphicWindow> win);

    bool _checkError();

    bool _getDefaultConfigures(std::vector<int> &out);

private:
    static const int FK_CONFIG_WIN[];
    static const int FK_CONFIG_BUFFER[];
#ifdef __ANDROID__
    static const int EGL_RECORDABLE_ANDROID;
    static const int FK_CONFIG_ANDROID[];
#endif
    int32_t glVersion;
    std::string alias;
    FkSize size;
    std::shared_ptr<FkGraphicWindow> _win = nullptr;
    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLConfig eglConfig = nullptr;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLSurface eglSurface = EGL_NO_SURFACE;
};

#endif //FK_GRAPHIC_FKCONTEXTCOMPO_H