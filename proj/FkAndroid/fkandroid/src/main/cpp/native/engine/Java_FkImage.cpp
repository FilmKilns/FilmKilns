/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "Java_FkImage.h"
#include "FkJniDefinition.h"
#include "FkImageEngine.h"
#include "FkGraphicWindow.h"
#include "FkRenderEngine.h"
#include "FkInstanceHolder.h"
#include "FkAndroidWindow.h"
#include "FkPaintInfo.pb.h"
#include "FkJavaFunc.h"
#include "FkJavaRuntime.h"
#include "FkJniGlobalRef.h"
#include "FkSurfaceTextureSource.h"

#define RENDER_ALIAS "RenderEngine"
#define IMAGE_ENGINE_ALIAS "ImageEngine"

using namespace com::alimin::fk;

namespace Java_FkImage {

    static std::shared_ptr<FkImageEngine> castHandle(jlong handle) {
        return  FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(handle);
    }

    int32_t nativeDrawPathFinish(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->drawPathFinish(layerId);
    }

    int32_t nativeDrawPath(JNIEnv *env, jclass cls, int64_t handle,
                           int32_t layerId, int32_t x, int32_t y,
                           const std::shared_ptr<FkJBuffer> &paintData) {
        Fk_CHECK_NULL(paintData);
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        auto paintInfo = std::make_shared<pb::FkPaintInfo>();
        if (!paintInfo->ParseFromArray(paintData->cVal()->data(), paintData->cVal()->capacity())) {
            FkLogE(FK_DEF_TAG, "Parse paint info fail.");
            return FK_IO_FAIL;
        }
        auto paint = std::make_shared<FkPaint>();
        paint->strokeWidth = paintInfo->strokewidth();
        paint->color = paintInfo->color();
        return engine->drawPath(layerId, x, y, paint);
    }

    int32_t nativeSave(JNIEnv *env, jclass cls, int64_t handle,
                       const std::shared_ptr<FkJString> &file, jobject listener) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        auto lRef = std::make_shared<FkJniGlobalRef>(listener);
        auto callback = [lRef](int ret) {
            JNIEnv *env = nullptr;
            if (FkJavaRuntime::getInstance().findEnv2(&env)) {
                FkJavaFunc::makeNativeMsgListener(env, lRef->obj())->call(env, lRef->obj(), 0, ret, NULL, NULL);
            }
        };
        auto _file = file->cVal();
        auto ret = engine->save(_file, callback);
        return ret;
    }

    int32_t nativeCropLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t left,
                            int32_t top, int32_t right, int32_t bottom) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        FkIntRect rect(left, top, right, bottom);
        return engine->cropLayer(layerId, rect);
    }

    int32_t nativeCrop(JNIEnv *env, jclass cls, int64_t handle,
                       int32_t left, int32_t top, int32_t right, int32_t bottom) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        FkIntRect rect(left, top, right, bottom);
        return engine->crop(rect);
    }

    int32_t nativeDrawPoint(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int64_t color,
                            int32_t size, int32_t x, int32_t y) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->drawPoint(layerId, FkColor::makeFrom(color), size, x, y);
    }

    int32_t nativePostRotation(JNIEnv *env, jclass cls, int64_t handle,
                               int32_t layerId, int32_t num, int32_t den) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        FkRational rational(num, den);
        return engine->postRotation(layerId, rational);
    }

    int32_t nativePostScale(JNIEnv *env, jclass cls, int64_t handle,
                            int32_t layerId, float dx, float dy) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->postScale(layerId, dx, dy);
    }

    int32_t nativePostTranslate(JNIEnv *env, jclass cls, int64_t handle,
                                int32_t layerId, int32_t dx, int32_t dy) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->postTranslate(layerId, dx, dy);
    }

    int32_t nativeNotifyRender(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->notifyRender();
    }

    int32_t nativeSetCanvasSize(JNIEnv *env, jclass cls, int64_t handle,
                                int32_t width, int32_t height) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->setCanvasSize(FkSize(width, height));
    }

    int32_t nativeRemoveLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->removeLayer(layerId);
    }

    int32_t nativeSetProjectionLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId,
                                     int32_t srcLayerId) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        return engine->setProjectionLayer(layerId, srcLayerId);
    }

    int32_t nativeNewLayerWithSource(JNIEnv *env, jclass cls, int64_t handle,
                                     int64_t sourceHandle) {
        if (sourceHandle == 0) {
            return FK_INVALID_PARAMETERS;
        }
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        auto source = FkInstanceHolder::getInstance().find<std::shared_ptr<FkAbsImageSource>>(sourceHandle);
        return engine->newLayerWithSource(source);
    }

    int32_t nativeNewLayerWithColor(JNIEnv *env, jclass cls, int64_t handle,
                                    int32_t width, int32_t height,
                                    int32_t red, int32_t green, int32_t blue, int32_t alpha) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        FkColor color = FkColor::makeFromRGBA8(red, green, blue, alpha);
        color.setAlphaType(FkColor::AlphaType::kPreMultiple);
        return engine->newLayerWithColor(FkSize(width, height), color);
    }

    int32_t nativeNewLayerWithFile(JNIEnv *env, jclass cls, int64_t handle,
                                   const std::shared_ptr<FkJString> &path) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        auto _path = path->cVal();
        auto layer = engine->newLayerWithFile(_path);
        return layer;
    }

    int32_t nativeSetSurface(JNIEnv *env, jclass cls, int64_t handle,
                             jobject surface, int32_t scaleType) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL(engine);
        if (surface) {
            auto win = std::make_shared<FkAndroidWindow>(surface);
            return engine->setSurface(win, scaleType);
        } else {
            return engine->setSurface(nullptr, scaleType);
        }
    }

    void nativeStop(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
        engine->stop();
    }

    void nativeStart(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
        engine->start();
    }

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
        engine->destroy();
        FkInstanceHolder::getInstance().release(handle);
    }

    void nativeCreate(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
        engine->create();
    }

    int64_t nativeCreateInstance(JNIEnv *env, jclass cls,
                                 const std::shared_ptr<FkJString> &workspace) {
        auto _workspace = workspace->cVal();
        std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>(RENDER_ALIAS);
        auto imageEngine = std::make_shared<FkImageEngine>(renderEngine, _workspace, IMAGE_ENGINE_ALIAS);
        return FkInstanceHolder::getInstance().put(imageEngine);
    }

}