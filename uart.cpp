#include "uart.h"
#include "utils.h"
#include "Register.h"
#include "Gpio.h"
#include "system.h"
#include <string.h>

static const uint32_t UART0_BASE = System::getPeripheralBase() + 0x201000;

static Register dr    (UART0_BASE + 0x00);
static Register fr    (UART0_BASE + 0x18);
static Register ibrd  (UART0_BASE + 0x24);
static Register fbrd  (UART0_BASE + 0x28);
static Register lcrh  (UART0_BASE + 0x2C);
static Register cr    (UART0_BASE + 0x30);
static Register imsc  (UART0_BASE + 0x38);
static Register icr   (UART0_BASE + 0x44);

enum {
    LCRH_SEND_BREAK,
    LCRH_ENABLE_PARITY,
    LCRH_EVEN_PARITY,
    LCRH_TWO_STOP_BITS,
    LCRH_ENABLE_FIFOS,
    LCRH_WORLD_LENGTH,
    LCRH_STICK_PARITY = 7
};

enum {
    CR_UART_ENABLE,
    CR_LOOPBACK_ENABLE = 7,
    CR_TRANSMIT_ENABLE,
    CR_RECEIVE_ENABLE,
    CR_RTS = 11,
    CR_RTS_ENABLE = 14,
    CR_CTS_ENABLE = 15
};

enum {
    IMSC_CTS_INT_MASK = 1,
    IMSC_RECEIVE_INT_MASK = 4,
    IMSC_TRANSMIT_INT_MASK,
    IMSC_TIMEOUT_INT_MASK,
    IMSC_FRAMING_INT_MASK,
    IMSC_PARITY_INT_MASK,
    IMSC_BREAK_INT_MASK,
    IMSC_OVERRUN_INT_MASK,
};

Uart::Uart() : mUseLock(false)
{
    // Disable UART0.
    cr.clear();
    // Setup the GPIO pin 14 && 15.
    Gpio::disablePullupAndPulldown(14,15);

    // Clear pending interrupts.
    icr.write(0x7FF);
    
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
    
    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    ibrd.write(1);
    fbrd.write(40);
    
    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    lcrh.setOnly(LCRH_ENABLE_FIFOS);
    lcrh.write(3, LCRH_WORLD_LENGTH);
    
    // Mask all interrupts.
    imsc.setOnly(IMSC_CTS_INT_MASK, IMSC_RECEIVE_INT_MASK, IMSC_TRANSMIT_INT_MASK, IMSC_TIMEOUT_INT_MASK,
                 IMSC_FRAMING_INT_MASK, IMSC_PARITY_INT_MASK, IMSC_BREAK_INT_MASK, IMSC_OVERRUN_INT_MASK);
    
    // Enable UART0, receive & transfer part of UART.
    cr.setOnly(CR_UART_ENABLE, CR_TRANSMIT_ENABLE, CR_RECEIVE_ENABLE);
}

static void uart_putc(char byte)
{
    // Wait for UART to become ready to transmit.
    while ( fr.get(5) ) { }
    dr.write(byte);
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
