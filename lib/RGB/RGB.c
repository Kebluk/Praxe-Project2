#include "NotArduino.h"
#include "RGB.h"
#include "MKL25Z4.h"

void RGB_init(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable clock for PORTB
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; // Enable clock for PORTD

    pinMode(PB18, OUTPUT); // Red
    pinMode(PB19, OUTPUT); // Green
    pinMode(PD1, OUTPUT);  // Blue
    RGB_Set(OFF, OFF, OFF); // Initialize RGB to off
    // Configure TPM0 for PWM
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Set TPM clock source to MCGFLLCLK
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // Enable clock for TPM0
    TPM0->CONTROLS[0].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge-aligned PWM, high-true pulses
    TPM0->CONTROLS[1].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge-aligned PWM, high-true pulses
    TPM0->CONTROLS[2].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge-aligned PWM, high-true pulses
    TPM0->MOD = 255; // Set PWM period
    TPM0->CONTROLS[0].CnV = 0; // Start with red off
    TPM0->CONTROLS[1].CnV = 0; // Start with green off
    TPM0->CONTROLS[2].CnV = 0; // Start with blue off
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
