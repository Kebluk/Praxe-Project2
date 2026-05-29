#include "wdog.h"
#include "NotArduino.h"
#include "RGB.h"
#include "PoT.h"
#include "Serial.h"

/**
 * @brief Active delay helper function.
 * This actively feeds (refreshes) the Watchdog Timer during the wait period.
 * * @param ms Delay time in milliseconds.
 */
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        // Calibration loop for ~1 millisecond.
        // On a core clock of ~20.97 MHz, 1ms is approx 21000 clock cycles.
        for (volatile uint32_t j = 0; j < 3000; j++) {
            __asm("nop");
        }
        wdog_refresh(); // Feed the watchdog to prevent reset
    }
}

void main(void) {
    wdog_init(WDOG_CONF_DIS); // Initialize watchdog config
    Serial_begin(115200);      // Initialize UART1 at 115200 baud
    RGB_SetColorBrightness(RGB_RED, 16); // Start with red off
    while (1) {
        wdog_refresh();         // Direct main-loop watchdog refresh
        int value1 = readPot1(); // Read analog value from POT (PTC2 / Channel 11)
        int value2 = readPot2(); // Read analog value from POT (PTC1 / Channel 10)
        Serial_print("value1 = ");
        Serial_printInt(map(value1, 0, 4099, 0, 255)); // Map 10-bit ADC value to 8-bit range for easier reading
        Serial_println("");
        RGB_SetColorBrightness(RGB_BLUE, map(value1, 0, 4099, 0, 255));
        Serial_print("value2 = ");
        Serial_printInt(map(value2, 0, 4099, 0, 255));
        Serial_println("");
        RGB_SetColorBrightness(RGB_GREEN, map(value2, 0, 4099, 0, 255));   // Map POT2 to Green brightness
        delay_ms(250); // Pause for 250ms to make the terminal easy to read!
    }
}