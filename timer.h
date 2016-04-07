//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <functional>

extern std::function<void(void)> gTimerCallback;

void enableTimer(uint32_t value);
void disableTimer();
void handleTimerInterrupt();

template <class Func>
void setTimer(uint32_t value, Func&& func) {
    gTimerCallback = std::forward<Func>(func);
    enableTimer(value);
}



#endif //KERNEL_TIMER_H
