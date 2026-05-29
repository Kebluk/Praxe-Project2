#include "NotArduino.h"
#include "Serial.h"
#include "MKL25Z4.h"
#include "soc_def.h"
#include "res_alloc.h"

// For UART1, the clock source is Bus Clock (MCGFLLCLK / 2).
// Bus Clock = (32.768 kHz * 640) / 2 = 10,485,760 Hz
#define UART1_CLOCK_HZ ((32768U * 640U) / 2U)

void Serial_begin(uint32_t baudrate) {
    // 1. Disable UART1 during configuration
    UART1->C2 = 0;

    // 2. Enable System Clock Gates
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;

    // 3. Configure UART1 pin mux for terminal TX/RX (PTC3/RX, PTC4/TX)
    PORT_UART1_RX->PCR[IOIND_UART1_RX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT3);
    PORT_UART1_TX->PCR[IOIND_UART1_TX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT3);

    // 4. Calculate SBR (baudDivisor) accounting for the fixed 16x oversampling ratio
    // UART1 uses Bus Clock = 10,485,760 Hz
    uint32_t uartClock = UART1_CLOCK_HZ;
    uint32_t baudDivisor = (uartClock + (baudrate * 8)) / (baudrate * 16);

    // 5. Set high and low bytes of the baud divisor
    UART1->BDH = (baudDivisor >> 8) & 0x1F;
    UART1->BDL = baudDivisor & 0xFF;

    // 6. Clear control registers and set default frame format
    UART1->C1 = 0;  // 8N1 format (8 data bits, no parity, 1 stop bit)
    UART1->C3 = 0;  // No interrupt on errors
    UART1->S2 = 0;  // Clear status

    // 7. Enable transmitter and receiver
    UART1->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
}

void Serial_write(uint8_t data) {
    while (!(UART1->S1 & UART_S1_TDRE_MASK)); // Wait until transmit buffer is empty
    UART1->D = data; // Write data to transmit buffer
}

void Serial_print(const char *s) {
    while (*s) {
        Serial_write((uint8_t)*s++);
    }
}

void Serial_println(const char *s) {
    Serial_print(s);
    Serial_print("\r\n");
}

void Serial_printInt(int32_t value) {
    if (value < 0) {
        Serial_write('-');
        value = -value;
    }
    Serial_printUInt((uint32_t)value);
}

void Serial_printUInt(uint32_t value) {
    char buf[12];
    int pos = 0;

    if (value == 0) {
        Serial_write('0');
        return;
    }

    while (value > 0) {
        buf[pos++] = '0' + (value % 10);
        value /= 10;
    }
    while (pos > 0) {
        Serial_write((uint8_t)buf[--pos]);
    }
}