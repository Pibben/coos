//
// Created by per on 2016-04-15.
//

#ifndef KERNEL_SMP_H
#define KERNEL_SMP_H

extern "C" void core_wakeup(void);

namespace smp {

    typedef void (*start_fn_t)(void *);
    // wake up the additional cores
    void start_core(int core, start_fn_t start, void *arg);
};

#endif //KERNEL_SMP_H
