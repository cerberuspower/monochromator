#ifndef __GLOBALS_INCLUDED__
#define __GLOBALS_INCLUDED__

#include "motor.hpp"
#include "uart.hpp"
#include <string.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"

typedef struct monochromator_t {
    Motor *motor;
    Uart *uart;
    uint16_t end_stop_forward;
    uint16_t end_stop_reverse;
    uint8_t aBuffer[RX_BUFFER_SIZE];
    uint32_t current_position;
    uint8_t step_divider_settings;
    uint32_t Max_Wavelength;
    uint32_t Min_Wavelength;


} monochromator_t;

void buffer_analyze(char buffer[12]);
extern monochromator_t monochromator;

#endif // ifndef __GLOBALS_INCLUDED__
