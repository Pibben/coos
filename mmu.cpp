//
// Created by per on 2016-04-14.
//

#include "mmu.h"

#include <stdint.h>
#include "reg.h"

//http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0425/ch04s01s01.html
//https://github.com/mrvn/test/blob/master/mmu.cc


namespace mmu {
    static volatile __attribute__ ((aligned (0x4000))) uint32_t page_table[4096];
    void enable() {
        uint32_t base;
        for (base = 0; base < 1024 - 16; base++) {
            page_table[base] = base << 20 | 0x1540A;
        }

        //16 MB peripherals at 0x3F000000
        for (; base < 1024; base++) {
            page_table[base] = base << 20 | 0x10416; //shared device, never execute
        }

        page_table[base] = base << 20 | 0x10416; //shared device, never execute

        // unused up to 0x7FFFFFFF
        for (; base < 2048; base++) {
            page_table[base] = 0;
        }

        for (; base < 4096; base++) {
            page_table[base] = 0;
        }

        uint32_t auxctrl;
        asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
        auxctrl |= ACTLR_SMP;
        asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));

        // setup domains (CP15 c3)
        // Write Domain Access Control Register
        // use access permissions from TLB entry
        asm volatile ("mcr     p15, 0, %0, c3, c0, 0" :: "r" (0x55555555));

        // set domain 0 to client
        asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (1));

        // always use TTBR0
        asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));

        // set TTBR0 (page table walk inner and outer write-back,
        // write-allocate, cacheable, shareable memory)
        asm volatile ("mcr p15, 0, %0, c2, c0, 0"
        :: "r" (0b1001010 | (unsigned) &page_table));

        asm volatile ("isb" ::: "memory");

        // enable MMU, caches and branch prediction in SCTLR
        uint32_t mode;
        asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (mode));
        // mask: 0b0111 0011 0000 0010 0111 1000 0010 0111
        // bits: 0b0010 0000 0000 0000 0001 1000 0010 0111
        mode &= 0x73027827;
        mode |= 0x20001827;
        asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r" (mode) : "memory");
    }
};
