/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns)
* CREATE TIME: 2025-2-22 22:24:54
*/

#include "FkJValues.h"
#include "FkJavaRuntime.h"

std::shared_ptr<FkJString> FkJString::create(JNIEnv *env, jstring obj) {
    return std::make_shared<FkJString>(env, obj);
}

FkJString::FkJString(JNIEnv *env, jstring obj) : FkJValue() {
    this->_jVal = obj;
    this->_cVal = std::string(env->GetStringUTFChars(obj, nullptr));
}

FkJString::FkJString(const FkJString &o) {
    JNIEnv *env = nullptr;
    if (_jVal && FkJavaRuntime::getInstance().findEnv(&env)) {
        env->DeleteLocalRef(_jVal);
    }
}

FkJString::~FkJString() {

}

std::shared_ptr<FkJBuffer> FkJBuffer::create(JNIEnv *env, jbyteArray obj) {
    return std::make_shared<FkJBuffer>(env, obj);
}

FkJBuffer::FkJBuffer(JNIEnv *env, jbyteArray obj) : FkJValue() {
    this->_jVal = obj;
    auto ptr = env->GetByteArrayElements(obj, nullptr);
    auto size = env->GetArrayLength(obj);
    this->_cVal = FkBuffer::wrap((uint8_t *) ptr, size);
}

FkJBuffer::FkJBuffer(const FkJBuffer &o) {
    JNIEnv *env = nullptr;
    if (_jVal && FkJavaRuntime::getInstance().findEnv(&env)) {
        env->DeleteLocalRef(_jVal);
    }
}

FkJBuffer::~FkJBuffer() {

}
