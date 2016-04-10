#include "uart.h"
#include "reg.h"
#include <string.h>

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
    : : [count]"r"(count) : "cc");
}

void uart_init()
{
    // Disable UART0.
    mmio_write(UART0_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15.
    
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    delay(150);
    
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    
    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);
    
    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);
    
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
    
    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);
    
    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    mmio_write(UART0_LCRH, UART0_ENABLE_FIFOS | UART0_WORLD_LENGTH_8);
    
    // Mask all interrupts.
    mmio_write(UART0_IMSC, UART0_ALL_INT_MASK);
    
    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, UART0_UART_ENABLE | UART0_TRANSMIT_ENABLE | UART0_RECEIVE_ENABLE);
}

static void uart_putc(unsigned char byte)
{
    // Wait for UART to become ready to transmit.
    while ( mmio_read(UART0_FR) & (1 << 5) ) { }
    mmio_write(UART0_DR, byte);
}

#if 0
static unsigned char uart_getc()
{
    // Wait for UART to have recieved something.
    while (mmio_read(UART0_FR) & UART0_RX_FIFO_EMPTY) {

    }
    return mmio_read(UART0_DR);
}
#endif

void uart_write(const unsigned char* buffer, size_t size)
{
    for ( size_t i = 0; i < size; i++ )
        uart_putc(buffer[i]);
}

void uart_puts(const char* str)
{
    uart_write((const unsigned char*) str, strlen(str));
}
