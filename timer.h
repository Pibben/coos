//
// Created by per on 2016-04-03.
//

#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <functional>

class ArmTimer {
private:
    std::function<void(void)> gTimerCallback;
    void enableTimer(uint32_t value);
    void disableTimer();
public:
    void handleTimerInterrupt();

    template <class Func>
    void setTimer(uint32_t value, Func&& func) {
        gTimerCallback = std::forward<Func>(func);
        enableTimer(value);
    }
};

class SystemTimer {
    std::function<void(void)> gTimerCallback;
    void enableTimer(uint32_t value);
    void disableTimer();
    uint8_t mTimerIdx;
public:
    SystemTimer(uint8_t timerIdx) : mTimerIdx(timerIdx) {}
    void handleTimerInterrupt();
    uint32_t getValue();

    template <class Func>
    void setTimer(uint32_t value, Func&& func) {
        gTimerCallback = std::forward<Func>(func);
        enableTimer(value);
    }
};


#endif //KERNEL_TIMER_H
