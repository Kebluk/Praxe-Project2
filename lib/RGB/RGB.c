#include "NotArduino.h"
#include "RGB.h"
#include "MKL25Z4.h"

void RGB_init(void) {
    /*                      Configure TPM0 for PWM
    TPM0->CONTROLS[0].CnSC |= 0b01010; // Edge-aligned PWM, high-true pulses
    TPM0->CONTROLS[1].CnSC |= 0b01010; // Edge-aligned PWM, high-true pulses
    TPM0->CONTROLS[2].CnSC |= 0b01010; // Edge-aligned PWM, high-true pulses
    */

    pinMode(PB18, OUTPUT); // Red
    pinMode(PB19, OUTPUT); // Green
    pinMode(PD1, OUTPUT);  // Blue
    RGB_Set(OFF, OFF, OFF); // Initialize RGB to off
    }

void RGB_Set(NotArduinoSwich red, NotArduinoSwich green, NotArduinoSwich blue) {
    // Implementation for setting RGB values with PWM
    digitalWrite(PB18, !red);   // Set red value
    digitalWrite(PB19, !green); // Set green value
    digitalWrite(PD1, !blue);   // Set blue value
}

void RGB_SetBrightness(RGBColor color, uint8_t value) {
    // Implementation for setting brightness of a specific color using PWM
    switch (color) {
        case RGB_RED:
            TPM0->CONTROLS[0].CnV = value; // Set red brightness
            break;
        case RGB_GREEN:
            TPM0->CONTROLS[1].CnV = value; // Set green brightness
            break;
        case RGB_BLUE:
            TPM0->CONTROLS[2].CnV = value; // Set blue brightness
            break;
    }

}
