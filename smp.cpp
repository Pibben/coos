//
// Created by per on 2016-04-15.
//

#include <stdint.h>
#include <stdio.h>
#include "smp.h"
#include "mmu.h"
#include "fpu.h"
#include "reg.h"
#include "system.h"

extern "C" void park_cpus();

namespace smp {
    // Setup SMP (Boot Offset = $4000008C + ($10 * Core), Core = 1..3)
    enum {
        CORE_BASE = 0x4000008C,

        Core0Boot = 0x00, // Core 0 Boot Offset
        Core1Boot = 0x10, // Core 1 Boot Offset
        Core2Boot = 0x20, // Core 2 Boot Offset
        Core3Boot = 0x30, // Core 3 Boot Offset
    };

    typedef void (*fn)(void);

    constexpr volatile fn *mailbox(int core) {
        return (volatile fn *) (CORE_BASE + core * 0x10);
    }

    // Multiprocessor Affinity Register (MPIDR)
    uint32_t get_mpidr(void) {
        uint32_t mpidr;
        asm volatile ("mrc p15,0,%0,c0,c0,5" : "=r" (mpidr));
        return mpidr;
    }

    extern "C" {
    void core_main(void);
    }

    static start_fn_t start_fn;
    void *start_arg;
    volatile bool started;

    void core_main(void) {
        System::instance().uart().write("core is up: MPIDR = ");
        System::instance().uart().writeU32(get_mpidr());
        System::instance().uart().write("\n");
        mmu::enable();
        System::instance().uart().write("core is virtual\n");
        FPU::enable();
        printf("core is floating\n");

        start_fn_t temp_fn = start_fn;
        void *temp_arg = start_arg;
        started = true;
        temp_fn(temp_arg);
        // FIXME: make restartable?
        while (true) { }
    }

    void start_core(int core, start_fn_t start, void *arg) {
        start_fn = start;
        start_arg = arg;
        printf("starting core %c\n", "0123"[core]);
        started = false;
        *mailbox(core) = core_wakeup;
        while (!started) { }
        printf("started core %c\n", "0123"[core]);
    }
}
