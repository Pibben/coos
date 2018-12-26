
#include <stdint.h>
#include <stdbool.h>


#include "interrupts.h"
#include "timer.h"
#include "system.h"
#include "Register.h"

static const uint32_t INTERRUPT_CONTROLLER_BASE = (System::getPeripheralBase() + 0xB200);


static Register IRQ_BASIC_PENDING (INTERRUPT_CONTROLLER_BASE + 0x00);
static Register IRQ_PENDING_1     (INTERRUPT_CONTROLLER_BASE + 0x04);
//IRQ_PENDING_2 = (INTERRUPT_CONTROLLER_BASE + 0x08),
//FIQ_CTRL = (INTERRUPT_CONTROLLER_BASE + 0x0C),
static Register ENABLE_IRQS_1     (INTERRUPT_CONTROLLER_BASE + 0x10);
//ENABLE_IRQS_2 = (INTERRUPT_CONTROLLER_BASE + 0x14),
static Register ENABLE_BASIC_IRQS (INTERRUPT_CONTROLLER_BASE + 0x18);
static Register DISABLE_IRQS_1    (INTERRUPT_CONTROLLER_BASE + 0x1C);
//DISABLE_IRQS_2 = (INTERRUPT_CONTROLLER_BASE + 0x20),
static Register DISABLE_BASIC_IRQS(INTERRUPT_CONTROLLER_BASE + 0x24);

static const uint32_t LOCAL_IRQ_BASE = (System::getLocalPeripheralBase() + 0x60);
static Register CORE0_IRQ_SOURCE (LOCAL_IRQ_BASE + 0x00);


namespace {
//BASIC IRQs
    enum {
        BASIC_ARM_TIMER_IRQ,
        BASIC_ARM_MAILBOX_IRQ,
        BASIC_ARM_DOORBELL_0_IRQ,
        BASIC_ARM_DOORBELL_1_IRQ,
        BASIC_GPU_0_HALTED_IRQ,
        BASIC_GPU_1_HALTED_IRQ,
        BASIC_ACCESS_ERROR_1_IRQ,
        BASIC_ACCESS_ERROR_0_IRQ
    };

    //Core Timer interrupts
    enum {
        CNTPSIRQ,
        CNTPNSIRQ,
        CNTHPIRQ,
        CNTVIRQ
    };
}

namespace cpu {
    namespace interrupt {
        void enableArmTimer() {
            ENABLE_BASIC_IRQS.set(BASIC_ARM_TIMER_IRQ);
        }

        void disableArmTimer() {
            DISABLE_BASIC_IRQS.set(BASIC_ARM_TIMER_IRQ);
        }

        void enableSystemTimer(uint_fast8_t timerIdx) {
            ENABLE_IRQS_1.set(timerIdx);
        }

        void disableSystemTimer(uint_fast8_t timerIdx) {
            DISABLE_IRQS_1.set(timerIdx);
        }

        void enable() {
            asm volatile ("cpsie i");
        }

        void disable() {
            asm volatile ("cpsid i");
        }
    }
}

extern "C"
void interruptHandler()
{
    if(IRQ_BASIC_PENDING.get(BASIC_ARM_TIMER_IRQ)) {
        System::armTimer().handleTimerInterrupt();
    } else if(IRQ_PENDING_1.get(1)) {
        System::systemTimer1().handleTimerInterrupt();
    } else if(IRQ_PENDING_1.get(3)) {
        System::systemTimer3().handleTimerInterrupt();
    } else if(CORE0_IRQ_SOURCE.get(CNTPNSIRQ)) {
        System::localTimer().handleTimerInterrupt();
    }
}

