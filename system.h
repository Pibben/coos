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
    SystemTimer mSystemTimer1;
    SystemTimer mSystemTimer3;
    System() : mSystemTimer1(1), mSystemTimer3(3) {}
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

    SystemTimer& systemTimer1() {
        return mSystemTimer1;
    }

    SystemTimer& systemTimer3() {
        return mSystemTimer3;
    }
};

#endif //KERNEL_SYSTEM_H
