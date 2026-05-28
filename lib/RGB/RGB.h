#pragma once
#include "MKL25Z4.h"

typedef enum
{
    RGB_RED = 0,
    RGB_GREEN = 1,
    RGB_BLUE = 2
} RGBColor;

/** @brief Initialize the RGB LED */
void RGB_init(void);

/**
 * @brief Set the state of the RGB LED
 * @param state The state to set the RGB LED to
 */
void RGB_Set(NotArduinoSwich state);

/** @brief Set the state of a specific color channel
 * @param color The color channel to set
 * @param state The state to set the color channel to
 */
void RGB_SetColor(RGBColor color, NotArduinoSwich state);

/** @brief Set the analog values for each color channel
 * @param red The brightness of the red channel (0-255)
 * @param green The brightness of the green channel (0-255)
 * @param blue The brightness of the blue channel (0-255)
 */
void RGB_AnalogWrite(uint8_t red, uint8_t green, uint8_t blue);

/** @brief Set the brightness of a specific color channel
 * @param color The color channel to set the brightness for
 * @param value The brightness value (0-255)
 */
void RGB_SetColorBrightness(RGBColor color, uint8_t value);