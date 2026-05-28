#pragma once
#include "MKL25Z4.h"

void Serial_begin(uint32_t baudrate);
void Serial_write(uint8_t data);
void Serial_print(const char *s);
void Serial_println(const char *s);
void Serial_printInt(int32_t value);
void Serial_printUInt(uint32_t value);
void initComunication();
