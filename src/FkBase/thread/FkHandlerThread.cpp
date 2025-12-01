/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHandlerThread.h"
#include "FkThread.h"

#define TAG "FkHandlerThread"

FK_IMPL_CLASS_TYPE(FkHandlerThread, FkObject)

std::shared_ptr<FkHandlerThread> FkHandlerThread::create(std::string name) {
    return std::make_shared<FkHandlerThread>(name);
}

FkHandlerThread::FkHandlerThread(std::string name)
        : FkObject(),
          name(name) {
    mThread = std::thread(&FkHandlerThread::run, std::ref(*this));
    mThreadId = FkThread::currentThreadId(mThread.get_id());
    FkLooper::prepare(mThreadId);
    FkLogI(TAG, "Create thread(%s: %ld) looper.", name.c_str(), mThreadId);
}

void FkHandlerThread::run() {
    _init();
    auto id = FkThread::currentThreadId();
    FkLooper::prepare();
    FkLogI(TAG, "Run thread(%s: %ld) looper.", name.c_str(), id);
    FkLooper::loop();
    FkLooper::exit();
    FkLogI(TAG, "Exist thread(%s: %ld).", name.c_str(), id);
}

FkHandlerThread::~FkHandlerThread() {
    quit();
    mThread.join();
}

void FkHandlerThread::quit() {
    FkLooper *looper = getLooper();
    if (looper) {
        looper->quit(false);
    }
}

bool FkHandlerThread::quitSafely() {
    FkLooper *looper = getLooper();
    if (looper) {
        looper->quit(true);
        return true;
    }
    return false;
}

FkLooper *FkHandlerThread::getLooper() {
    return FkLooper::getLooper(mThreadId);
}