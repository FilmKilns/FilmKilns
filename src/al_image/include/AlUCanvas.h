/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUCANVAS_H
#define HWVC_ANDROID_ALUCANVAS_H

#include "Unit.h"
#include "AlImageCanvas.h"
#include "AlImageCanvasModel.h"

al_def_unit(AlUCanvas, Unit) {
public:
    AlUCanvas(const string &alias);

    virtual ~AlUCanvas();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

private:
    /// 更新画布大小
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | AlSize       | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onResize(AlMessage *m);

    /// 清空画布
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onClear(AlMessage *m);

    /// 绘制图层到画布上
    /// FORMAT:
    /// +------------------------------------------------+
    /// | msg::obj              | msg::arg1 | msg::arg2 |
    /// +------------------------------------------------+
    /// | AlImageLayerDrawModel | none      | none      |
    /// +------------------------------------------------+
    /// \param msg
    /// \return
    bool onDraw(AlMessage *m);

    /// 上屏
    /// FORMAT:
    /// +----------------------------------+
    /// | msg::obj | msg::arg1 | msg::arg2 |
    /// +----------------------------------+
    /// | none     | none      | none      |
    /// +----------------------------------+
    /// \param msg
    /// \return
    bool onShow(AlMessage *m);

    /// 保存图片到文件
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | req code  | none      | out path  |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onSave(AlMessage *m);

    /// 设置画布背景样式
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | kBGType  | none      | out path  |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onSetBackground(AlMessage *m);

    bool onEncodeFinish(AlMessage *msg);

private:
    void _newDefaultCanvas(AlSize size);

    void _draw(AlImageLayerDrawModel *description);

    void _update(int32_t width, int32_t height, int32_t color);

    void _notifyDrawDone();

private:
    AlImageCanvas mCanvas;
    int32_t mDrawCount = 0;
    /// Request save to file(as jpeg...) next draw.
    std::string mOutputPath;
};


#endif //HWVC_ANDROID_ALUCANVAS_H
