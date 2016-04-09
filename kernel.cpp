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



#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    
    uart_init();
    uart_puts("Low level print OK!\r\n");
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

    auto& system = System::instance();
    auto& ev = system.eventloop();

    printf("Interrupts ");
    setTimer(0x400, [] {
        printf("OK!\r\n");
    });

    ev.runOne();


    while ( true );
}

void *__dso_handle = 0;
