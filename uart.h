#include <unistd.h>

#ifndef UART_H
#define UART_H

class Uart {
public:
    Uart();

    void write(const char* buffer, size_t size);
};

#endif