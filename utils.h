//
// Created by per on 2017-09-02.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

namespace Util {

/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
    : : [count]"r"(count) : "cc");
}

};//ns

#endif //UTILS_H
