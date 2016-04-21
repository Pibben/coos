//
// Created by per on 2016-04-15.
//

#include "smp.h"

extern "C" void park_cpus();

namespace smp {
    void parkCpus() {
        park_cpus();
    }
};