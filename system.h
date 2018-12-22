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
    static Eventloop mEventloop;
    static Uart mUart;
    static ArmTimer mArmTimer;
    static SystemTimer mSystemTimer1;
    static SystemTimer mSystemTimer3;
    static LocalTimer mLocalTimer;
public:
    static Eventloop& eventloop() {
        return mEventloop;
    }

    static Uart& uart() {
        return mUart;
    }

    static ArmTimer& armTimer() {
        return mArmTimer;
    }

    static SystemTimer& systemTimer1() {
        return mSystemTimer1;
    }

    static SystemTimer& systemTimer3() {
        return mSystemTimer3;
    }

    static LocalTimer& localTimer() {
        return mLocalTimer;
    }

    static constexpr uint32_t getPeripheralBase() {
        return 0x3f000000;
    }
    static constexpr uint32_t getLocalPeripheralBase() {
        return 0x40000000;
    }
};



#endif //KERNEL_SYSTEM_H
