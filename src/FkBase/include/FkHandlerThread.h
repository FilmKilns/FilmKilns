/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKHANDLERTHREAD_H
#define FK_BASE_FKHANDLERTHREAD_H

#include "FkObject.h"
#include "FkHandler.h"
#include "FkLooper.h"
#include <thread>
#include <mutex>
#include <condition_variable>

FK_SUPER_CLASS(FkHandlerThread, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkHandlerThread)

public:
    static std::shared_ptr<FkHandlerThread> create(std::string name);

public:
    FkHandlerThread(std::string name);

    FkHandlerThread(const FkHandlerThread &o) = delete;

    ~FkHandlerThread();

    void quit();

    bool quitSafely();

    FkLooper *getLooper();

private:
    void run();

    void _init();

private:
    std::string name;
    std::thread mThread;
    long mThreadId = 0;

};


#endif //FK_BASE_FKHANDLERTHREAD_H
