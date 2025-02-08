/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/FrameBuffer.h"

FrameBuffer::FrameBuffer(int w, int h) {
    size = new Size(w, h);
    createTexture();
}

FrameBuffer::~FrameBuffer() {
    if (size) {
        delete size;
        size = nullptr;
    }
    if (GL_NONE != fbo) {
        glDeleteFramebuffers(1, &fbo);
    }
    if (GL_NONE != id) {
        glDeleteTextures(1, &id);
    }
}

void FrameBuffer::createTexture() {
    glGenTextures(1, &id);
    glGenFramebuffers(1, &fbo);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, size->width, size->height, 0,
                 fmt, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, id, 0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

int FrameBuffer::width() {
    if (size) {
        return size->width;
    }
    return 0;
}

int FrameBuffer::height() {
    if (size) {
        return size->height;
    }
    return 0;
}
