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
#include "FkJavaRegister.h"
#include "FkJniDefinition.h"
#include "../source/Java_FkYUV420SPImageSource.h"
#include "../source/Java_FkCompressedImageSource.h"
#include "../source/Java_FkNativeTestObject.h"
#include "../source/Java_FkBitmapSource.h"
#include "../source/Java_FkSurfaceTextureSource.h"

namespace film_k {
    const char *TAG = "FkJavaRegister";

    void jni_register_all(JNIEnv *env) {
        Java_FkYUV420SPImageSource::reg(env);
        Java_FkCompressedImageSource::reg(env);
        Java_FkNativeTestObject::reg(env);
        Java_FkBitmapSource::reg(env);
        Java_FkSurfaceTextureSource::reg(env);
    }
}
