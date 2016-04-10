#include <unistd.h>


void uart_init();
void uart_write(const unsigned char* buffer, size_t size);
void uart_puts(const char* str);
