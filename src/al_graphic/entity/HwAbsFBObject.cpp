/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsFBObject.h"
#include "../include/HwAbsTexture.h"
#include <GLES2/gl2.h>

HwAbsFBObject::HwAbsFBObject() : Object() {
    glGenFramebuffers(1, &fbo);
}

HwAbsFBObject::~HwAbsFBObject() {
    this->tex = nullptr;
    glDeleteFramebuffers(1, &fbo);
}

void HwAbsFBObject::bindTex(HwAbsTexture *tex) {
    if (this->tex == tex) {
        return;
    }
    this->tex = tex;
    if (this->tex) {
        bind();
        this->tex->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               tex->target(), tex->texId(), 0);
        this->tex->unbind();
        unbind();
    }
}

void HwAbsFBObject::unbindTex() {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           tex->target(), GL_NONE, 0);
    unbind();
    this->tex = nullptr;
}

void HwAbsFBObject::bind() {
//    GLint curFbo;
//    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void HwAbsFBObject::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

bool HwAbsFBObject::read(uint8_t *pixels) {
    if (!tex) {
        return false;
    }
    bind();
    bool ret = tex->read(pixels);
    unbind();
    return ret;
}

bool HwAbsFBObject::read(AlBuffer *pixels) {
    if (nullptr == pixels) {
        return false;
    }
    return read(pixels->data());
}
