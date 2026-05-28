#include "wdog.h"
#include "NotArduino.h"
#include "RGB.h"
#include "PoT.h"
#include "Serial.h"

void main(void) {
    wdog_init(WDOG_CONF_DIS);
    RGB_init();
    RGB_Set(OFF, OFF, ON);
    while (1) {
        wdog_refresh();
        int value = readValue();
        Serial_begin(115200);
        Serial_write(value & 0xFF); // Send lower byte of the value
        Serial_write((value >> 8) & 0xFF); // Send upper byte of the value
    }
}