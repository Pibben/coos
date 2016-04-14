
#include <stdint.h>
#include <stdbool.h>


#include "interrupts.h"
#include "timer.h"
#include "system.h"

void enable_Basic_IRQs(uint32_t irqs) {
    REG(ENABLE_BASIC_IRQS) = irqs;
}

void disable_Basic_IRQs(uint32_t irqs) {
    REG(DISABLE_BASIC_IRQS) = irqs;
}

extern "C"
void interruptHandler()
{
    if(REG(IRQ_BASIC_PENDING) & BASIC_ARM_TIMER_IRQ) {
        System::instance().armTimer().handleTimerInterrupt();
    } else if(REG(IRQ_PENDING_1) & (1 << 1)) {
        System::instance().systemTimer1().handleTimerInterrupt();
    } else if(REG(IRQ_PENDING_1) & (1 << 3)) {
        System::instance().systemTimer3().handleTimerInterrupt();
    }
}
