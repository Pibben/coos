//
// Created by per on 2016-04-27.
//

#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <stdint.h>

enum class State {
    FREE,
    RUNNING,
    READY,
    SUSPENDED
};

class Process {
public:
    Process() : mPID(0), mPrio(0), mNext(0), mPrev(0), mStack(0), mState(State::FREE), mQuantum(0) {}
    uint8_t mPID;
    uint8_t mPrio;
    uint8_t mNext;
    uint8_t mPrev;
    uintptr_t mStack;
    State mState;
    uint16_t mQuantum;
};

namespace Scheduler {
    void schedule();
};

#endif //KERNEL_PROCESS_H
