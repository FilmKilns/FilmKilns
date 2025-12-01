/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKDEFINITION_H
#define FK_BASE_FKDEFINITION_H

#include "FkLogcat.h"
#include "FkClassType.h"
#include <functional>
#include <inttypes.h>
#include <stdlib.h>

// +-------- Define --------+
#define FK_INTERFACE class
#define FK_ENUM enum class
#define FK_EXTEND : public
#define FK_EXT_AND , public
#define FK_NULLABLE
#define Fk_POINTER_CAST(CLASS, ptr) std::static_pointer_cast<CLASS>(ptr)

typedef std::function<void(int)> FkResultCallback;

// +-------- Log --------+
#define FK_DEF_TAG "FilmKilns"
#define FkLogV(tag, fmt, args...) FkLogcat::v(tag, fmt, ##args)
#define FkLogD(tag, fmt, args...) FkLogcat::d(tag, fmt, ##args)
#define FkLogI(tag, fmt, args...) FkLogcat::i(tag, fmt, ##args)
#define FkLogE(tag, fmt, args...) FkLogcat::e(tag, fmt, ##args)
#define FkLogW(tag, fmt, args...) FkLogcat::w(tag, fmt, ##args)

// +-------- Assert --------+
#if defined(__FK_DEBUG__)
#include <assert.h>
#define FkAssertMsg(condition, value, msg) \
if (!(condition)) FkLogE("FkAssert", "assert(%s) failed: %s", #condition, msg); \
assert(condition)
#define FkAssert(condition, value) assert(condition)
#else
#define FkAssertMsg(condition, value, msg)            \
    if(!(condition)) {                        \
        FkLogE("FkAssert", "assert(%s) failed: %s", #condition, msg); \
        return value;                         \
    }
#define FkAssert(condition, value) FkAssertMsg(condition, value, "")
#endif

// +------ State Code ------+
typedef int32_t FkResult;
#define FK_DONE 1
#define FK_OK 0
#define FK_FAIL -1
#define FK_INVALID_STATE -2
#define FK_PROTOCOL_NOT_ACCEPT -3
#define FK_INVALID_DATA -4
#define FK_EMPTY_DATA -5
#define FK_NPE -6
#define FK_PORT_NOT_FOUND -7
#define FK_SOURCE_NOT_FOUND -8
#define FK_NOT_SUPPORT -9
#define FK_INVALID_PARAMETERS -10
//Image
#define FK_EMPTY_CANVAS -100
//File
#define FK_FILE_NOT_FOUND -200
#define FK_FILE_OPEN_FAIL -201
//IO
#define FK_IO_FAIL -301

#define FK_SKIP -400
#define FK_DO_NOTHING -401

// +------ ID ------+
typedef int32_t FkID;
#define FK_ID_NONE -1
#define Fk_CANVAS_ID 0

// +------ KID ------+
#define FK_KID(a, b, c, d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

FK_ENUM kState : uint32_t {
    IDL = 1,
    CREATING = 1 << 1,
    CREATED = 1 << 2,
    STARTING = 1 << 3,
    RUNNING = 1 << 4,
    STOPPING = 1 << 5,
    STOPPED = 1 << 6,
    DESTROYING = 1 << 7,
};

FK_ENUM kScaleType : int {
    DEFAULT = 0,
    CENTER_MATRIX,
    CENTER_INSIDE,
    CENTER_CROP,
};

FK_ENUM kVisibility : int {
    VISIBLE = 0,
    GONE,
    INVISIBLE,
};


#define Fk_CHECK_NULL_WITH_STATEMENT(value, statement) do { if (value == nullptr) { FkLogE(FK_DEF_TAG, "%s is null.", #value); statement; }} while(false)
#define Fk_CHECK_NULL(value) Fk_CHECK_NULL_WITH_STATEMENT(value, return FK_NPE)

#endif //FK_BASE_FKDEFINITION_H
