#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <vector>

#include "uart.h"
#include "system.h"
#include "timer.h"
#include "md5.h"
#include "mmu.h"
#include "smp.h"
#include "fpu.h"


#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    auto& system = System::instance();

    system.uart().write("Low level print OK!\r\n", 22);

    FPU::enable();

    printf("CLib printf OK!\r\n");
    
    std::vector<int> v;
    v.push_back(4711);
    if(v.front() == 4711) {
        printf("std::vector OK\r\n");
    } else {
        printf("std::vector failed\r\n");
    }

    class StaticTest {
    public:
        uint32_t mValue = 0x1234;
        StaticTest() {
            mValue = 0x4711;
        }
    };

    static StaticTest st;

    if(st.mValue == 0x4711) {
        printf("Static object OK\r\n");
    } else {
        printf("Static object Failed\r\n");
    }

    auto& ev = system.eventloop();

    printf("ARM Timer ");
    system.armTimer().setTimer(0x400, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    printf("System Timer 0 ");
    system.systemTimer1().setTimer(1000*1000, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    printf("System Timer 2 ");
    system.systemTimer3().setTimer(1000*1000, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    mmu::enable();
    //smp::parkCpus();

    MD5 md5;
    uint32_t t = system.systemTimer1().getValue();
    md5.update((unsigned char*)0x0, 1024*1024*2);
    md5.finalize();
    printf("MD5: [%s] (%lu us)\r\n", md5.hexdigest().c_str(), system.systemTimer1().getValue()-t);

    t = system.systemTimer1().getValue();
    for(volatile int i = 0; i < 1000*1000; ++i) {

    }
    printf("Buzy loop %lu us.\r\n", system.systemTimer1().getValue()-t);

    while ( true );
}

void *__dso_handle = 0;
