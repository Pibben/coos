
#include <stdint.h>
#include <stdbool.h>


#include "interrupts.h"
#include "timer.h"

void enable_Basic_IRQs(uint32_t irqs) {
    REG(ENABLE_BASIC_IRQS) = irqs;
}

void disable_Basic_IRQs(uint32_t irqs) {
    REG(DISABLE_BASIC_IRQS) = irqs;
}

extern "C"
void interruptHandler()
{
    handleTimerInterrupt();
}
