#include <stdint.h>

#ifndef REG_H
#define REG_H

enum {
    CPSR_IRQ = (1 << 7)
};

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

    SYSTEM_TIMER_BASE = (PERIPHERAL_BASE + 0x3000),
    SYSTEM_TIMER_CS = (SYSTEM_TIMER_BASE + 0x00),
    SYSTEM_TIMER_CLO = (SYSTEM_TIMER_BASE + 0x04),
    SYSTEM_TIMER_CHI = (SYSTEM_TIMER_BASE + 0x08),
    SYSTEM_TIMER_C0 = (SYSTEM_TIMER_BASE + 0x0c),
    SYSTEM_TIMER_C1 = (SYSTEM_TIMER_BASE + 0x10),
    SYSTEM_TIMER_C2 = (SYSTEM_TIMER_BASE + 0x14),
    SYSTEM_TIMER_C3 = (SYSTEM_TIMER_BASE + 0x18),

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

//SYSTEM_TIMER_CS
enum {
    SYSTEM_TIMER_CS_M3 = (1 << 3),
    SYSTEM_TIMER_CS_M2 = (1 << 2),
    SYSTEM_TIMER_CS_M1 = (1 << 1),
    SYSTEM_TIMER_CS_M0 = (1 << 0)
};

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

//UART0_LCRH
enum {
    UART0_STICK_PARITY = (1 << 7),
    UART0_WORLD_LENGTH_5 = (0 << 5),
    UART0_WORLD_LENGTH_6 = (1 << 5),
    UART0_WORLD_LENGTH_7 = (2 << 5),
    UART0_WORLD_LENGTH_8 = (3 << 5),
    UART0_ENABLE_FIFOS = (1 << 4),
    UART0_TWO_STOP_BITS = (1 << 3),
    UART0_EVEN_PARITY = (1 << 2),
    UART0_ENABLE_PARITY = (1 << 1),
    UART0_SEND_BREAK = (1 << 0)
};

//UART0_IMSC
enum {
    UART0_OVERRUN_INT_MASK = (1 << 10),
    UART0_BREAK_INT_MASK = (1 << 9),
    UART0_PARITY_INT_MASK = (1 << 8),
    UART0_FRAMING_INT_MASK = (1 << 7),
    UART0_TIMEOUT_INT_MASK = (1 << 6),
    UART0_TRANSMIT_INT_MASK = (1 << 5),
    UART0_RECEIVE_INT_MASK = (1 << 4),
    UART0_CTS_INT_MASK = (1 << 1),
    UART0_ALL_INT_MASK = 0b11111110010
};

//UART0_CR
enum {
    UART0_CTS_ENABLE = (1 << 15),
    UART0_RTS_ENABLE = (1 << 14),
    UART0_RTS = (1 << 11),
    UART0_RECEIVE_ENABLE = (1 << 9),
    UART0_TRANSMIT_ENABLE = (1 << 8),
    UART0_LOOPBACK_ENABLE = (1 << 7),
    UART0_UART_ENABLE = (1 << 0)
};

//UART0_FR
enum {
    UART0_TX_FIFO_EMPTY = (1 << 7),
    UART0_RX_FIFO_FULL = (1 << 6),
    UART0_TX_FIFO_FULL = (1 << 5),
    UART0_RX_FIFO_EMPTY = (1 << 4),
    UART0_BUSY = (1 << 3),
    UART0_CTS = (1 << 0)
};

#define REG(reg) (*(volatile uint32_t *)(reg))

enum {
    ACTLR_SMP = (1 << 6),
    SCTLR_M = (1 << 0), //MMU, also enables BP (?)
    SCTLR_C = (1 << 2), //Data and unified cache. ACTLR_SMP must be enabled.
    SCTLR_I = (1 << 12) //Instruction cache
};

#endif
