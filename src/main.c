#include "wdog.h"
#include "NotArduino.h"
#include "RGB.h"
#include "PoT.h"
#include "Serial.h"

void main(void) {
    wdog_init(WDOG_CONF_DIS);
    Serial_begin(115200);
    RGB_init();
    RGB_Set(ON);
    

    while (1) {
        int value = readValue();

        Serial_print("value = ");
        Serial_printInt(value);
        Serial_println("");
    }
}