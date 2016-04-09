//
// Created by per on 2016-04-08.
//

#ifndef KERNEL_EVENTLOOP_H
#define KERNEL_EVENTLOOP_H

#include <functional>
#include <stdio.h>
#include <deque>

class Eventloop {
private:
    std::deque<std::function<void(void)>> mQueue;
public:
    Eventloop() : mQueue() {}

    template <class TFunc>
    void post(TFunc&& func) {
        mQueue.push_back(std::move<TFunc>(func));
    }

    void deplete() {
        while(!mQueue.empty()) {
            auto f = mQueue.front();
            f();
            mQueue.pop_front();
        }
    }

    void runOne() {
        while(mQueue.empty()) {
            asm ("" ::: "memory");
        }
        auto f = mQueue.front();
        f();
        mQueue.pop_front();
    }
};

#endif //KERNEL_EVENTLOOP_H
