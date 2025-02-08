//
// Created by limin on 2019/5/7.
//

#include <jni.h>
#include <log.h>
#include "HwFIFOBuffer.h"
#include "Logcat.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MemFIFOTest_FRAME_SIZE 8
HwFIFOBuffer *fifo = nullptr;
uint8_t *dBuf = nullptr;
int index = 'a';
int pushCount = 0;
int takeCount = 0;

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_test_MemFIFOTest_push
        (JNIEnv *env, jobject thiz) {
    if (!fifo) {
        fifo = new HwFIFOBuffer(8 * MemFIFOTest_FRAME_SIZE);
        dBuf = new uint8_t[MemFIFOTest_FRAME_SIZE];
    }
    if (index > 'z') {
        index = 'a';
    }
    memset(dBuf, index, MemFIFOTest_FRAME_SIZE);
    ++pushCount;
    Logcat::i("HWVC", "MemFIFOTest_push: %c,%c,%c,%c,%c,%c,%c,%c, %d",
              (char) dBuf[0], (char) dBuf[1],
              (char) dBuf[2], (char) dBuf[3],
              (char) dBuf[4], (char) dBuf[5],
              (char) dBuf[6], (char) dBuf[7],
              pushCount);
    fifo->push(dBuf, MemFIFOTest_FRAME_SIZE, 6000);
    ++index;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_test_MemFIFOTest_take
        (JNIEnv *env, jobject thiz) {
    if (fifo) {
        HwBuffer *buf = fifo->take(MemFIFOTest_FRAME_SIZE);
        if (buf) {
            ++takeCount;
            Logcat::i("HWVC", "MemFIFOTest_take: %c,%c,%c,%c,%c,%c,%c,%c, %lld, %d",
                      (char) buf->data()[0], (char) buf->data()[1],
                      (char) buf->data()[2], (char) buf->data()[3],
                      (char) buf->data()[4], (char) buf->data()[5],
                      (char) buf->data()[6], (char) buf->data()[7],
                      buf->size(), takeCount);
        }
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_test_MemFIFOTest_release
        (JNIEnv *env, jobject thiz) {
    if (fifo) {
        delete fifo;
        fifo = nullptr;
    }
    if (dBuf) {
        delete[] dBuf;
        dBuf = nullptr;
    }
}

#ifdef __cplusplus
}
#endif