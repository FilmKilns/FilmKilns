/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKVEC2_H
#define FK_BASE_FKVEC2_H

#include "FkObject.h"

/**
 * position or scale params
 */
/// [x,y]
FK_SUPER_TEMPLATE_CLASS(FkVec2, FkObject, typename T) {
FK_DEF_CLASS_TYPE_FUNC(FkVec2)

public:
    static void swap(FkVec2<T> *vec1, FkVec2<T> *vec2) {
        float tmp = vec1->x;
        vec1->x = vec2->x;
        vec2->x = tmp;

        tmp = vec1->y;
        vec1->y = vec2->y;
        vec2->y = tmp;
    };

public:
    FkVec2(T x, T y) : FkObject(), x(x), y(y) {

    };

    FkVec2(const FkVec2<T> &o) : FkObject(), x(o.x), y(o.y) {

    };

    virtual ~FkVec2() {};

    virtual void set(FkVec2<T> vec) {
        x = vec.x;
        y = vec.y;
    }

    virtual FkVec2<T> operator+(const T value) {
        return FkVec2<T>(x + value, y + value);
    };

    virtual FkVec2<T> operator+(const FkVec2<T> &vec) {
        return FkVec2<T>(x + vec.x, y + vec.y);
    };

    virtual FkVec2<T> operator-(const FkVec2<T> &vec) {
        return FkVec2<T>(x - vec.x, y - vec.y);
    };

    virtual FkVec2<T> operator/(const float value) {
        return FkVec2<T>(x / value, y / value);
    }

    virtual FkVec2<T> operator/(const int32_t value) {
        return FkVec2<T>(x / value, y / value);
    }

    virtual bool operator<(const FkVec2<T> &vec) {
        return x < vec.x && y < vec.y;
    };

    virtual bool operator>(const FkVec2<T> &vec) {
        return x > vec.x && y > vec.y;
    };

    virtual bool operator==(const FkVec2<T> &vec) {
        return x == vec.x && y == vec.y;
    };

    virtual FkVec2<T> &operator+=(const FkVec2<T> &vec) {
        x += vec.x;
        y += vec.y;
        return *this;
    };

    virtual bool isZero() {
        return 0 == x && 0 == y;
    };

public:
    T x;
    T y;
};


#endif //FK_BASE_FKVEC2_H
