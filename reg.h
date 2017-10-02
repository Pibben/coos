#include <stdint.h>

#ifndef REG_H
#define REG_H

enum {
    CPSR_MODE_USR = 0x10,
    CPSR_MODE_FIQ = 0x11,
    CPSR_MODE_IRQ = 0x12,
    CPSR_MODE_SVC = 0x13,
    CPSR_MODE_ABT = 0x17,
    CPSR_MODE_UND = 0x1B,
    CPSR_MODE_SYS = 0x1F,


    CPSR_FIQ = (1 << 6),
    CPSR_IRQ = (1 << 7),
};

enum {
    // The GPIO registers base address.
    PERIPHERAL_BASE = 0x3f000000,
    GPIO_BASE = 0x3f200000,

};





#endif
