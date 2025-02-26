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
#include "Java_FkImage.h"
#include "FkJniDefinition.h"
#include "FkJavaRuntime.h"

namespace Java_FkImage {
#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * +--------------------------------------------+
     * |  START of internal interface. DO NOT EDIT  |
     * +--------------------------------------------+
     */
    jint _nativeDrawPathFinish(JNIEnv *env, jclass cls, jlong handle, jint layerId) {
        return (jint)nativeDrawPathFinish(env, cls, handle, layerId);
    }

    jint _nativeDrawPath(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint x, jint y, jbyteArray paintData) {
        return (jint)nativeDrawPath(env, cls, handle, layerId, x, y, FkJBuffer::create(env, paintData));
    }

    jint _nativeSave(JNIEnv *env, jclass cls, jlong handle, jstring file, jobject listener) {
        return (jint)nativeSave(env, cls, handle, FkJString::create(env, file), listener);
    }

    jint _nativeCropLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint left, jint top, jint right, jint bottom) {
        return (jint)nativeCropLayer(env, cls, handle, layerId, left, top, right, bottom);
    }

    jint _nativeCrop(JNIEnv *env, jclass cls, jlong handle, jint left, jint top, jint right, jint bottom) {
        return (jint)nativeCrop(env, cls, handle, left, top, right, bottom);
    }

    jint _nativeDrawPoint(JNIEnv *env, jclass cls, jlong handle, jint layerId, jlong color, jint size, jint x, jint y) {
        return (jint)nativeDrawPoint(env, cls, handle, layerId, color, size, x, y);
    }

    jint _nativePostRotation(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint num, jint den) {
        return (jint)nativePostRotation(env, cls, handle, layerId, num, den);
    }

    jint _nativePostScale(JNIEnv *env, jclass cls, jlong handle, jint layerId, jfloat dx, jfloat dy) {
        return (jint)nativePostScale(env, cls, handle, layerId, dx, dy);
    }

    jint _nativePostTranslate(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint dx, jint dy) {
        return (jint)nativePostTranslate(env, cls, handle, layerId, dx, dy);
    }

    jint _nativeNotifyRender(JNIEnv *env, jclass cls, jlong handle) {
        return (jint)nativeNotifyRender(env, cls, handle);
    }

    jint _nativeSetCanvasSize(JNIEnv *env, jclass cls, jlong handle, jint width, jint height) {
        return (jint)nativeSetCanvasSize(env, cls, handle, width, height);
    }

    jint _nativeRemoveLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId) {
        return (jint)nativeRemoveLayer(env, cls, handle, layerId);
    }

    jint _nativeSetProjectionLayer(JNIEnv *env, jclass cls, jlong handle, jint layerId, jint srcLayerId) {
        return (jint)nativeSetProjectionLayer(env, cls, handle, layerId, srcLayerId);
    }

    jint _nativeNewLayerWithSource(JNIEnv *env, jclass cls, jlong handle, jlong sourceHandle) {
        return (jint)nativeNewLayerWithSource(env, cls, handle, sourceHandle);
    }

    jint _nativeNewLayerWithColor(JNIEnv *env, jclass cls, jlong handle, jint width, jint height, jint red, jint green, jint blue, jint alpha) {
        return (jint)nativeNewLayerWithColor(env, cls, handle, width, height, red, green, blue, alpha);
    }

    jint _nativeNewLayerWithFile(JNIEnv *env, jclass cls, jlong handle, jstring path) {
        return (jint)nativeNewLayerWithFile(env, cls, handle, FkJString::create(env, path));
    }

    jint _nativeSetSurface(JNIEnv *env, jclass cls, jlong handle, jobject surface, jint scaleType) {
        return (jint)nativeSetSurface(env, cls, handle, surface, scaleType);
    }

    void _nativeStop(JNIEnv *env, jclass cls, jlong handle) {
        nativeStop(env, cls, handle);
    }

    void _nativeStart(JNIEnv *env, jclass cls, jlong handle) {
        nativeStart(env, cls, handle);
    }

    void _nativeDestroy(JNIEnv *env, jclass cls, jlong handle) {
        nativeDestroy(env, cls, handle);
    }

    void _nativeCreate(JNIEnv *env, jclass cls, jlong handle) {
        nativeCreate(env, cls, handle);
    }

    jlong _nativeCreateInstance(JNIEnv *env, jclass cls, jstring workspace) {
        return (jlong)nativeCreateInstance(env, cls, FkJString::create(env, workspace));
    }

    /**
     * +------------------------------------------+
     * |  END of internal interface. DO NOT EDIT  |
     * +------------------------------------------+
     */

    bool reg(JNIEnv *env) {
        return FkJavaRuntime::jniRegister(env, CLASS_NAME, METHODS, METHODS_NUM);
    }

    const char *CLASS_NAME = "com/alimin/fk/engine/FkImage";

    const int METHODS_NUM = 22;

    const JNINativeMethod METHODS[] = {
            {"nativeDrawPathFinish", "(JI)I", (void *) _nativeDrawPathFinish},
            {"nativeDrawPath", "(JIII[B)I", (void *) _nativeDrawPath},
            {"nativeSave", "(JLjava/lang/String;Lcom/alimin/fk/engine/FkNativeMsgListener;)I", (void *) _nativeSave},
            {"nativeCropLayer", "(JIIIII)I", (void *) _nativeCropLayer},
            {"nativeCrop", "(JIIII)I", (void *) _nativeCrop},
            {"nativeDrawPoint", "(JIJIII)I", (void *) _nativeDrawPoint},
            {"nativePostRotation", "(JIII)I", (void *) _nativePostRotation},
            {"nativePostScale", "(JIFF)I", (void *) _nativePostScale},
            {"nativePostTranslate", "(JIII)I", (void *) _nativePostTranslate},
            {"nativeNotifyRender", "(J)I", (void *) _nativeNotifyRender},
            {"nativeSetCanvasSize", "(JII)I", (void *) _nativeSetCanvasSize},
            {"nativeRemoveLayer", "(JI)I", (void *) _nativeRemoveLayer},
            {"nativeSetProjectionLayer", "(JII)I", (void *) _nativeSetProjectionLayer},
            {"nativeNewLayerWithSource", "(JJ)I", (void *) _nativeNewLayerWithSource},
            {"nativeNewLayerWithColor", "(JIIIIII)I", (void *) _nativeNewLayerWithColor},
            {"nativeNewLayerWithFile", "(JLjava/lang/String;)I", (void *) _nativeNewLayerWithFile},
            {"nativeSetSurface", "(JLandroid/view/Surface;I)I", (void *) _nativeSetSurface},
            {"nativeStop", "(J)V", (void *) _nativeStop},
            {"nativeStart", "(J)V", (void *) _nativeStart},
            {"nativeDestroy", "(J)V", (void *) _nativeDestroy},
            {"nativeCreate", "(J)V", (void *) _nativeCreate},
            {"nativeCreateInstance", "(Ljava/lang/String;)J", (void *) _nativeCreateInstance},
    };

#ifdef __cplusplus
    }
#endif
};