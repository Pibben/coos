//
// Created by per on 2016-04-21.
//

#include <stdint.h>

#include "fpu.h"
#include "reg.h"

namespace FPU {
    void enable() {
        uint32_t reg;

        asm volatile("mrc p15, 0, %0, c1, c0, 2" : "=r" (reg)); // CPACR - Coprocessor Access Control Register
        reg |= CPACR_CP10_ACCESS_ALL | CPACR_CP11_ACCESS_ALL;
        asm volatile("mcr p15,0,%0,c1,c0,2" :: "r" (reg));

        asm volatile("vmsr fpexc,%0" :: "r" (FPEXC_EN));
    }
};