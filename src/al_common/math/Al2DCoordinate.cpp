/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "Al2DCoordinate.h"
#include "AlOrthMatrix.h"
#include "AlRectF.h"
#include "AlVec4.h"
#include "AlMath.h"

Al2DCoordinate::Al2DCoordinate(int32_t xWide, int32_t yWide)
        : Object(), wide(xWide, yWide), scale(1, 1), rotation(0, 1), position(0, 0) {

}

Al2DCoordinate::Al2DCoordinate(const Al2DCoordinate &o)
        : Object(), wide(o.wide), scale(o.scale), rotation(o.rotation), position(o.position) {

}

Al2DCoordinate::~Al2DCoordinate() {
}

void Al2DCoordinate::setWide(int32_t x, int32_t y) {
    wide.x = x;
    wide.y = y;
}

void Al2DCoordinate::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

void Al2DCoordinate::setRotation(AlRational r) {
    rotation.num = r.num;
    rotation.den = r.den;
}

void Al2DCoordinate::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Al2DCoordinate::translate(AlVec2 *vec, Al2DCoordinate *dstCoord) {
    if (nullptr == vec || nullptr == dstCoord) {
        return;
    }
    AlRectF sRect(-wide.x / 2,
                  wide.y / 2,
                  wide.x / 2,
                  -wide.y / 2);
    AlRectF dRect(-dstCoord->wide.x / 2,
                  dstCoord->wide.y / 2,
                  dstCoord->wide.x / 2,
                  -dstCoord->wide.y / 2);
    double alpha = static_cast<float>(dstCoord->rotation.toFloat() * AlMath::PI);


    AlVec4 tVec(vec->x * wide.x / 2, vec->y * wide.y / 2);
    AlMatrix sMat;
    AlMatrix rMat;
    AlMatrix tMat;
    sMat.setScale(1 / (0 != dstCoord->scale.x ? dstCoord->scale.x : 1),
                  1 / (0 != dstCoord->scale.y ? dstCoord->scale.y : 1));
    rMat.setRotation(alpha);
    tMat.setTranslate(-dstCoord->position.x * wide.x / 2, -dstCoord->position.y * wide.y / 2);
    /// 矩阵表示变换步骤时，刚好和实际顺序相反
    /// 缩放(sMat)->旋转(rMat)->位移(tMat) = vec * tMat * rMat * sMat
    tVec = tVec * tMat * rMat * sMat;

    AlOrthMatrix oMat;
    oMat.update(dRect.left, dRect.right, dRect.bottom, dRect.top, -1.0f, 1.0f);

    AlVec4 point = tVec * oMat;
    vec->x = point.x;
    vec->y = point.y;
}

AlSize Al2DCoordinate::getRegion() {
    return AlSize(wide.x, wide.y);
}
