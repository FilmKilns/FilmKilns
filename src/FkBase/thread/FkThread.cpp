/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
* CREATE TIME: 2022-3-26 22:24:30
*/

#include "FkThread.h"
#include "FkDefinition.h"
#include <sstream>

FK_IMPL_CLASS_TYPE(FkThread, FkObject)

bool FkThread::sleep(int64_t us) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(us * 1000));
    return true;
}

long FkThread::currentThreadId(std::__thread_id id) {
    std::ostringstream oss;
    oss << id;
    return std::stol(oss.str());
}

static void *run(void *arg) {
    FkThread *thread = static_cast<FkThread *>(arg);
    FkLogI(FK_DEF_TAG, "FkThread(%ld) start", FkThread::currentThreadId());
    thread->runnable();
    FkLogI(FK_DEF_TAG, "FkThread(%ld:%s) stop", FkThread::currentThreadId(), thread->name.c_str());
    return nullptr;
}

FkThread::FkThread(std::string name, std::function<void()> runnable) {
    this->inter = false;
    this->name = name;
    this->runnable = runnable;
    pthread_mutex_init(&mutex, nullptr);
}

FkThread::~FkThread() {
    FkLogI(FK_DEF_TAG, "~FkThread");
    this->runnable = nullptr;
    stop();
}

void FkThread::start() {
    this->inter = false;
    createThread();
}

void FkThread::createThread() {
    pthread_attr_init(&attr);
    //将线程的属性称为detached，则线程退出时会自己清理资源
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&thread, &attr, run, (void *) this);
    if (0 != ret) {
        pthread_attr_destroy(&attr);
    }
    FkAssertMsg(0 == ret, , "FkThread create failed: %d");
}

void FkThread::stop() {
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
}

bool FkThread::isRunning() {
    return !interrupted();
}

void FkThread::interrupt() {
    lock();
    if (inter) {
        return;
    }
    inter = true;
    unLock();
    if (pthread_join(thread, 0)) {
        FkLogI(FK_DEF_TAG, "FkThread(%ld) join failed", thread);
    }
}

bool FkThread::interrupted() {
    lock();
    bool ret = inter;
    unLock();
    return ret;
}

void FkThread::lock() {
    pthread_mutex_lock(&mutex);
}

void FkThread::unLock() {
    pthread_mutex_unlock(&mutex);
}