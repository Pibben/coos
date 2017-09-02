#include "uart.h"
#include "reg.h"
#include <string.h>

/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
    : : [count]"r"(count) : "cc");
}

Uart::Uart() : mUseLock(false)
{
    // Disable UART0.
    REG(UART0_CR) = 0x00000000;
    // Setup the GPIO pin 14 && 15.
    
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    REG(GPPUD) = 0x00000000;
    delay(150);
    
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    REG(GPPUDCLK0) = (1 << 14) | (1 << 15);
    delay(150);
    
    // Write 0 to GPPUDCLK0 to make it take effect.
    REG(GPPUDCLK0) = 0x00000000;
    
    // Clear pending interrupts.
    REG(UART0_ICR) = 0x7FF;
    
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
    
    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    REG(UART0_IBRD) = 1;
    REG(UART0_FBRD) = 40;
    
    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    REG(UART0_LCRH) = UART0_ENABLE_FIFOS | UART0_WORLD_LENGTH_8;
    
    // Mask all interrupts.
    REG(UART0_IMSC) = UART0_ALL_INT_MASK;
    
    // Enable UART0, receive & transfer part of UART.
    REG(UART0_CR) = UART0_UART_ENABLE | UART0_TRANSMIT_ENABLE | UART0_RECEIVE_ENABLE;
}

static void uart_putc(char byte)
{
    // Wait for UART to become ready to transmit.
    while ( REG(UART0_FR) & (1 << 5) ) { }
    REG(UART0_DR) = byte;
}

#if 0
static unsigned char uart_getc()
{
    // Wait for UART to have recieved something.
    while (REG(UART0_FR) & UART0_RX_FIFO_EMPTY) {

    }
    return REG(UART0_DR);
}
#endif

void Uart::write(const char* buffer, size_t size) {
    if (mUseLock) {
        mLock.lock();
    }
    for (size_t i = 0; i < size; i++) {
        uart_putc(buffer[i]);
    }
    if(mUseLock) {
        mLock.unlock();
    }
}

void Uart::write(const char* buffer) {
    write(buffer, strlen(buffer));
}

void Uart::writeU32(uint32_t x) {
    static const char HEX[] = "0123456789ABCDEF";
    char buf[] = "0x00000000";
    char *p = &buf[2];
    for(int i = 28; i >= 0; i -= 4) {
        *p++ = HEX[(x >> i) % 16];
    }
    write(buf, 10);
}

void Uart::enableLocking() {
    mUseLock = true;
}
