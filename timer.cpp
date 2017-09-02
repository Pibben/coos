//
// Created by per on 2016-04-03.
//

#include <stdio.h>

#include "timer.h"
#include "reg.h"
#include "interrupts.h"
#include "system.h"

void ArmTimer::enableTimer(uint32_t value) {
    enable_Basic_IRQs(BASIC_ARM_TIMER_IRQ);

    REG(ARMTIMER_LOAD) = value;

    REG(ARMTIMER_CTRL) = ARMTIMER_CTRL_23BIT |
                         ARMTIMER_CTRL_ENABLE |
                         ARMTIMER_CTRL_INT_ENABLE |
                         ARMTIMER_CTRL_PRESCALE_256;

    enable_interrupts();
}

void ArmTimer::disableTimer() {
    disable_Basic_IRQs(BASIC_ARM_TIMER_IRQ);
}

static void clearIRQ(void) {
    REG(ARMTIMER_IRQ_CLEAR) = 1;
}

void ArmTimer::handleTimerInterrupt() {
    clearIRQ();
    System::instance().eventloop().postIntr(gTimerCallback);
    disableTimer();
}


void SystemTimer::enableTimer(uint32_t value) {
    REG(ENABLE_IRQS_1) = (1 << mTimerIdx);
    REG(SYSTEM_TIMER_CS) |= (1 << mTimerIdx);

    REG(SYSTEM_TIMER_C0+mTimerIdx*4) = REG(SYSTEM_TIMER_CLO) + value;

    enable_interrupts();
}

void SystemTimer::disableTimer() {
    REG(DISABLE_IRQS_1) = (1 << mTimerIdx);
}

void SystemTimer::handleTimerInterrupt() {
    REG(SYSTEM_TIMER_CS) |= (1 << mTimerIdx);
    System::instance().eventloop().postIntr(gTimerCallback);
    disableTimer();
}

uint32_t SystemTimer::getValue() {
    return REG(SYSTEM_TIMER_CLO);
}