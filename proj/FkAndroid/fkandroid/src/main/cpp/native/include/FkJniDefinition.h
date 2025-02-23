//
// Created by mingyi.li on 4/8/22.
//

#ifndef FKANDROID_FKJNIDEFINITION_H
#define FKANDROID_FKJNIDEFINITION_H

#include "FkJniStringUTF8.h"
#include "FkBuffer.h"
#include "FkJValues.h"

// #define FK_CLASS
#define FK_JNI_METHOD_DEFINE(PKT, CLASS, METHOD) JNICALL Java_ ## PKT ## _ ## CLASS ## _ ## METHOD

typedef struct _Array {
    uint8_t *data = nullptr;
    int32_t len = 0;
} Array;

#endif //FKANDROID_FKJNIDEFINITION_H
