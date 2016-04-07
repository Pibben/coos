#include <stdint.h>

#ifndef REG_H
#define REG_H

enum
{
    // The GPIO registers base address.
    //GPIO_BASE = 0x20200000,
    PERIPHERAL_BASE = 0x3f000000,
    GPIO_BASE = 0x3f200000,

    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),
    
    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    INTERRUPT_CONTROLLER_BASE = (PERIPHERAL_BASE + 0xB200),

    IRQ_BASIC_PENDING = (INTERRUPT_CONTROLLER_BASE + 0x00),
    IRQ_PENDING_1 = (INTERRUPT_CONTROLLER_BASE + 0x04),
    IRQ_PENDING_2 = (INTERRUPT_CONTROLLER_BASE + 0x08),
    FIQ_CTRL = (INTERRUPT_CONTROLLER_BASE + 0x0C),
    ENABLE_IRQS_1 = (INTERRUPT_CONTROLLER_BASE + 0x10),
    ENABLE_IRQS_2 = (INTERRUPT_CONTROLLER_BASE + 0x14),
    ENABLE_BASIC_IRQS = (INTERRUPT_CONTROLLER_BASE + 0x18),
    DISABLE_IRQS_1 = (INTERRUPT_CONTROLLER_BASE + 0x1C),
    DISABLE_IRQS_2 = (INTERRUPT_CONTROLLER_BASE + 0x20),
    DISABLE_BASIC_IRQS = (INTERRUPT_CONTROLLER_BASE + 0x24),

    ARMTIMER_BASE = (PERIPHERAL_BASE + 0xB400),

    ARMTIMER_LOAD = (ARMTIMER_BASE + 0x00),
    ARMTIMER_VALUE = (ARMTIMER_BASE + 0x04),
    ARMTIMER_CTRL = (ARMTIMER_BASE + 0x08),
    ARMTIMER_IRQ_CLEAR = (ARMTIMER_BASE + 0x0C),
    ARMTIMER_IRQ_RAW = (ARMTIMER_BASE + 0x10),
    ARMTIMER_MASKED_IRQ = (ARMTIMER_BASE + 0x14),
    ARMTIMER_RELOAD = (ARMTIMER_BASE + 0x18),
    ARMTIMER_PREDIVIDER = (ARMTIMER_BASE + 0x1C),
    ARMTIMER_COUNTER = (ARMTIMER_BASE + 0x20),


    UART0_BASE = GPIO_BASE + 0x1000,
    
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}
//BASIC IRQs
enum {
    BASIC_ARM_TIMER_IRQ       = (1 << 0),
    BASIC_ARM_MAILBOX_IRQ     = (1 << 1),
    BASIC_ARM_DOORBELL_0_IRQ  = (1 << 2),
    BASIC_ARM_DOORBELL_1_IRQ  = (1 << 3),
    BASIC_GPU_0_HALTED_IRQ    = (1 << 4),
    BASIC_GPU_1_HALTED_IRQ    = (1 << 5),
    BASIC_ACCESS_ERROR_1_IRQ  = (1 << 6),
    BASIC_ACCESS_ERROR_0_IRQ  = (1 << 7)
};

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}
//ARMTIMER_CTRL:
enum {

/** @brief 0 : 16-bit counters - 1 : 23-bit counter */
    RMTIMER_CTRL_16BIT = ( 0 << 1 ),
    ARMTIMER_CTRL_23BIT = ( 1 << 1 ),

    ARMTIMER_CTRL_PRESCALE_1   = ( 0 << 2 ),
    ARMTIMER_CTRL_PRESCALE_16  = ( 1 << 2 ),
    ARMTIMER_CTRL_PRESCALE_256 = ( 2 << 2 ),

/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
    ARMTIMER_CTRL_INT_ENABLE  = ( 1 << 5 ),
    ARMTIMER_CTRL_INT_DISABLE = ( 0 << 5 ),

/** @brief 0 : Timer disabled - 1 : Timer enabled */
    ARMTIMER_CTRL_ENABLE  = ( 1 << 7 ),
    ARMTIMER_CTRL_DISABLE = ( 0 << 7 ),

    ARMTIMER_CTRL_CNTR_DISABLE = (0 << 9),
    ARMTIMER_CTRL_CNTR_ENABLE = (1 << 9),
};

#define REG(reg) (*(volatile uint32_t *)reg)

#endif
