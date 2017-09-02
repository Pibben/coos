//
// Created by per on 2016-04-10.
//

#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include <stdint.h>
#include "interrupts.h"

class InterruptLock {
private:
    volatile uint32_t mFlags;
public:

    InterruptLock() : mFlags(0U) { }

    void lock() {
        __asm volatile("mrs %0, cpsr" : "=r" (mFlags));
        disable_interrupts();
    }

    void unlock() {
        if ((mFlags & CPSR_IRQ) == 0) {
            enable_interrupts();
        }
    }

    void lockIntr() {

    }

    void unlockIntr() {

    }
};

class SpinLock {
private:
    volatile uint32_t mLockValue;
public:
    SpinLock() : mLockValue(0) {}

    void lock() {
        while (__sync_lock_test_and_set(&mLockValue, 1) == 1) { }
    }

    void unlock() {
        __sync_lock_release(&mLockValue);
    }
};

class WaitCond { //TODO: Rename
public:
    void wait(InterruptLock& lock) {
        lock.unlock();
        __asm volatile("wfi");
        lock.lock();
    }
};



#endif //CONCURRENCY_H
