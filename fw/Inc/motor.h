/*
 * Library for step stick motor
 */

#ifndef __MOTOR_INCLUDED__
#define __MOTOR_INCLUDED__

#include "stm32f3xx_hal.h"


#define motor_step_on()     HAL_GPIO_WritePin(MOTOR_STEP_GPIO_Port, MOTOR_STEP_Pin, GPIO_PIN_SET)
#define motor_step_off()    HAL_GPIO_WritePin(MOTOR_STEP_GPIO_Port, MOTOR_STEP_Pin, GPIO_PIN_RESET)

#define motor_dir_front()   HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET)
#define motor_dir_reverse() HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET)


void motor_basic_delay(void);
void motor_step_pulse(void);
void motor_steps(uint32_t number_steps);

#endif // __MOTOR_INCLUDED__
