//
// Created by per on 2016-05-01.
//

#ifndef KERNEL_PROCESSTABLE_H
#define KERNEL_PROCESSTABLE_H


#include <array>
#include "process.h"

#define NUM_PROC 64

template <class T>
class ProcessTable {
public:
    struct List {
        T mPayload;
        size_t mPrev;
        size_t mNext;
    };
private:
    std::array<List, NUM_PROC> mTable;
    size_t mNumProcesses;

    void unlinkUnsafe(size_t pid) {
        mTable[mTable[pid].mNext].mPrev = mTable[pid].mPrev;
        mTable[mTable[pid].mPrev].mNext = mTable[pid].mNext;
    }

    void unlink(size_t pid) {
        unlinkUnsafe(pid);
        mTable[pid].mPrev = pid;
        mTable[pid].mNext = pid;
    }

    void unlink(List* list) {
        unlink(mTable[list->mNext].mPrev);
    }

    void link(size_t pid, List* pre) {
        List& l = mTable[pid];

        l.mNext = pre->mNext;
        l.mPrev = mTable[pre->mNext].mPrev;
        pre->mNext = pid;
        mTable[pre->mNext].mPrev = pid;
    }

public:
    size_t add(const T& t) {
        //TODO: Lock
        List l;
        l.mPayload = t;
        l.mNext = -1;
        l.mPrev = -1;

        mTable[mNumProcesses] = l;
        mNumProcesses++;
        return mNumProcesses-1;
    }

    T& get(size_t pid) {
        return mTable[pid].mPayload;
    }

    void moveToListBack(size_t pid, List** list) {
        List& l = mTable[pid];
        if(*list == nullptr) {
            *list = &l;
        } else {
            unlinkUnsafe(pid);

            List* back = &mTable[mTable[pid].mPrev];

            link(pid, back);
        }
    }

    T* listFront(List* list) {
        return list ? &list->mPayload : nullptr;
    }

    T* listPopFront(List* list) {
        T* retval = listFront(list);
        unlink(list);
        return retval;
    }

    size_t size() { return mTable.size(); }
};


#endif //KERNEL_PROCESSTABLE_H
