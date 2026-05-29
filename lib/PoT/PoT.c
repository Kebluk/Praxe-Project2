#include "NotArduino.h"
#include "MKL25Z4.h"

int readValue(){
    // Safety Check: Enable clock gate for ADC0 (Bit 27 of SIM_SCGC6)
    // Without this, writing to ADC0 registers will cause a HardFault.
    if (!(SIM->SCGC6 & SIM_SCGC6_ADC0_MASK)) {
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
        
        // Optional: Configure ADC0 for 10-bit single-ended mode, bus clock / 2
        // Default register values work, but setting CFG1 is highly recommended
        ADC0->CFG1 = ADC_CFG1_ADIV(1) | ADC_CFG1_ADICLK(1) | ADC_CFG1_MODE(1);
    }

    // Configure ADC0 for single-ended conversion on channel 0 (PTE20 / ADC0_DP0/ADC0_SE0)
    ADC0->SC1[0] = ADC_SC1_ADCH(0); // Start conversion on channel 0

    // Wait for conversion to complete
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

    return ADC0->R[0]; // Return the converted value
}