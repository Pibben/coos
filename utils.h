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

template<class=void>
uint32_t valueBuilder(uint32_t value) {
    return value;
}

template<class=void, class... Tail>
uint32_t valueBuilder(uint32_t value, uint_fast8_t bit, Tail... tail) {
    value |= (1 << bit);
    return valueBuilder<Tail...>(value, tail...);
}

};//ns

#endif //UTILS_H
