#include "motor.h"

void motor_basic_delay(void)
{
    uint16_t cnt;
    for (cnt = 0; cnt < 259; cnt++)
        ;
}

void motor_step_pulse(void)
{
    motor_step_on();
    motor_basic_delay();
    motor_step_off();
    motor_basic_delay();
}

void motor_steps(uint32_t number_steps)
{
    uint32_t cnt;
    for (cnt = 0; cnt < number_steps; cnt++)
        motor_step_pulse();
}
