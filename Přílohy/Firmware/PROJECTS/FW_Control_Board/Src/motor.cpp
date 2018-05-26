#include "motor.hpp"

#define MOTOR_DELAY_OPTIMAL           45
#define MOTOR_DELAY_MINIMAL           30
#define MOTOR_DELAY_MAXIMAL           50
#define MOTOR_ACCELERATION_SPEED_LOOP 1000

Motor::Motor(float step_angle, uint8_t step_divider)
{
    step_pin(false);
    step_pin(DIRECTION_FORWARD);
    this->step_angle         = step_angle;
    this->step_divider       = step_divider;
    this->delay_basic_number = MOTOR_DELAY_OPTIMAL;
    mode(step_divider);
    disable();
}

void Motor::step(void)
{
    step_pin(true);
    delay();
    step_pin(false);
    delay();
}

void Motor::dir(motor_directon_t direction)
{
    this->direction = direction;
    if (direction == DIRECTION_FORWARD)
        dir_pin(true);
    else
        dir_pin(false);
}

void Motor::enable(void)
{
    ena_pin(false);
}

void Motor::disable(void)
{
    ena_pin(true);
}

void Motor::mode(uint8_t m)
{
    switch (m)
    {
        case 32:
            mode_select_pin2(true);
            mode_select_pin1(true);
            mode_select_pin0(true);
            delay_magic_number = delay_basic_number;
            break;
        case 16:
            mode_select_pin2(true);
            mode_select_pin1(false);
            mode_select_pin0(false);
            delay_magic_number = delay_basic_number * 2;
            break;
        case 8:
            mode_select_pin2(false);
            mode_select_pin1(true);
            mode_select_pin0(true);
            delay_magic_number = delay_basic_number * 4;
            break;
        case 4:
            mode_select_pin2(false);
            mode_select_pin1(true);
            mode_select_pin0(false);
            delay_magic_number = delay_basic_number * 8;
            break;
        case 2:
            mode_select_pin2(false);
            mode_select_pin1(false);
            mode_select_pin0(true);
            delay_magic_number = delay_basic_number * 16;
            break;
        default:
            mode_select_pin2(false);
            mode_select_pin1(false);
            mode_select_pin0(false);
            delay_magic_number = delay_basic_number * 32;
            m = 1;
            break;
    }
    this->step_divider = m;
    calculate_number_full_rotaiton();
} // mode

void Motor::acceleration(void)
{
    for (; delay_basic_number > MOTOR_DELAY_MINIMAL; delay_basic_number--)
    {
        mode(step_divider);
        for (uint16_t i = 0; i < MOTOR_ACCELERATION_SPEED_LOOP; i++)
            step();
    }
}

void Motor::deacceleration(void)
{
    for (; delay_basic_number < MOTOR_DELAY_MAXIMAL; delay_basic_number++)
    {
        mode(step_divider);
        for (uint16_t i = 0; i < MOTOR_ACCELERATION_SPEED_LOOP; i++)
            step();
    }
}

void Motor::execute_steps(uint32_t steps)
{
    enable();
    for (; steps; steps--)
        step();
    disable();
}
