#include "NotArduino.h"
#include "MKL25Z4.h"
#include "soc_def.h"
#include "res_alloc.h"

void Serial_begin(uint32_t baudrate) {
    // Configure UART0 pin mux for terminal TX/RX
    PORT_UART0_RX->PCR[IOIND_UART0_RX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT2);
    PORT_UART0_TX->PCR[IOIND_UART0_TX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT2);

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

void initComunication(){
	// EN: set input multiplexer in port to UART1 peripherals
	PORTC->PCR[3] = 0b11<<8;
	PORTC->PCR[4] = 0b11<<8;

	// EN: allow ENVIC for receiving interrupts form UART1
	NVIC_SetPriority(UART1_IRQn, 2);
	NVIC_EnableIRQ(UART1_IRQn);

	// EN: enable sending interrupt for incoming and finished messages
	UART1->C2 = (UART_C2_RIE_MASK | UART_C2_TE_MASK | UART_C2_RE_MASK);

	// EN: setup speed of communication to 115200Bd
	UART1->BDH = UART_BDH_SBR(13u >> 8u);
	UART1->BDL = UART_BDL_SBR(13u);


}