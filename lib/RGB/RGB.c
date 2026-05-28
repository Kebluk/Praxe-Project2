#include "NotArduino.h"
#include "RGB.h"

void RGB_init(void) {
    RGB_Set(OFF); // Initialize RGB to off
}

void RGB_Set(NotArduinoSwich state) {
    RGB_SetColor(RGB_RED, state);
    RGB_SetColor(RGB_GREEN, state);
    RGB_SetColor(RGB_BLUE, state);
}

void RGB_SetColor(RGBColor color, NotArduinoSwich state) {
    analogWrite((color == RGB_RED) ? PB18 : (color == RGB_GREEN) ? PB19 : PD1, (state == ON) ? 255 : 0);     // Set red channel
}

void RGB_AnalogWrite(uint8_t red, uint8_t green, uint8_t blue) {
    // Implementation for setting RGB values with PWM
    analogWrite(PB18, red);   // Set red value
    analogWrite(PB19, green); // Set green value
    analogWrite(PD1, blue);   // Set blue value
}

void RGB_SetColorBrightness(RGBColor color, uint8_t value) {
    // Implementation for setting brightness of a specific color using PWM
    switch (color) {
        case RGB_RED:
            analogWrite(PB18, value); // Set red brightness
            break;
        case RGB_GREEN:
            analogWrite(PB19, value); // Set green brightness
            break;
        case RGB_BLUE:
            analogWrite(PD1, value); // Set blue brightness
            break;
    }
}
