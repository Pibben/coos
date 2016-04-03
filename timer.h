//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_TIMER_H_H
#define KERNEL_TIMER_H_H

//https://github.com/BrianSidebotham/arm-tutorial-rpi/blob/master/part-4/armc-013/rpi-armtimer.h

#include <stdint.h>

#include "regh"

/** @brief See the documentation for the ARM side timer (Section 14 of the
    BCM2835 Peripherals PDF) */
#define RPI_ARMTIMER_BASE               ( PERIPHERAL_BASE + 0xB400 )

/** @brief 0 : 16-bit counters - 1 : 23-bit counter */
#define RPI_ARMTIMER_CTRL_23BIT         ( 1 << 1 )

#define RPI_ARMTIMER_CTRL_PRESCALE_1    ( 0 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_16   ( 1 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_256  ( 2 << 2 )

/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
#define RPI_ARMTIMER_CTRL_INT_ENABLE    ( 1 << 5 )
#define RPI_ARMTIMER_CTRL_INT_DISABLE   ( 0 << 5 )

/** @brief 0 : Timer disabled - 1 : Timer enabled */
#define RPI_ARMTIMER_CTRL_ENABLE        ( 1 << 7 )
#define RPI_ARMTIMER_CTRL_DISABLE       ( 0 << 7 )


typedef struct {

    volatile uint32_t Load;
    volatile uint32_t Value;
    volatile uint32_t Control;
    volatile uint32_t IRQClear;
    volatile uint32_t RAWIRQ;
    volatile uint32_t MaskedIRQ;
    volatile uint32_t Reload;
    volatile uint32_t PreDivider;
    volatile uint32_t FreeRunningCounter;

} arm_timer_t;


#endif //KERNEL_TIMER_H_H
