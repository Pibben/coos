//
// Created by per on 2016-04-08.
//

#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#include "eventloop.h"
#include "uart.h"
#include "timer.h"

class System {
private:
    Eventloop mEventloop;
    Uart mUart;
    ArmTimer mArmTimer;
    SystemTimer mSystemTimer0;
    SystemTimer mSystemTimer2;
    System() : mSystemTimer0(0), mSystemTimer2(2) {}
public:
    static System& instance() {
        static System system;
        return system;
    }

    Eventloop& eventloop() {
        return mEventloop;
    }

    Uart& uart() {
        return mUart;
    }

    ArmTimer& armTimer() {
        return mArmTimer;
    }

    SystemTimer& systemTimer0() {
        return mSystemTimer0;
    }

    SystemTimer& systemTimer2() {
        return mSystemTimer2;
    }
};

#endif //KERNEL_SYSTEM_H
