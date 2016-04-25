#include <unistd.h>
#include <stdint.h>

#ifndef UART_H
#define UART_H

class Uart {
private:
    bool mUseLock;
    int write_lock;
public:
    Uart();

    void write(const char* buffer, size_t size);
    void write(const char* buffer);
    void writeU32(uint32_t x);
    void enableLocking();
};

#endif
