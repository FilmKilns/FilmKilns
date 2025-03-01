/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
*/

#include "Java_FkImageModel.h"
#include "FkJniDefinition.h"
#include "FkImageEngine.h"
#include "FkImageModelEngine.h"
#include "FkInstanceHolder.h"
#include "FkJavaFunc.h"
#include "FkJavaRuntime.h"
#include "FkJniGlobalRef.h"

#define FILE_ENGINE_ALIAS "FileEngine"

using namespace com::alimin::fk;
namespace Java_FkImageModel {

    static std::shared_ptr<FkImageModelEngine> castHandle(jlong handle) {
        return FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageModelEngine>>(handle);
    }

    int32_t nativeGetLayers(JNIEnv *env, jclass cls, int64_t handle, jobject listener) {
        auto engine = castHandle(handle);
        auto lRef = std::make_shared<FkJniGlobalRef>(listener);
        auto callback = [lRef](std::shared_ptr<pb::FkPictureModel> &model) {
            JNIEnv *env = nullptr;
            if (FkJavaRuntime::getInstance().findEnv2(&env)) {
                auto size = model->ByteSizeLong();
                std::vector<uint8_t> vec(size);
                memset(vec.data(), 0, size);
                auto buf = env->NewDirectByteBuffer(vec.data(), size);
                auto ret = model->SerializeToArray(vec.data(), vec.size());
                FkJavaFunc::makeNativeMsgListener(env, lRef->obj())->call(env, lRef->obj(), 0, ret, NULL, buf);
                env->DeleteLocalRef(buf);
            }
        };
        engine->getLayers(callback);
        return FK_OK;
    }

    int32_t nativeLoad(JNIEnv *env, jclass cls, int64_t handle,
                       const std::shared_ptr<FkJString> &file, jobject listener) {
        auto engine = castHandle(handle);
        auto lRef = std::make_shared<FkJniGlobalRef>(listener);
        auto finishCallback = [lRef]() {
            FkJavaRuntime::getInstance().attachThread();
            JNIEnv *env = nullptr;
            if (FkJavaRuntime::getInstance().findEnv(&env)) {
                FkJavaFunc::makeNativeMsgListener(env, lRef->obj())->call(env, lRef->obj(),
                                                                          0, 0, NULL, NULL);
            }
            FkJavaRuntime::getInstance().detachThread();
        };
        auto _file = file->cVal();
        return engine->load(_file, finishCallback);
    }

    int32_t nativeSave(JNIEnv *env, jclass cls, int64_t handle,
                       const std::shared_ptr<FkJString> &file) {
        auto _file = file->cVal();
        auto engine = castHandle(handle);
        return engine->save(_file);
    }

    void nativeStop(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        engine->stop();
    }

    void nativeStart(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        engine->start();
    }

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        engine->destroy();
        FkInstanceHolder::getInstance().release(handle);
    }

    void nativeCreate(JNIEnv *env, jclass cls, int64_t handle) {
        auto engine = castHandle(handle);
        engine->create();
    }

    int64_t nativeCreateInstance(JNIEnv *env, jclass cls, int64_t imageEngineHandle) {
        auto imageEngine = FkInstanceHolder::getInstance().find<std::shared_ptr<FkImageEngine>>(
                imageEngineHandle);
        auto fileEngine = std::make_shared<FkImageModelEngine>(imageEngine, FILE_ENGINE_ALIAS);
        return FkInstanceHolder::getInstance().put(fileEngine);
    }
}
