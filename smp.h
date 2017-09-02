//
// Created by per on 2016-04-15.
//

#ifndef KERNEL_SMP_H
#define KERNEL_SMP_H

extern "C" void core_wakeup(void);

namespace smp {

    typedef void (*start_fn_t)(void *);
    // wake up the additional cores
    void start_core(int core);
    void run(int core, start_fn_t fun, void* arg);
    uint_fast8_t getCoreId();
};

#endif //KERNEL_SMP_H
