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
//    return syscall(SYS_gettid);
    return pthread_self();
}