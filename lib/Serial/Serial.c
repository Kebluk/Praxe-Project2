#include "NotArduino.h"
#include "MKL25Z4.h"
#include "soc_def.h"
#include "res_alloc.h"

void Serial_begin(uint32_t baudrate) {
    // 1. Safety: Enable the System Clock Gate for PORTA (assuming RX/TX are on PTA1/PTA2)
    // If your pins are on PORTD, use SIM_SCGC5_PORTD_MASK instead.
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    // 2. Configure UART0 pin mux for terminal TX/RX (Alt 2)
    PORT_UART0_RX->PCR[IOIND_UART0_RX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT2);
    PORT_UART0_TX->PCR[IOIND_UART0_TX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT2);

    // 3. Configure UART0 clock gating and clock source
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Enable clock for UART0
    
    // Clear and set the UART0 clock source to MCGFLLCLK (1)
    SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(1);

    // 4. Calculate SBR (baudDivisor) accounting for the 16x oversampling ratio (OSR)
    uint32_t uartClock = 20971520; // MCGFLLCLK frequency
    
    // Formula: SBR = uartClock / (baudrate * 16)
    // Adding (baudrate * 8) provides mathematical rounding to the nearest integer
    uint32_t baudDivisor = (uartClock + (baudrate * 8)) / (baudrate * 16); 

    // Set high and low bytes of the baud divisor
    UART0->BDH = (baudDivisor >> 8) & 0x1F; 
    UART0->BDL = baudDivisor & 0xFF; 

    // Enable transmitter and receiver
    UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; 
}

void Serial_write(uint8_t data) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK)); // Wait until transmit buffer is empty
    UART0->D = data; // Write data to transmit buffer
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