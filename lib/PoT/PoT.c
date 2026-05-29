#include "NotArduino.h"
#include "MKL25Z4.h"

// Helper function to read a specific ADC channel
int readADCChannel(uint8_t channel) {
    // Safety Check: Enable clock gate for ADC0 (Bit 27 of SIM_SCGC6)
    // Without this, writing to ADC0 registers will cause a HardFault.
    if (!(SIM->SCGC6 & SIM_SCGC6_ADC0_MASK)) {
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
        
        // Configure ADC0 for 10-bit single-ended mode, bus clock / 2
        ADC0->CFG1 = ADC_CFG1_ADIV(1) | ADC_CFG1_ADICLK(1) | ADC_CFG1_MODE(1);
    }

    // Configure ADC0 for single-ended conversion on the selected channel
    ADC0->SC1[0] = ADC_SC1_ADCH(channel); 

    // Wait for conversion to complete
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

    return ADC0->R[0]; // Return the converted value
}

// Read Potentiometer 1 (PTC2 / ADC0_SE11 -> Channel 11)
int readPot1() {
    return readADCChannel(11);
}

// Read Potentiometer 2 (PTB2 / ADC0_SE12 -> Channel 12)
int readPot2() {
    return readADCChannel(12);
}

// Backward compatible single read value (defaults to Pot 1)
int readValue() {
    return readPot1();
}