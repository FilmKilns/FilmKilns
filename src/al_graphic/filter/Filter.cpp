//
// Created by limin on 2018/12/16.
//

#include "../include/Filter.h"
#include "log.h"
#include "../include/HwFrameBuffer.h"
#include "../include/HwAndroidFrameBuffer.h"

Filter::Filter() : Object() {
    name = __func__;
}

#ifdef ANDROID

Filter::Filter(bool requestHwMode) : Object(), requestHwMode(requestHwMode) {
    name = __func__;
}

#endif

Filter::~Filter() {
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }
    if (drawer) {
        delete drawer;
        drawer = nullptr;
    }
}

bool Filter::init(int w, int h) {
    if (initialized)
        return false;
#ifdef ANDROID
    if (requestHwMode) {
        fbo = new HwAndroidFrameBuffer(w, h);
    } else {
        fbo = new HwFrameBuffer(w, h);
    }
#else
    fbo = new HwFrameBuffer(w, h);
#endif
    initialized = true;
    return true;
}

void Filter::draw(GLuint texture) {
    if (!fbo || !drawer)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->getFrameBuffer());
    drawer->useProgram();
    bindResources();
    drawer->draw(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Filter::bindResources() {

}

HwAbsFrameBuffer *Filter::getFrameBuffer() {
    return fbo;
}

void Filter::setParams(int *params) {
    if (nullptr == params) return;
    int key = FILTER_NONE;
    for (int i = 0;; ++i) {
        if (0 == i % 2) {
            key = params[i];
            if (FILTER_NONE == params[i]) return;
        } else {
            setParam(key, params[i]);
        }
    }
}

void Filter::setParam(int key, int value) {

}