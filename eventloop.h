//
// Created by per on 2016-04-08.
//

#ifndef KERNEL_EVENTLOOP_H
#define KERNEL_EVENTLOOP_H

#include <functional>
#include <stdio.h>
#include <deque>
#include <mutex>
#include "interrupts.h"
#include "concurrency.h"

class Eventloop {
private:
    std::deque<std::function<void(void)>> mQueue;
    InterruptLock mLock;
    WaitCond mCond;
public:
    Eventloop() : mQueue() {}

    template <class TFunc>
    void post(TFunc&& func) {
        mLock.lock();
        mQueue.push_back(std::forward<TFunc>(func));
        mLock.unlock();
    }

    template <class TFunc>
    void postIntr(TFunc&& func) {
        mLock.lockIntr();
        mQueue.push_back(std::forward<TFunc>(func));
        mLock.unlockIntr();
    }

    void deplete() {
        mLock.lock();
        while(!mQueue.empty()) {
            auto f = mQueue.front();
            mLock.unlock();
            f();
            mLock.lock();
            mQueue.pop_front();
        }
        mLock.unlock();
    }

    void runOne() {
        mLock.lock();
        while(mQueue.empty()) {
            mCond.wait(mLock);
        }
        auto f = mQueue.front();
        mLock.unlock();
        f();
        mLock.lock();
        mQueue.pop_front();
        mLock.unlock();
    }
};

#endif //KERNEL_EVENTLOOP_H
