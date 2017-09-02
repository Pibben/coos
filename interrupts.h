//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_INTERRUPTS_H
#define KERNEL_INTERRUPTS_H

//https://raw.githubusercontent.com/BrianSidebotham/arm-tutorial-rpi/master/part-4/armc-013/rpi-interrupts.h
#include <stdint.h>

#include "reg.h"


void enable_Basic_IRQs(uint32_t irqs);
void disable_Basic_IRQs(uint32_t irqs);
void enable_interrupts();
void disable_interrupts();


#endif //KERNEL_INTERRUPTS_H
