//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_INTERRUPTS_H
#define KERNEL_INTERRUPTS_H

//https://raw.githubusercontent.com/BrianSidebotham/arm-tutorial-rpi/master/part-4/armc-013/rpi-interrupts.h
#include <stdint.h>

namespace cpu {
    namespace interrupt {
        void enableArmTimer();

        void disableArmTimer();

        void enableSystemTimer(uint_fast8_t timerIdx);

        void disableSystemTimer(uint_fast8_t timerIdx);

        void enable();

        void disable();
    }
}


#endif //KERNEL_INTERRUPTS_H
