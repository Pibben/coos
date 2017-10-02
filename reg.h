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
    //GPIO_BASE = 0x20200000,
            PERIPHERAL_BASE = 0x3f000000,
    GPIO_BASE = 0x3f200000,

};


//SYSTEM_TIMER_CS
enum {
    SYSTEM_TIMER_CS_M3 = (1 << 3),
    SYSTEM_TIMER_CS_M2 = (1 << 2),
    SYSTEM_TIMER_CS_M1 = (1 << 1),
    SYSTEM_TIMER_CS_M0 = (1 << 0)
};



#define REG(reg) (*(volatile uint32_t *)(reg))

enum {
    ACTLR_SMP = (1 << 6),
    SCTLR_M = (1 << 0), //MMU, also enables BP (?)
    SCTLR_C = (1 << 2), //Data and unified cache. ACTLR_SMP must be enabled.
    SCTLR_I = (1 << 12), //Instruction cache
    SCTLR_AFE = (1 << 29),
};

enum {
    CPACR_CP10_ACCESS_DENIED = (0 << 20),
    CPACR_CP10_ACCESS_P1 = (1 << 20),
    CPACR_CP10_ACCESS_ALL = (3 << 20),
    CPACR_CP11_ACCESS_DENIED = (0 << 22),
    CPACR_CP11_ACCESS_P1 = (1 << 22),
    CPACR_CP11_ACCESS_ALL = (3 << 22),

    CPACR_D32DIS = (1 << 30),
    CPACR_ASEDIS = (1 << 31),
};

enum {
    FPEXC_EN = (1 << 30),
};

#endif
