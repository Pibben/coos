//
// Created by per on 2017-10-02.
//

#ifndef KERNEL_CORE_H
#define KERNEL_CORE_H

enum {
    CPSR_MODE_USR = 0x10,
    CPSR_MODE_FIQ = 0x11,
    CPSR_MODE_IRQ = 0x12,
    CPSR_MODE_SVC = 0x13,
    CPSR_MODE_ABT = 0x17,
    CPSR_MODE_UND = 0x1B,
    CPSR_MODE_SYS = 0x1F,


    CPSR_FIQ = (1 << 6),
    CPSR_IRQ = (1 << 7),
};

#endif //KERNEL_CORE_H
