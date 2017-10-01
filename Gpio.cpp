//
// Created by per on 2017-10-01.
//

#include "Gpio.h"
#include "reg.h"
#include "Register.h"
#include "utils.h"


static Register GPPUD    (GPIO_BASE + 0x94); // Controls actuation of pull up/down to ALL GPIO pins.
static Register GPPUDCLK0(GPIO_BASE + 0x98); // Controls actuation of pull up/down for specific GPIO pin.

namespace Gpio {
    void disablePullupAndPulldownInternal(uint32_t value) {
        // Disable pull up/down for all GPIO pins & delay for 150 cycles.
        GPPUD.clear();
        Util::delay(150);

        // Disable pull up/down for pin 14,15 & delay for 150 cycles.
        GPPUDCLK0.write((1 << 14) | (1 << 15));
        Util::delay(150);

        // Write 0 to GPPUDCLK0 to make it take effect.
        GPPUDCLK0.clear();
    }
}