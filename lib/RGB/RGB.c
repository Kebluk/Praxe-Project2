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
    // Note: The FRDM-KL25Z RGB LED is active-low (0 is fully bright, 255 is off)
    uint8_t duty = (state == ON) ? 255 : 0;
    
    analogWrite((color == RGB_RED) ? PB18 : (color == RGB_GREEN) ? PB19 : PD1, duty);     
}

void RGB_AnalogWrite(uint8_t red, uint8_t green, uint8_t blue) {
    // Invert the inputs since the RGB LED is active-low
    analogWrite(PB18, red);   // Set red value
    analogWrite(PB19, green); // Set green value
    analogWrite(PD1, blue);   // Set blue value
}

void RGB_SetColorBrightness(RGBColor color, uint8_t value) {
    // Invert the input value for active-low brightness adjustment
    analogWrite((color == RGB_RED) ? PB18 : (color == RGB_GREEN) ? PB19 : PD1, value);
}