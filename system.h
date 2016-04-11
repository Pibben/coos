//
// Created by per on 2016-04-08.
//

#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#include "eventloop.h"
#include "uart.h"

class System {
private:
    Eventloop mEventloop;
    Uart mUart;
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
};

#endif //KERNEL_SYSTEM_H
