/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYERMODEL_H
#define HWVC_ANDROID_ALIMAGELAYERMODEL_H

#include <vector>
#include "Object.h"
#include "AlVec2.h"
#include "AlIdentityCreator.h"
#include "AlQuad.h"
#include "AlRational.h"
#include "HwResult.h"
#include "AlAbsElemented.h"

class AlImageLayer;

class AlAbsMAction;

class AlFileImporter;

al_class AlImageLayerModel al_extend AlAbsElemented {
public:
    static AlImageLayerModel *create(AlIdentityCreator *creator, const std::string path);

    static AlImageLayerModel *create(AlIdentityCreator *creator, AlImageLayerModel &model);

private:
    friend AlFileImporter;

    static AlImageLayerModel *create(int32_t id, const std::string path);

    AlImageLayerModel(int32_t id, const std::string &path);

public:

    AlImageLayerModel(const AlImageLayerModel &o);

    virtual ~AlImageLayerModel();

    std::string getPath();

    int32_t getId();

    void setAlpha(float alpha);

    float getAlpha();

    void setScale(float scaleX, float scaleY);

    AlVec2 &getScale();

    void setRotation(AlRational &r);

    AlRational &getRotation();

    double getRadian();

    void postPosition(float x, float y);

    void setPosition(float x, float y);

    AlVec2 &getPosition();

    void setQuad(AlVec2 &leftTop, AlVec2 &leftBottom, AlVec2 &rightBottom, AlVec2 &rightTop);

    void setQuad(const AlQuad &quad);

    AlQuad &getQuad();

    HwResult addAction(AlAbsMAction *opt);

    std::vector<AlAbsMAction *> *getAllActions();

    bool removeCropAction();

    bool removeAlignCropAction();

    int32_t countFilterAction();

    void dump();

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

private:
    bool _removeAction(const type_info &info);

private:
    friend AlImageLayer;
    std::string path;
    int32_t id = AlIdentityCreator::NONE_ID;
    float alpha = 1.0f;
    ///弧度表示，顺时针方向为正向(CW)，r=rotation*PI
    AlRational rotation = AlRational();
    AlVec2 position;
    AlVec2 scale;
    AlQuad quad;
    std::vector<AlAbsMAction *> actions;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMODEL_H
