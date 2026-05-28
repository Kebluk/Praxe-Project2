#include "NotArduino.h"
#include "MKL25Z4.h"

void Serial_begin(uint32_t baudrate) {
    // Configure UART0 for the specified baud rate
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Enable clock for UART0
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // Set UART0 clock source to MCGFLLCLK

    uint32_t uartClock = 20971520; // MCGFLLCLK frequency
    uint32_t baudDivisor = (uartClock + (baudrate / 2)) / baudrate; // Calculate baud divisor
    UART0->BDH = (baudDivisor >> 8) & 0x1F; // Set high byte of baud divisor
    UART0->BDL = baudDivisor & 0xFF; // Set low byte of baud divisor

    UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; // Enable transmitter and receiver
}

void Serial_write(uint8_t data) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK)); // Wait until transmit buffer is empty
    UART0->D = data; // Write data to transmit buffer
}