/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/MessageQueue.h"
#include <sstream>

MessageQueue::MessageQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

MessageQueue::~MessageQueue() {
    LOGI("MessageQueue::~MessageQueue");
    pthread_mutex_lock(&mutex);
    invalid = true;
    pthread_mutex_unlock(&mutex);
    clear();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void MessageQueue::offer(Message *msg) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    queue.push_back(msg);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::offerAtFront(Message *msg) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }

    queue.push_front(msg);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


Message *MessageQueue::take() {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return nullptr;
    }
    if (size() <= 0) {
        if (0 != pthread_cond_wait(&cond, &mutex)) {
            pthread_mutex_unlock(&mutex);
            return nullptr;
        }
    }
    if (queue.empty()) {
        pthread_mutex_unlock(&mutex);
        return nullptr;
    }
    /**
     * Here maybe crash if queue is empty.
     */
    Message *e = queue.front();
    queue.pop_front();

    pthread_mutex_unlock(&mutex);
    return e;
}

int MessageQueue::size() {
    return queue.size();
};

void MessageQueue::pop() {
//    queue->pop();
}

void MessageQueue::notify() {
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::clear() {
    notify();
    if (queue.empty()) {
        return;
    }
    pthread_mutex_lock(&mutex);
    while (!queue.empty()) {
        auto *e = queue.front();
        queue.pop_front();
        if (e) {
            delete e;
        }
    }
    queue.clear();
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::removeAllMessage(const int what) {
    pthread_mutex_lock(&mutex);
    if (invalid) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    list<Message *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        Message *e = *itr;
        if (what == e->what) {
            queue.remove(e);
            delete e;
        }
        ++itr;
    }
    pthread_mutex_unlock(&mutex);
}

void MessageQueue::printQueue() {
#if 1
    std::ostringstream os;
    list<Message *>::iterator itr = queue.begin();
    while (itr != queue.end()) {
        Message *e = *itr;
        os << e->desc << "(" << e << "),";
        ++itr;
    }
    Logcat::i("HWVC", "MessageQueue(%s)", os.str().c_str());
#endif
}