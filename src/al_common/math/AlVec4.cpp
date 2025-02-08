/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlVec4.h"
#include "Logcat.h"

#define TAG "AlVec4"

AlVec4::AlVec4(AlVec2 vec2, float z, float w) : Object(), x(vec2.x), y(vec2.y), z(z), w(w) {

}

AlVec4::AlVec4(float x, float y, float z, float w) : Object(), x(x), y(y), z(z), w(w) {

}

AlVec4::AlVec4(const AlVec4 &o) : Object(), x(o.x), y(o.y), z(o.z), w(o.w) {

}

AlVec4::~AlVec4() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

void AlVec4::set(int32_t i, float val) {
    switch (i) {
        case 0: {
            x = val;
            break;
        }
        case 1: {
            y = val;
            break;
        }
        case 2: {
            z = val;
            break;
        }
        case 3: {
            w = val;
            break;
        }
        default:
            break;
    }
}

AlVec4 AlVec4::operator*(AlMatrix &m) {
     AlVec4 vec;
    for (int i = 0; i < 4; i++) {
        vec.set(i, x * m.get(0, i) +
                    y * m.get(1, i) +
                    z * m.get(2, i) +
                    w * m.get(3, i));
    }
    return vec;
}

AlVec2 AlVec4::xy() {
    return AlVec2(x, y);
}

void AlVec4::dump() {
    Logcat::i(TAG, "[%f, %f, %f, %f]", x, y, z, w);
}
