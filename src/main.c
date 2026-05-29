#include "wdog.h"
#include "NotArduino.h"
#include "RGB.h"
#include "PoT.h"
#include "Serial.h"

void main(void) {
    wdog_init(WDOG_CONF_DIS); // Initialize watchdog config
    Serial_begin(115200);      // Initialize UART1 at 115200 baud
    RGB_SetColorBrightness(RGB_RED, 32); // Start with red off
    while (1) {
        wdog_refresh();         // Direct main-loop watchdog refresh
        int value1 = readPot1(); // Read analog value from POT (PTC2 / Channel 11)
        int value2 = readPot2(); // Read analog value from POT (PTC1 / Channel 10)
        int mappedValue1 = map(value1, 0, 4098, 0, 255); // Map 10-bit ADC value to 8-bit range for easier reading
        int mappedValue2 = map(value2, 0, 4098, 0, 255); // Map 10-bit ADC value to 8-bit range for easier reading
        Serial_print("Raw value1 = ");
        Serial_printInt(value1);
        Serial_println("");
        Serial_print("Mapped value1 = ");
        Serial_printInt(mappedValue1); // Map 10-bit ADC value to 8-bit range for easier reading
        Serial_println("");
        RGB_SetColorBrightness(RGB_BLUE, mappedValue1);
        Serial_print("Raw value2 = ");
        Serial_printInt(value2);
        Serial_println("");
        Serial_print("Mapped value2 = ");
        Serial_printInt(mappedValue2);
        Serial_println("");
        RGB_SetColorBrightness(RGB_GREEN, mappedValue2);   // Map POT2 to Green brightness
        delay_ms(250); // Pause for 250ms to make the terminal easy to read!
    }
}