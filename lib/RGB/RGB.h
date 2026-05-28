#pragma once
#include "MKL25Z4.h"

typedef enum {
    RGB_RED = 0,
    RGB_GREEN = 1,
    RGB_BLUE = 2
} RGBColor;

void RGB_init(void);
void RGB_Set(NotArduinoSwich red, NotArduinoSwich green, NotArduinoSwich blue);
void RGB_SetBrightness(RGBColor color, uint8_t value);