#ifndef __UART_INCLUDED__
#define __UART_INCLUDED__

#include "stm32f0xx_hal.h"

#define RX_BUFFER_SIZE 12

class Uart
{
private:
    UART_HandleTypeDef *uart;
    bool rx_flag;
public:
    uint8_t rx_buffer[RX_BUFFER_SIZE];

    Uart(UART_HandleTypeDef *uart);

    inline void send_byte(uint8_t byte)
    {
        HAL_UART_Transmit(uart, &byte, 1, 0xFF);
    }

    inline void receive_it(void)
    {
        HAL_UART_Receive_IT(uart, rx_buffer, RX_BUFFER_SIZE);
    }

    void set_rx_flag(void);
    void clear_rx_flag(void);
    bool get_rx_flag(void);
    void print(char str[]);
};

#endif // ifndef __UART_INCLUDED__
