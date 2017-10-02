//
// Created by per on 2016-04-21.
//

#include <stdint.h>

#include "fpu.h"

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

namespace fpu {
    void enable() {
        uint32_t reg;

        asm volatile("mrc p15, 0, %0, c1, c0, 2" : "=r" (reg)); // CPACR - Coprocessor Access Control Register
        reg |= CPACR_CP10_ACCESS_ALL | CPACR_CP11_ACCESS_ALL;
        asm volatile("mcr p15,0,%0,c1,c0,2" :: "r" (reg));

        asm volatile("vmsr fpexc,%0" :: "r" (FPEXC_EN));
    }
};