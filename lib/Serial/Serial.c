#include "NotArduino.h"
#include "Serial.h"
#include "MKL25Z4.h"
#include "soc_def.h"
#include "res_alloc.h"

// Reference to the system core clock frequency variable managed by CMSIS
extern uint32_t SystemCoreClock;

/**
 * @brief Initializes UART1 peripheral with specified baud rate
 * @param baudrate Target baud rate (e.g., 115200)
 */
void Serial_begin(uint32_t baudrate) {
    // Disable UART1 Transmitter & Receiver during setup
    UART1->C2 = 0; // Clears TE (Transmit Enable) and RE (Receive Enable)

    // Enable Clocks to Port C and UART1 Peripheral
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // Port C clock gate (for pins PTC3/PTC4)
    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK; // UART1 peripheral clock gate


    // Configure Pin Multiplexing to UART1 Alternate Function
    // PTC3 is mapped to UART1_RX (Alt 3)
    PORT_UART1_RX->PCR[IOIND_UART1_RX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT3);
    // PTC4 is mapped to UART1_TX (Alt 3)
    PORT_UART1_TX->PCR[IOIND_UART1_TX] = PORT_PCR_MUX(PORT_PCR_MUX_VAL_ALT3);

    // Calculate Dynamic Bus Clock and Baud Rate Divisor (SBR)
    // Standard UART1/2 are clocked by the Bus Clock.
    // Instead of hardcoding the clock, we dynamically read the core clock and the bus divider
    // to calculate the exact current Bus Clock frequency. This avoids any baud rate mismatch.
    uint32_t busClockDiv = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT) + 1;
    uint32_t uartClockHz = SystemCoreClock / busClockDiv;
    
    // Formula: Baudrate = UART1_Clock / (SBR * 16)
    // Therefore: SBR = UART1_Clock / (Baudrate * 16)
    // Note: (baudrate * 8) is added to perform nearest-integer rounding
    uint32_t baudDivisor = (uartClockHz + (baudrate * 8)) / (baudrate * 16);

    // Write Baud Rate Registers (BDH and BDL)
    UART1->BDH = (baudDivisor >> 8) & 0x1F; // High 5 bits of divisor
    UART1->BDL = baudDivisor & 0xFF;        // Low 8 bits of divisor

    // Configure Control Registers for Standard 8N1 Frame
    UART1->C1 = 0;  // 8 Data Bits, No Parity, 1 Stop Bit
    UART1->C3 = 0;  // Disable all interrupts, errors, and inverted signaling
    UART1->S2 = 0;  // Clear active edge/receive interrupts and status flags


    // Re-Enable Transmitter and Receiver
    UART1->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
}

/**
 * @brief Transmits a single byte of data over UART1
 * @param data Byte to send
 */
void Serial_write(uint8_t data) {
    // S1[TDRE] (Transmit Data Register Empty Flag) is set when the transmit buffer is ready
    while (!(UART1->S1 & UART_S1_TDRE_MASK)); 
    UART1->D = data; // Writing to D clears the TDRE flag and starts transmission
}

/**
 * @brief Prints a null-terminated string over UART1
 * @param s Pointer to the character string
 */
void Serial_print(const char *s) {
    while (*s) {
        Serial_write((uint8_t)*s++);
    }
}

/**
 * @brief Prints a null-terminated string followed by a newline (\r\n)
 * @param s Pointer to the character string
 */
void Serial_println(const char *s) {
    Serial_print(s);
    Serial_print("\r\n");
}

/**
 * @brief Converts and prints a signed 32-bit integer
 * @param value Signed value to print
 */
void Serial_printInt(int32_t value) {
    if (value < 0) {
        Serial_write('-');
        value = -value;
    }
    Serial_printUInt((uint32_t)value);
}

/**
 * @brief Converts and prints an unsigned 32-bit integer
 * @param value Unsigned value to print
 */
void Serial_printUInt(uint32_t value) {
    char buf[12];
    int pos = 0;

    if (value == 0) {
        Serial_write('0');
        return;
    }

    // Extract digits backwards
    while (value > 0) {
        buf[pos++] = '0' + (value % 10);
        value /= 10;
    }
    
    // Output digits in correct order
    while (pos > 0) {
        Serial_write((uint8_t)buf[--pos]);
    }
}