#pragma once
#include "MKL25Z4.h"

typedef enum
{
    INPUT = 0,
    OUTPUT = 1
} NotArduinoPinMode;

typedef enum
{
    OFF = 0,
    ON = 1
} NotArduinoSwitch;

typedef enum
{
    LOW = 0,
    HIGH = 1
} NotArduinoDigitalValue;

typedef enum
{
    // Port A: 0-31
    PA0 = 0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15, PA16,
    PA17, PA18, PA19, PA20, PA21, PA22, PA23, PA24, PA25, PA26, PA27, PA28, PA29, PA30, PA31,
    // Port B: 32-63
    PB0 = 32, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15, PB16,
    PB17, PB18, PB19, PB20, PB21, PB22, PB23, PB24, PB25, PB26, PB27, PB28, PB29, PB30, PB31,
    // Port C: 64-95
    PC0 = 64, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15, PC16,
    PC17, PC18, PC19, PC20, PC21, PC22, PC23, PC24, PC25, PC26, PC27, PC28, PC29, PC30, PC31,
    // Port D: 96-127
    PD0 = 96, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15, PD16,
    PD17, PD18, PD19, PD20, PD21, PD22, PD23, PD24, PD25, PD26, PD27, PD28, PD29, PD30, PD31,
    // Port E: 128-159
    PE0 = 128, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15, PE16,
    PE17, PE18, PE19, PE20, PE21, PE22, PE23, PE24, PE25, PE26, PE27, PE28, PE29, PE30, PE31
} NotArduinoPin;

/**
 * @brief Configure a pin as INPUT or OUTPUT
 * @param pin The pin to configure
 * @param mode The mode to set the pin to (INPUT or OUTPUT)
 */
void pinMode(NotArduinoPin pin, NotArduinoPinMode mode);

/**
 * @brief Write a digital value (HIGH or LOW) to a pin
 * @param pin The pin to write to
 * @param digitalValue The digital value to write (HIGH or LOW)
 */
void digitalWrite(NotArduinoPin pin, NotArduinoDigitalValue digitalValue);

/**
 * @brief Read a digital value (HIGH or LOW) from a pin
 * @param pin The pin to read from
 * @return The digital value read (HIGH or LOW)
 */
NotArduinoDigitalValue digitalRead(NotArduinoPin pin);

/**
 * @brief Write an analog value (0-255) to a pin using PWM. Doesn't need pinMode() beforehand, but pin must support TPM functionality.
 * @param pin The pin to write to (set automatically to the correct MUX mode for TPM)
 * @param value The analog value to write (0-255)
 */
void analogWrite(NotArduinoPin pin, uint8_t value);

/**
 * @brief Map a value from one range to another
 * @param x The value to map
 * @param in_min The minimum value of the input range
 * @param in_max The maximum value of the input range
 * @param out_min The minimum value of the output range
 * @param out_max The maximum value of the output range
 * @return The mapped value
 */
int map(int x, int in_min, int in_max, int out_min, int out_max);

/**
 * @brief Active delay helper function. This actively feeds (refreshes) the Watchdog Timer during the wait period.
 * @param ms Delay time in milliseconds.
 */
void delay_ms(uint32_t ms); 