#ifndef __MOTOR_INCLUDED__
#define __MOTOR_INCLUDED__

#include "stm32f3xx_hal.h"

typedef enum {
    DIRECTION_FORWARD,
    DIRECTION_REVERSE,
} motor_directon_t;

class Motor
{
private:
    float step_angle;
    uint8_t step_divider;
    uint16_t number_of_step_for_rotatoin;
    uint32_t delay_basic_number;
    uint32_t delay_magic_number;
    motor_directon_t direction;

    inline void delay(void) const
    {
        for (uint16_t i = 0; i < delay_magic_number; i++)
            asm volatile ("mov r0, r0");
    }

    inline GPIO_PinState bool_to_gpio(bool level)
    {
        return (level) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }

    inline void step_pin(bool level)
    {
        HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, bool_to_gpio(level));
    }

    inline void mode_select_pin0(bool level)
    {
        HAL_GPIO_WritePin(MS0_GPIO_Port, MS0_Pin, bool_to_gpio(level));
    }

    inline void mode_select_pin1(bool level)
    {
        HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, bool_to_gpio(level));
    }

    inline void mode_select_pin2(bool level)
    {
        HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, bool_to_gpio(level));
    }

    inline void ena_pin(bool level)
    {
        HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, bool_to_gpio(level));
    }

    inline void dir_pin(bool level)
    {
        HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, bool_to_gpio(level));
    }

    inline void calculate_number_full_rotaiton(void)
    {
        this->number_of_step_for_rotatoin = (360.0 / step_angle) * step_divider;
    }

public:
    Motor(float step_angle, uint8_t step_divider);
    void step(void);
    void dir(motor_directon_t direction);
    void mode(uint8_t m);
    void enable(void);
    void disable(void);
    void acceleration(void);
    void deacceleration(void);
    void execute_steps(uint32_t steps);

    inline void full_rotation(void)
    {
        for (uint16_t i = 0; i < number_of_step_for_rotatoin; i++)
        {
            step();
        }
    }

    inline void set_delay_basic_to_default()
    {
        delay_basic_number = 45;
    }

    inline void set_delay_basic(uint32_t value)
    {
        delay_basic_number = value;
    }

    inline motor_directon_t get_direction(void)
    {
        return direction;
    }
};

#endif // ifndef __MOTOR_INCLUDED__
