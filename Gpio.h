//
// Created by per on 2017-10-01.
//

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "utils.h"

namespace Gpio {
    void disablePullupAndPulldownInternal(uint32_t value);

    template<class... Ts>
    void disablePullupAndPulldown(uint_fast8_t pin, Ts... pins) {
        uint32_t allPins = Util::valueBuilder<Ts...>(0, pin, pins...);
        disablePullupAndPulldownInternal(allPins);
    }
}


#endif //KERNEL_GPIO_H
