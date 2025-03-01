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

    int32_t nativeDrawPathFinish(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId) {
        return FK_FAIL;
    }

    int32_t
    nativeDrawPath(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t x, int32_t y,
                   const std::shared_ptr<FkJBuffer> &paintData) {
        return FK_FAIL;
    }

    int32_t
    nativeSave(JNIEnv *env, jclass cls, int64_t handle, const std::shared_ptr<FkJString> &file,
               jobject listener) {
        return FK_FAIL;
    }

    int32_t nativeCropLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t left,
                            int32_t top, int32_t right, int32_t bottom) {
        return FK_FAIL;
    }

    int32_t
    nativeCrop(JNIEnv *env, jclass cls, int64_t handle, int32_t left, int32_t top, int32_t right,
               int32_t bottom) {
        return FK_FAIL;
    }

    int32_t nativeDrawPoint(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int64_t color,
                            int32_t size, int32_t x, int32_t y) {
        return FK_FAIL;
    }

    int32_t
    nativePostRotation(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t num,
                       int32_t den) {
        return FK_FAIL;
    }

    int32_t
    nativePostScale(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, float dx, float dy) {
        return FK_FAIL;
    }

    int32_t
    nativePostTranslate(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t dx,
                        int32_t dy) {
        return FK_FAIL;
    }

    int32_t nativeNotifyRender(JNIEnv *env, jclass cls, int64_t handle) {
        return FK_FAIL;
    }

    int32_t
    nativeSetCanvasSize(JNIEnv *env, jclass cls, int64_t handle, int32_t width, int32_t height) {
        return FK_FAIL;
    }

    int32_t nativeRemoveLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId) {
        return FK_FAIL;
    }

    int32_t nativeSetProjectionLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId,
                                     int32_t srcLayerId) {
        return FK_FAIL;
    }

    int32_t
    nativeNewLayerWithSource(JNIEnv *env, jclass cls, int64_t handle, int64_t sourceHandle) {
        return FK_FAIL;
    }

    int32_t
    nativeNewLayerWithColor(JNIEnv *env, jclass cls, int64_t handle, int32_t width, int32_t height,
                            int32_t red, int32_t green, int32_t blue, int32_t alpha) {
        return FK_FAIL;
    }

    int32_t nativeNewLayerWithFile(JNIEnv *env, jclass cls, int64_t handle,
                                   const std::shared_ptr<FkJString> &path) {
        return FK_FAIL;
    }

    int32_t
    nativeSetSurface(JNIEnv *env, jclass cls, int64_t handle, jobject surface, int32_t scaleType) {
        return FK_FAIL;
    }

    void nativeStop(JNIEnv *env, jclass cls, int64_t handle) {
    }

    void nativeStart(JNIEnv *env, jclass cls, int64_t handle) {
    }

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
    }

    void nativeCreate(JNIEnv *env, jclass cls, int64_t handle) {
    }

    int64_t
    nativeCreateInstance(JNIEnv *env, jclass cls, const std::shared_ptr<FkJString> &workspace) {
        return FK_FAIL;
    }

//static std::shared_ptr<FkImageEngine> castHandle(jlong handle) {
//    return  FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(handle);
//}
//
//JNIEXPORT jlong JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreateInstance
//        (JNIEnv *env, jobject that, jstring workspace) {
//    auto pWorkspace = env->GetStringUTFChars(workspace, nullptr);
//    std::string workspaceStr(pWorkspace);
//    env->ReleaseStringUTFChars(workspace, pWorkspace);
//    std::shared_ptr<FkEngine> renderEngine = std::make_shared<FkRenderEngine>(RENDER_ALIAS);
//    auto imageEngine = std::make_shared<FkImageEngine>(renderEngine, workspaceStr, IMAGE_ENGINE_ALIAS);
//    return FkInstanceHolder::getInstance().put(imageEngine);
//}
//
//JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeCreate
//        (JNIEnv *env, jobject that, jlong handle) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
//    engine->create();
//}
//
//JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeDestroy
//        (JNIEnv *env, jobject that, jlong handle) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
//    engine->destroy();
//    FkInstanceHolder::getInstance().release(handle);
//}
//
//JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStart
//        (JNIEnv *env, jobject that, jlong handle) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
//    engine->start();
//}
//
//JNIEXPORT void JNICALL Java_com_alimin_fk_engine_FkImage_nativeStop
//        (JNIEnv *env, jobject that, jlong handle) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL_WITH_STATEMENT(engine, return);
//    engine->stop();
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetSurface
//        (JNIEnv *env, jobject that, jlong handle, jobject surface, jint scaleType) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    if (surface) {
//        auto win = std::make_shared<FkAndroidWindow>(surface);
//        return engine->setSurface(win, scaleType);
//    } else {
//        return engine->setSurface(nullptr, scaleType);
//    }
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithFile
//        (JNIEnv *env, jobject that, jlong handle, jstring path) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    auto *p = env->GetStringUTFChars(path, nullptr);
//    auto layer = engine->newLayerWithFile(std::string(p));
//    env->ReleaseStringUTFChars(path, p);
//    return layer;
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithColor
//        (JNIEnv *env, jobject that, jlong handle, jint widht, jint height, jint red, jint green,
//         jint blue, jint alpha) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    FkColor color = FkColor::makeFromRGBA8(red, green, blue, alpha);
//    color.setAlphaType(FkColor::AlphaType::kPreMultiple);
//    return engine->newLayerWithColor(FkSize(widht, height), color);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNewLayerWithSource
//        (JNIEnv *env, jobject that, jlong handle, jlong sourceHandle) {
//    if (sourceHandle == 0) {
//        return FK_INVALID_PARAMETERS;
//    }
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    auto source = FkInstanceHolder::getInstance().find<std::shared_ptr<FkAbsImageSource>>(sourceHandle);
//    return engine->newLayerWithSource(source);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetProjectionLayer
//        (JNIEnv *env, jobject that, jlong handle, jint layerId, jint srcLayerId) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->setProjectionLayer(layerId, srcLayerId);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeRemoveLayer
//        (JNIEnv *env, jobject that, jlong handle, jint layerId) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->removeLayer(layerId);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeNotifyRender
//        (JNIEnv *env, jobject that, jlong handle) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->notifyRender();
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSetCanvasSize
//        (JNIEnv *env, jobject that, jlong handle, jint width, jint height) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->setCanvasSize(FkSize(width, height));
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostTranslate
//        (JNIEnv *env, jobject that, jlong handle, jint layer, jint dx, jint dy) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->postTranslate(layer, dx, dy);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostScale
//        (JNIEnv *env, jobject that, jlong handle, jint layer, jfloat dx, jfloat dy) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->postScale(layer, dx, dy);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativePostRotation
//        (JNIEnv *env, jobject that, jlong handle, jint layer, jint num, jint den) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    FkRational rational(num, den);
//    return engine->postRotation(layer, rational);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeDrawPoint
//        (JNIEnv *env, jobject that, jlong handle, jint layer, jlong color, jint size, jint x, jint y) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->drawPoint(layer, FkColor::makeFrom(color), size, x, y);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeCrop
//        (JNIEnv *env, jobject that, jlong handle, jint left, jint top, jint right, jint bottom) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    FkIntRect rect(left, top, right, bottom);
//    return engine->crop(rect);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeCropLayer
//        (JNIEnv *env, jobject that, jlong handle, jint layer, jint left, jint top, jint right, jint bottom) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    FkIntRect rect(left, top, right, bottom);
//    return engine->cropLayer(layer, rect);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeSave
//        (JNIEnv *env, jobject that, jlong handle, jstring file, jobject listener) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    auto *p = env->GetStringUTFChars(file, nullptr);
//    auto lRef = std::make_shared<FkJniGlobalRef>(listener);
//    auto callback = [lRef](int ret) {
//        JNIEnv *env = nullptr;
//        if (FkJavaRuntime::getInstance().findEnv2(&env)) {
//            FkJavaFunc::makeNativeMsgListener(env, lRef->obj())->call(env, lRef->obj(), 0, ret, NULL, NULL);
//        }
//    };
//    auto ret = engine->save(std::string(p), callback);
//    env->ReleaseStringUTFChars(file, p);
//    return ret;
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeDrawPath
//        (JNIEnv *env, jobject that, jlong handle, jint layerId, jint x, jint y, jbyteArray paintData) {
//    if (paintData == nullptr) {
//        return FK_NPE;
//    }
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    auto size = env->GetArrayLength(paintData);
//    auto ptr = env->GetByteArrayElements(paintData, nullptr);
//    if (size == 0 || ptr == nullptr) {
//        return FK_NPE;
//    }
//    auto paintInfo = std::make_shared<pb::FkPaintInfo>();
//    if (!paintInfo->ParseFromArray(ptr, size)) {
//        FkLogE(FK_DEF_TAG, "Parse paint info fail.");
//        return FK_IO_FAIL;
//    }
//    auto paint = std::make_shared<FkPaint>();
//    paint->strokeWidth = paintInfo->strokewidth();
//    paint->color = paintInfo->color();
//    return engine->drawPath(layerId, x, y, paint);
//}
//
//JNIEXPORT jint JNICALL Java_com_alimin_fk_engine_FkImage_nativeDrawPathFinish
//        (JNIEnv *env, jobject that, jlong handle, jint layerId) {
//    auto engine = castHandle(handle);
//    Fk_CHECK_NULL(engine);
//    return engine->drawPathFinish(layerId);
//}

}