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

long FkThread::currentThreadId() {
    mach_port_t mach_thread = pthread_mach_thread_np(pthread_self());
    long thread_id_long = (long)mach_thread;
    return thread_id_long;
}