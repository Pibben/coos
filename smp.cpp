//
// Created by per on 2016-04-15.
//

#include <stdint.h>
#include <stdio.h>
#include "smp.h"
#include "mmu.h"
#include "fpu.h"
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
    uint_fast8_t getCoreId() {
        uint32_t mpidr;
        asm volatile ("mrc p15,0,%0,c0,c0,5" : "=r" (mpidr));
        return mpidr & 0x3;
    }

    extern "C" {
    void core_main(void);
    }

    volatile bool started;

    volatile static start_fn_t tasks[4];
    static void* taskArgs[4];

    static inline void delay(int32_t count)
    {
        asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
        : : [count]"r"(count) : "cc");
    }

    void nullTask(void) {
        uint_fast8_t coreId = getCoreId();
        while(true) {
            if (tasks[coreId]) {
                tasks[coreId](taskArgs[coreId]);
                tasks[coreId] = nullptr;
            }
        }
    }

    void core_main(void) {
        System::uart().write("core is up: ID = ");
        System::uart().writeU32(getCoreId());
        System::uart().write("\n");
        cpu::core::mmu::enable();
        System::uart().write("core is virtual\n");
        cpu::core::fpu::enable();
        printf("core is floating\n");

        started = true;
        nullTask();
    }

    void start_core(int core) {
        printf("starting core %c\n", "0123"[core]);
        started = false;
        *mailbox(core) = core_wakeup;
        while (!started) { }
        printf("started core %c\n", "0123"[core]);
    }

    void run(int core, start_fn_t fun, void* arg) {
        taskArgs[core] = arg;
        tasks[core] = fun;
    }
}
