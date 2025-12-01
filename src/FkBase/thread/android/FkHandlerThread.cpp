/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkHandlerThread.h"
#include <sys/prctl.h>

void FkHandlerThread::_init() {
    pthread_setname_np(mThread.native_handle(), name.c_str());
}