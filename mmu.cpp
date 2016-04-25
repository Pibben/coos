//
// Created by per on 2016-04-14.
//

#include "mmu.h"

#include <stdint.h>
#include "reg.h"

//http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0425/ch04s01s01.html
//https://github.com/mrvn/test/blob/master/mmu.cc

enum {
    MMU_L1_SEC_IS_SECTION = (2 << 0),
    MMU_L1_SEC_CB_00 = (0 << 2),
    MMU_L1_SEC_CB_01 = (1 << 2),
    MMU_L1_SEC_CB_10 = (2 << 2),
    MMU_L1_SEC_CB_11 = (3 << 2),
    MMU_L1_SEC_XN = (1 << 4),
    MMU_L1_SEC_DOMAIN = (1 << 5),
    MMU_L1_SEC_AP = (1 << 10), //Access, when SCTLR.AFE is 1
    MMU_L1_SEC_TEX_000 = (0 << 12),
    MMU_L1_SEC_TEX_001 = (1 << 12),
    MMU_L1_SEC_TEX_010 = (2 << 12),
    MMU_L1_SEC_TEX_011 = (3 << 12),
    MMU_L1_SEC_TEX_100 = (4 << 12),
    MMU_L1_SEC_TEX_101 = (5 << 12),
    MMU_L1_SEC_TEX_110 = (6 << 12),
    MMU_L1_SEC_TEX_111 = (7 << 12),
    MMU_L1_SEC_AP2 = (1 << 15), //R/O
    MMU_L1_SEC_S = (1 << 16),
    MMU_L1_SEC_NG = (1 << 17), //Not global
    MMU_L1_SEC_IS_SUPERSECTION = (1 << 18),

    MMU_L1_SEC_OWB_WA_IWB_WA = (MMU_L1_SEC_TEX_001 | MMU_L1_SEC_CB_11),
    MMU_L1_SEC_OWB_WA_IWT_NWA = (MMU_L1_SEC_TEX_101 | MMU_L1_SEC_CB_10),
    MMU_L1_SEC_DEVICE = (MMU_L1_SEC_TEX_000 | MMU_L1_SEC_CB_01),

    MMU_L1_TBL_IS_TABLE = (1 << 0),
    MMU_L2_PAGE_IS_LARGE = (1 << 0),
    MMU_L2_PAGES_SMALL_XN = (1 << 0),
    MMU_L2_PAGE_IS_SMALL = (1 << 1),
    MMU_L2_PAGE_CB_00 = (0 << 2),
    MMU_L2_PAGE_CB_01 = (1 << 2),
    MMU_L2_PAGE_CB_10 = (2 << 2),
    MMU_L2_PAGE_CB_11 = (3 << 2),
    MMU_L2_PAGE_AP = (1 << 4),
    MMU_L2_PAGE_SMALL_TEX_000 = (0 << 6),
    MMU_L2_PAGE_SMALL_TEX_001 = (1 << 6),
    MMU_L2_PAGE_SMALL_TEX_010 = (2 << 6),
    MMU_L2_PAGE_SMALL_TEX_011 = (3 << 6),
    MMU_L2_PAGE_SMALL_TEX_100 = (4 << 6),
    MMU_L2_PAGE_SMALL_TEX_101 = (5 << 6),
    MMU_L2_PAGE_SMALL_TEX_110 = (6 << 6),
    MMU_L2_PAGE_SMALL_TEX_111 = (7 << 6),
    MMU_L2_PAGE_SMALL_AP2 = (1 << 9),
    MMU_L2_PAGE_SMALL_S = (1 << 10),
    MMU_L2_PAGE_SMALL_NG = (1 << 11),
//MMU_L2_PAGE_OWB_WA_IWB_WA = (MMU_l2_PAGE_SMALL_TEX_001 | MMU_L2_PAGE_CB_11),
    MMU_L2_PAGE_OWB_WA_IWB_WA = (MMU_L2_PAGE_SMALL_TEX_101 | MMU_L2_PAGE_CB_01),
};

// MMU_L2_PAGE_IS_SMALL | MMU_l2_PAGE_SMALL_S | MMU_L2_PAGE_OWB_WA_IWB_WA | MMU_L2_PAGE_AP | MMU_L2_PAGES_SMALL_XN

namespace mmu {
    static volatile __attribute__ ((aligned (0x4000))) uint32_t page_table[4096];
    void enable() {
        uint32_t base;
        for (base = 0; base < 1024 - 16; base++) {
            page_table[base] = base << 20 | MMU_L1_SEC_IS_SECTION | MMU_L1_SEC_S | MMU_L1_SEC_OWB_WA_IWB_WA |
                                                                                   MMU_L1_SEC_AP;
        }

        //16 MB peripherals at 0x3F000000
        for (; base < 1024; base++) {
            page_table[base] = base << 20 | MMU_L1_SEC_IS_SECTION | MMU_L1_SEC_S | MMU_L1_SEC_XN | MMU_L1_SEC_DEVICE |
                                                                                                   MMU_L1_SEC_AP;
        }

        //Mailboxes at 0x40000000
        page_table[base] = base << 20 | MMU_L1_SEC_IS_SECTION | MMU_L1_SEC_S | MMU_L1_SEC_XN | MMU_L1_SEC_DEVICE |
                                                                                               MMU_L1_SEC_AP;
        ++base;

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

        // enable MMU and D/L2 cache in SCTLR
        uint32_t mode;
        asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (mode));
        mode |= (SCTLR_M | SCTLR_C | SCTLR_I | SCTLR_AFE);
        asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r" (mode) : "memory");
    }
};
