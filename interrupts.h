//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_INTERRUPTS_H
#define KERNEL_INTERRUPTS_H

//https://raw.githubusercontent.com/BrianSidebotham/arm-tutorial-rpi/master/part-4/armc-013/rpi-interrupts.h
#include <stdint.h>

#include "reg.h"


void enableArmTimerInterrupt();
void disableArmTimerInterrupt();
void enableSystemTimerInterrupt(uint_fast8_t timerIdx);
void disableSystemTimerInterrupt(uint_fast8_t timerIdx);
void enable_interrupts();
void disable_interrupts();


#endif //KERNEL_INTERRUPTS_H
