#include <unistd.h>

#if defined(__cplusplus)
extern "C" {
#endif
void uart_init();
void uart_write(const unsigned char* buffer, size_t size);
void uart_puts(const char* str);
#if defined(__cplusplus)
} 
#endif
