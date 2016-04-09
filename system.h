//
// Created by per on 2016-04-08.
//

#ifndef KERNEL_SYSTEM_H
#define KERNEL_SYSTEM_H

#include "eventloop.h"

class System {
private:
    Eventloop mEventloop;
public:
    static System& instance() {
        static System system;
        return system;
    }

    Eventloop& eventloop() {
        return mEventloop;
    }
};

#endif //KERNEL_SYSTEM_H
