/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "HwvcFilter.h"
#include "NormalDrawer.h"
#include "entity/FilterEntity.h"
#include "AlBitmapFactory.h"
#include "HwProgram.h"
#include "log.h"

HwvcFilter::HwvcFilter(char *path) : HwAbsFilter(), path(path) {
}

HwvcFilter::~HwvcFilter() {
    if (params) {
        delete[]params;
        params = nullptr;
    }
    if (paramLocations) {
        delete[]paramLocations;
        paramLocations = nullptr;
    }
    if (textures) {
        glDeleteTextures(size, textures);
        delete[]textures;
        textures = nullptr;
    }
    if (textureLocations) {
        delete[]textureLocations;
        textureLocations = nullptr;
    }
}

AlAbsGLProgram *HwvcFilter::createProgram() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    FilterEntity *entity = FilterEntity::fromFile(path);
    gettimeofday(&end, NULL);
    long time = end.tv_usec - start.tv_usec;
    HwProgram *program = HwProgram::create(&entity->vertex, &entity->fragment);
    //读取Sampler
    this->size = entity->samplers.size();
    if (0 != this->size) {
        textures = new GLuint[size];
        textureLocations = new GLint[size];
        int i = 0;
        for (auto itr = entity->samplers.begin(); itr != entity->samplers.end(); itr++) {
            textureLocations[i] = program->getUniformLocation(itr->first);
            textures[i] = loadTexture(itr->second);
            ++i;
        }
    }
    //读取Params
    paramSize = entity->params.size();
    if (0 != paramSize) {
        paramLocations = new GLint[paramSize];
        params = new float[paramSize];
        int i = 0;
        for (auto itr = entity->params.begin(); itr != entity->params.end(); itr++) {
            paramLocations[i] = program->getUniformLocation(itr->first);
            params[i] = itr->second;
            ++i;
        }
    }
    gettimeofday(&end, NULL);
    LOGI("%s(ver: %d) read cost: %ld / %ld us", entity->name.c_str(), entity->version, time,
         (end.tv_usec - start.tv_usec));
    delete entity;
    return program;
}

void HwvcFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    /**
     * GL_TEXTURE0为保留Sampler，给默认画面使用
     */
    for (int i = 0; i < size; ++i) {
        int offset = i + 1;
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glUniform1i(textureLocations[i], offset);
    }
    for (int i = 0; i < paramSize; ++i) {
        program->setUniform1f(paramLocations[i], params[i]);
    }
    program->unbind();
}

GLuint HwvcFilter::loadTexture(string pngBuf) {
    AlBitmap *bmp = AlBitmapFactory::decodeBuffer(
            AlBuffer::wrap((uint8_t *) pngBuf.data(), pngBuf.size()));
    if (nullptr == bmp) {
        LOGE("Read image failed");
        return GL_NONE;
    }
    GLuint texture = GL_NONE;
    glGenTextures(1, &texture);
    if (GL_NONE == texture)
        return GL_NONE;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->getWidth(), bmp->getHeight(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, bmp->getPixels());
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    delete bmp;
    return texture;
}