//
// Created by per on 2016-04-03.
//

#include <stdio.h>

#include "timer.h"
#include "reg.h"
#include "interrupts.h"
#include "system.h"

std::function<void(void)> gTimerCallback;

void enableTimer(uint32_t value) {
    enable_Basic_IRQs(BASIC_ARM_TIMER_IRQ);

    REG(ARMTIMER_LOAD) = value;

    REG(ARMTIMER_CTRL) = ARMTIMER_CTRL_23BIT |
                         ARMTIMER_CTRL_ENABLE |
                         ARMTIMER_CTRL_INT_ENABLE |
                         ARMTIMER_CTRL_PRESCALE_256;

    _enable_interrupts();
}

void disableTimer() {
    disable_Basic_IRQs(BASIC_ARM_TIMER_IRQ);
}

static void clearIRQ(void) {
    REG(ARMTIMER_IRQ_CLEAR) = 1;
}

void handleTimerInterrupt() {
    clearIRQ();
    System::instance().eventloop().postIntr(gTimerCallback);
    disableTimer();
}
