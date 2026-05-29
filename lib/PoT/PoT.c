#include "NotArduino.h"
#include "MKL25Z4.h"

static uint8_t adc_initialized = 0;

// Simple busy-wait delay function
static void delay_us(unsigned int us) {
    unsigned int ticks = us * 24;  // Approximately 1 microsecond at 24 MHz
    while (ticks--);
}

// Helper function to read a specific ADC channel
int readADCChannel(uint8_t channel) {
    // Initialize ADC and pins only once
    if (!adc_initialized) {
        // Enable clock gates for ports
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;

        // Configure pins for analog input (MUX = 0)
        PORTC->PCR[2] = PORT_PCR_MUX(0);  // PTC2 for channel 11
        PORTB->PCR[2] = PORT_PCR_MUX(0);  // PTB2 for channel 12

        // Enable ADC0 clock
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
        
        // Allow time for ADC to initialize
        delay_us(10);
        
        // Configure ADC0: 10-bit resolution, bus clock (ADICLK=0), ADIV=1 (divide by 2)
        ADC0->CFG1 = ADC_CFG1_ADIV(1) | ADC_CFG1_ADICLK(0) | ADC_CFG1_MODE(1);
        
        // Configure ADC0: short sample time for stable reads
        ADC0->CFG2 = ADC_CFG2_ADLSTS(0);  // Short sample time
        
        // Set SC2: use VREFH/VREFL as reference with full range
        ADC0->SC2 = 0;  // Default VREF, no continuous conversion trigger
        
        adc_initialized = 1;
    }

    // Configure ADC0 for single-ended conversion on the selected channel
    ADC0->SC1[0] = ADC_SC1_ADCH(channel);

    // Wait for conversion to complete
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

    int result = ADC0->R[0];  // Return 10-bit result (0-1023)
    
    // Small delay to stabilize before next read
    delay_us(20);
    
    return result;
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