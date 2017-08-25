//
// Created by per on 2016-04-27.
//

#include "process.h"
#include "ProcessTable.h"
#include "reg.h"

extern "C" void contextSwitch(uintptr_t*, uintptr_t*);

using Func = void(*)(void);

static uintptr_t createStack(Func func, uintptr_t stack, uintptr_t retAddr) {
    static constexpr uint8_t CONTEXT_WORDS = 15;
    uint32_t* sAddr = reinterpret_cast<uint32_t*>(stack);
    sAddr -= CONTEXT_WORDS;

    for(int i = 0; i < CONTEXT_WORDS - 3; ++i) {
        //Insert args here
        sAddr[i] = 0;
    }

    sAddr[CONTEXT_WORDS - 3] = CPSR_MODE_SYS | CPSR_FIQ;
    sAddr[CONTEXT_WORDS - 2] = static_cast<uint32_t>(retAddr);
    sAddr[CONTEXT_WORDS - 1] = reinterpret_cast<uint32_t>(func);

    return reinterpret_cast<uintptr_t>(sAddr);
}

namespace Scheduler {
    ProcessTable<Process> processTable;
    ProcessTable<Process>::List* ready;
    //ProcessTable<Process>::List* running;
    ProcessTable<Process>::List* suspended;
    size_t currentPid;

    bool needsReschedule() {
        if(processTable.listFront(ready)->mPrio > processTable.get(currentPid).mPrio) {
            return true;
        } else {
            return false;
        }
    }

    void schedule() {
        Process& oldProc = processTable.get(currentPid);
        Process* newProc = processTable.listPopFront(ready);
        processTable.moveToListBack(currentPid, &ready);
        oldProc.mState = State::READY;
        newProc->mState = State::RUNNING;
        currentPid = newProc->mPID;
        contextSwitch(&oldProc.mStack, &newProc->mStack);
    }

    void suspend(size_t pid) {
        auto& p = processTable.get(pid);
        p.mState = State::SUSPENDED;
        if(pid == currentPid) {
            schedule();
        }
    }

    void resume(size_t pid) {
        auto& p = processTable.get(pid);
        p.mState = State::READY;
        if(needsReschedule()) {
            schedule();
        }
    }

    size_t create(Func func, uintptr_t stack) {
        for(size_t i = 0; i < processTable.size(); ++i) {
            auto& p = processTable.get(i);
            if(p.mState == State::FREE) {
                p.mPID = i;
                p.mNext = i;
                p.mPrev = i;
                p.mStack = createStack(func, stack, 0);
                p.mState = State::SUSPENDED;


                processTable.moveToListBack(i, &suspended);
                return i;
            }
        }

        return 0;
    }

    void createNullProc() {
        auto& p = processTable.get(0);
        p.mPID = 0;
        p.mPrev = 0;
        p.mNext = 0;
        p.mPrio = 0;
        p.mStack = 0;
        p.mState = State::RUNNING;
        currentPid = 0;
    }
};