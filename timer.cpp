//
// Created by per on 2016-04-03.
//

#include <stdio.h>

#include "timer.h"
#include "interrupts.h"
#include "system.h"
#include "Register.h"
#include "coprocessor.h"

static const uint32_t ARMTIMER_BASE = (System::getPeripheralBase() + 0xB400);

static Register ARMTIMER_LOAD     (ARMTIMER_BASE + 0x00);
//ARMTIMER_VALUE = (ARMTIMER_BASE + 0x04),
static Register ARMTIMER_CTRL     (ARMTIMER_BASE + 0x08);
static Register ARMTIMER_IRQ_CLEAR(ARMTIMER_BASE + 0x0C);
//ARMTIMER_IRQ_RAW = (ARMTIMER_BASE + 0x10),
//ARMTIMER_MASKED_IRQ = (ARMTIMER_BASE + 0x14),
//ARMTIMER_RELOAD = (ARMTIMER_BASE + 0x18),
//ARMTIMER_PREDIVIDER = (ARMTIMER_BASE + 0x1C),
//ARMTIMER_COUNTER = (ARMTIMER_BASE + 0x20),

//ARMTIMER_CTRL:
enum {

/** @brief 0 : 16-bit counters - 1 : 23-bit counter */
    //ARRMTIMER_CTRL_16BIT = ( 0 << 1 ),
    ARMTIMER_CTRL_23BIT = 1,

    //ARMTIMER_CTRL_PRESCALE_1   = ( 0 << 2 ),
    //ARMTIMER_CTRL_PRESCALE_16  = ( 1 << 2 ),
    //ARMTIMER_CTRL_PRESCALE_256 = ( 2 << 2 ),
    ARMTIMER_CTRL_PRESCALE,

/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
    ARMTIMER_CTRL_INT_ENABLE  = 5,
    //ARMTIMER_CTRL_INT_DISABLE = ( 0 << 5 ),

/** @brief 0 : Timer disabled - 1 : Timer enabled */
    ARMTIMER_CTRL_ENABLE  = 7,
    //ARMTIMER_CTRL_DISABLE = ( 0 << 7 ),

    //ARMTIMER_CTRL_CNTR_DISABLE = (0 << 9),
    ARMTIMER_CTRL_CNTR_ENABLE = 9,
};

void ArmTimer::enableTimer(uint32_t value) {
    cpu::interrupt::enableArmTimer();

    ARMTIMER_LOAD.write(value);

    ARMTIMER_CTRL.setOnly(ARMTIMER_CTRL_23BIT, ARMTIMER_CTRL_ENABLE, ARMTIMER_CTRL_INT_ENABLE);
    ARMTIMER_CTRL.write(2, ARMTIMER_CTRL_PRESCALE);

    cpu::interrupt::enable();
}

void ArmTimer::disableTimer() {
    cpu::interrupt::disableArmTimer();
}

static void clearIRQ(void) {
    ARMTIMER_IRQ_CLEAR.write(1);
}

void ArmTimer::handleTimerInterrupt() {
    clearIRQ();
    System::eventloop().postIntr(gTimerCallback);
    disableTimer();
}


static const uint32_t SYSTEM_TIMER_BASE = (System::getPeripheralBase() + 0x3000);
static Register SYSTEM_TIMER_CS  (SYSTEM_TIMER_BASE + 0x00);
static Register SYSTEM_TIMER_CLO (SYSTEM_TIMER_BASE + 0x04);
static Register SYSTEM_TIMER_CHI (SYSTEM_TIMER_BASE + 0x08);
static Register SYSTEM_TIMER_C0  (SYSTEM_TIMER_BASE + 0x0c);
static Register SYSTEM_TIMER_C1  (SYSTEM_TIMER_BASE + 0x10);
static Register SYSTEM_TIMER_C2  (SYSTEM_TIMER_BASE + 0x14);
static Register SYSTEM_TIMER_C3  (SYSTEM_TIMER_BASE + 0x18);

void SystemTimer::enableTimer(uint32_t value) {
    cpu::interrupt::enableSystemTimer(mTimerIdx);
    SYSTEM_TIMER_CS.set(mTimerIdx);

    const uint32_t c0Address = SYSTEM_TIMER_BASE + 0x0c;
    const uint32_t currentValue = SYSTEM_TIMER_CLO.read();
    Register timerValueRegister(c0Address+mTimerIdx*4);

    timerValueRegister.write(currentValue + value);

    cpu::interrupt::enable();
}

void SystemTimer::disableTimer() {
    cpu::interrupt::disableSystemTimer(mTimerIdx);
}

void SystemTimer::handleTimerInterrupt() {
    SYSTEM_TIMER_CS.set(mTimerIdx);
    System::eventloop().postIntr(gTimerCallback);
    disableTimer();
}

uint32_t SystemTimer::getValue() {
    return SYSTEM_TIMER_CLO.read();
}

static const uint32_t LOCAL_PERIPHERAL_BASE = (System::getLocalPeripheralBase());
static Register CORE_TIMER_CTL (LOCAL_PERIPHERAL_BASE + 0x00);
static Register CORE_TIMER_PRESCALE (LOCAL_PERIPHERAL_BASE + 0x08);

void LocalTimer::enableTimer(uint32_t value) {
    //TODO: Frequency in CNTFRQ (not set on boot?) QEMU is 62.5MHz.
    //Clock source in 0x40000000, crystal clock (default) is 19.2MHz
    //Pre-scaler in 0x40000008, set to 0x06AAAAAB for 1MHz clock.

    WRITE_CP32(value, CNTP_TVAL);
    cpu::interrupt::enableCoreTimer();
    WRITE_CP32(1, CNTP_CTL); // ENABLE

    cpu::interrupt::enable();
}

void LocalTimer::disableTimer() {
    cpu::interrupt::disableCoreTimer();
    WRITE_CP32(0, CNTP_CTL); // ~ENABLE
}

void LocalTimer::handleTimerInterrupt() {
    WRITE_CP32(0, CNTP_TVAL);
    System::eventloop().postIntr(gTimerCallback);
    disableTimer();
}

uint64_t LocalTimer::getValue() {
    uint64_t value;
    READ_CP64(value, CNTPCT);
    return value;
}