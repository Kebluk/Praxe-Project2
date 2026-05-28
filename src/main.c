#include "wdog.h"
#include "NotArduino.h"
#include "RGB.h"

void main(void) {
    wdog_init(WDOG_CONF_DIS);
    RGB_init();
    RGB_Set(OFF, OFF, HIGH);
    while (1) {
        wdog_refresh();
        
    }
}