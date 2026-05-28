#include "NotArduino.h"
#include "MKL25Z4.h"

int readValue(){
    // Configure ADC0 for single-ended conversion on channel 0 (PTE20)
    ADC0->SC1[0] = ADC_SC1_ADCH(0); // Start conversion on channel 0

    // Wait for conversion to complete
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

    return ADC0->R[0]; // Return the converted value
}
