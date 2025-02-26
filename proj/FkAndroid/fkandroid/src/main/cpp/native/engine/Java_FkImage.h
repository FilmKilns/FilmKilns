/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
*
* CREATE AUTO. DO NOT EDIT.
*/

/**
* +--------------------------------------------+
* |          CREATE AUTO. DO NOT EDIT          |
* +--------------------------------------------+
*/
#pragma once
#include "FkJniDefinition.h"
#include <any>

namespace Java_FkImage {
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * +----------------------------------------+
     * |  START of public interface. NEED IMPL  |
     * +----------------------------------------+
     */
    int32_t nativeDrawPathFinish(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId);

    int32_t nativeDrawPath(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t x, int32_t y, const std::shared_ptr<FkJBuffer> & paintData);

    int32_t nativeSave(JNIEnv *env, jclass cls, int64_t handle, const std::shared_ptr<FkJString> & file, jobject listener);

    int32_t nativeCropLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t left, int32_t top, int32_t right, int32_t bottom);

    int32_t nativeCrop(JNIEnv *env, jclass cls, int64_t handle, int32_t left, int32_t top, int32_t right, int32_t bottom);

    int32_t nativeDrawPoint(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int64_t color, int32_t size, int32_t x, int32_t y);

    int32_t nativePostRotation(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t num, int32_t den);

    int32_t nativePostScale(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, float dx, float dy);

    int32_t nativePostTranslate(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t dx, int32_t dy);

    int32_t nativeNotifyRender(JNIEnv *env, jclass cls, int64_t handle);

    int32_t nativeSetCanvasSize(JNIEnv *env, jclass cls, int64_t handle, int32_t width, int32_t height);

    int32_t nativeRemoveLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId);

    int32_t nativeSetProjectionLayer(JNIEnv *env, jclass cls, int64_t handle, int32_t layerId, int32_t srcLayerId);

    int32_t nativeNewLayerWithSource(JNIEnv *env, jclass cls, int64_t handle, int64_t sourceHandle);

    int32_t nativeNewLayerWithColor(JNIEnv *env, jclass cls, int64_t handle, int32_t width, int32_t height, int32_t red, int32_t green, int32_t blue, int32_t alpha);

    int32_t nativeNewLayerWithFile(JNIEnv *env, jclass cls, int64_t handle, const std::shared_ptr<FkJString> & path);

    int32_t nativeSetSurface(JNIEnv *env, jclass cls, int64_t handle, jobject surface, int32_t scaleType);

    void nativeStop(JNIEnv *env, jclass cls, int64_t handle);

    void nativeStart(JNIEnv *env, jclass cls, int64_t handle);

    void nativeDestroy(JNIEnv *env, jclass cls, int64_t handle);

    void nativeCreate(JNIEnv *env, jclass cls, int64_t handle);

    int64_t nativeCreateInstance(JNIEnv *env, jclass cls, const std::shared_ptr<FkJString> & workspace);

    /**
     * +--------------------------------------+
     * |  END of public interface. NEED IMPL  |
     * +--------------------------------------+
     */

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    jint _nativeDrawPathFinish(JNIEnv *env, jclass cls, jlong handle, jint layerId);

    jint _nativeDrawPath(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint x, jint y, jbyteArray paintData);

    jint _nativeSave(JNIEnv *env, jclass cls, jlong handle, jstring file, jobject listener);

    jint _nativeCropLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint left, jint top, jint right, jint bottom);

    jint _nativeCrop(JNIEnv *env, jclass cls, jlong handle, jint left, jint top, jint right, jint bottom);

    jint _nativeDrawPoint(JNIEnv *env, jclass cls, jlong handle, jint layerId, jlong color, jint size, jint x, jint y);

    jint _nativePostRotation(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint num, jint den);

    jint _nativePostScale(JNIEnv *env, jclass cls, jlong handle, jint layerId, jfloat dx, jfloat dy);

    jint _nativePostTranslate(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint dx, jint dy);

    jint _nativeNotifyRender(JNIEnv *env, jclass cls, jlong handle);

    jint _nativeSetCanvasSize(JNIEnv *env, jclass cls, jlong handle, jint width, jint height);

    jint _nativeRemoveLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId);

    jint _nativeSetProjectionLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint srcLayerId);

    jint _nativeNewLayerWithSource(JNIEnv *env, jclass cls, jlong handle, jlong sourceHandle);

    jint _nativeNewLayerWithColor(JNIEnv *env, jclass cls, jlong handle, jint width, jint height, jint red, jint green, jint blue, jint alpha);

    jint _nativeNewLayerWithFile(JNIEnv *env, jclass cls, jlong handle, jstring path);

    jint _nativeSetSurface(JNIEnv *env, jclass cls, jlong handle, jobject surface, jint scaleType);

    void _nativeStop(JNIEnv *env, jclass cls, jlong handle);

    void _nativeStart(JNIEnv *env, jclass cls, jlong handle);

    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle);

    void _nativeCreate(JNIEnv *env, jclass cls, jlong handle);

    jlong _nativeCreateInstance(JNIEnv *env, jclass cls, jstring workspace);

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env);

    extern const char *CLASS_NAME;

    extern const int METHODS_NUM;

    extern const JNINativeMethod METHODS[22];

#ifdef __cplusplus
}
#endif
};