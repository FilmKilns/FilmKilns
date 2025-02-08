/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSFILTER_H
#define HWVC_ANDROID_HWABSFILTER_H

#include "Object.h"
#include "AlAbsGLProgram.h"
#include "HwAbsTexture.h"
#include "HwAbsFBObject.h"

const int Hw_FILTER_BASE = 100;
const int Hw_FILTER_NONE = 2333;

class HwAbsFilter : public Object {
public:
    string name;

    HwAbsFilter();

    virtual ~HwAbsFilter();

    virtual bool prepare();

    /// 绘制纹理，注意要先调用glViewPort
    /// \param src
    /// \param dest
    virtual void draw(HwAbsTexture *src, HwAbsTexture *dest);

    virtual void setParams(int *params);

    virtual void setParam(int key, int value);

    virtual AlAbsGLProgram *createProgram() = 0;

    virtual void drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest);

    virtual void drawStart(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest);

    virtual void drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest);

private:
    HwAbsFBObject *fbo = nullptr;
    AlAbsGLProgram *program = nullptr;
    bool initialized = false;
};


#endif //HWVC_ANDROID_HWABSFILTER_H
