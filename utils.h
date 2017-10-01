//
// Created by per on 2017-09-02.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

namespace Util {

/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(uint32_t count)
{
    while(count--) {
        __asm__ __volatile__("");
    }
}
}

};//ns

#endif //UTILS_H
