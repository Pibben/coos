//
// Created by per on 2016-04-03.
//

#include "timer.h"
#include "reg.h"
#include "interrupts.h"

static timer_reg_t * timer = (timer_reg_t *)RPI_ARMTIMER_BASE;

extern "C"
void enableTimer() {
    enable_Basic_IRQs(RPI_BASIC_ARM_TIMER_IRQ);

    timer->Load = 0x400;

    timer->Control = RPI_ARMTIMER_CTRL_23BIT |
                     RPI_ARMTIMER_CTRL_ENABLE |
                     RPI_ARMTIMER_CTRL_INT_ENABLE |
                     RPI_ARMTIMER_CTRL_PRESCALE_256;

    /* Enable interrupts! */
    _enable_interrupts();
}

extern "C"
void disableTimer() {
    disable_Basic_IRQs(RPI_BASIC_ARM_TIMER_IRQ);
}

extern "C"
void clearIRQ(void) {
    timer->IRQClear = 1;
}
