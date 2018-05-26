#include "globals.hpp"

monochromator_t monochromator;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /*if (GPIO_Pin == monochromator.end_stop_forward)
     * {
     *  monochromator.motor->dir(DIRECTION_REVERSE);
     * }
     *
     * else
     * {
     *  monochromator.motor->dir(DIRECTION_FORWARD);
     * }*/
    monochromator.motor->disable();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    monochromator.uart->receive_it();
    if (monochromator.uart->rx_buffer[0] != '\n' && monochromator.uart->rx_buffer[0] != '\r')
    {
        monochromator.uart->set_rx_flag();
    }
}
