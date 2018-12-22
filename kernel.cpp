#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vector>

#include "uart.h"
#include "system.h"
#include "timer.h"
#include "md5.h"
#include "mmu.h"
#include "smp.h"
#include "fpu.h"

void coreTest(void* arg) {
    char str[] = "Hello from X\n";
    str[strlen(str)-2] = smp::getCoreId()+'0';
    System::uart().write(str);
}

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    cpu::interrupt::enable();

    System::uart().write("Low level print OK!\r\n", 22);

    cpu::core::fpu::enable();

    printf("CLib printf OK!\r\n");

    //ATAG_Scanner((uint32_t*)atags);
    
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
        StaticTest(uint32_t value) {
            mValue = value;
        }
    };

    static StaticTest st(0x4711);

    if(st.mValue == 0x4711) {
        printf("Static object OK\r\n");
    } else {
        printf("Static object Failed\r\n");
    }

    auto& ev = System::eventloop();

    printf("ARM Timer ");
    System::armTimer().setTimer(0x400, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    printf("System Timer 0 ");
    System::systemTimer1().setTimer(1000*1000, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    printf("System Timer 2 ");
    System::systemTimer3().setTimer(1000*1000, [] {
        printf("OK!\r\n");
    });

    ev.runOne();
    printf("Local Timer ");
    System::localTimer().setTimer(1000*1000, [] {
        printf("OK!\r\n");
    });

    ev.runOne();

    cpu::core::mmu::enable();

    smp::start_core(1);
    smp::start_core(2);
    smp::start_core(3);

    System::uart().enableLocking(); //All cores must have mmu/cache (?) before enabling lock

    smp::run(1, coreTest, (void *) 1);
    smp::run(2, coreTest, (void *) 2);
    smp::run(3, coreTest, (void *) 3);

    MD5 md5;
    uint64_t t = System::localTimer().getValue();
    md5.update((unsigned char*)0x0, 1024*1024*2);
    md5.finalize();
    printf("MD5: [%s] (%llu ticks)\r\n", md5.hexdigest().c_str(), System::localTimer().getValue()-t);

    t = System::localTimer().getValue();
    for(volatile int i = 0; i < 1000*1000; ++i) {

    }
    printf("Buzy loop %llu ticks.\r\n", System::localTimer().getValue()-t);

    while ( true );
}

void *__dso_handle = 0;
