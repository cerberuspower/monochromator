#include "uart.hpp"

Uart::Uart(UART_HandleTypeDef *uart)
{
    this->uart = uart;
    clear_rx_flag();
    receive_it();
}

void Uart::print(char str[])
{
    for (uint8_t i=0; str[i]; i++)
        send_byte(str[i]);
}

void Uart::set_rx_flag(void)
{
    rx_flag = true;
}

void Uart::clear_rx_flag(void)
{
    rx_flag = false;
}

bool Uart::get_rx_flag(void)
{
    return rx_flag;
}
