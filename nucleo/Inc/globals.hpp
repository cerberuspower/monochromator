#ifndef __GLOBALS_INCLUDED__
#define __GLOBALS_INCLUDED__

#include "motor.hpp"
#include "uart.hpp"

typedef struct monochromator_t {
    Motor *motor;
    Uart *uart;
    uint16_t end_stop_forward;
    uint16_t end_stop_reverse;
} monochromator_t;

extern monochromator_t monochromator;

#endif // ifndef __GLOBALS_INCLUDED__
