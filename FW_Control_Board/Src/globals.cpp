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

void get_current_position(void){
    char tsbuffer[12];
	sprintf(tsbuffer,"NxSETx%d",monochromator.current_position);
	monochromator.uart->print(tsbuffer);
}

void Go_To_Min_Wavelength(void){
    monochromator.motor->dir(DIRECTION_REVERSE);
    while(HAL_GPIO_ReadPin(END_STOP_REVERSE_GPIO_Port,END_STOP_REVERSE_Pin)==0)
          {
              monochromator.motor->enable();
              monochromator.motor->step();
          }
    monochromator.motor->disable();
}

void Go_To_Max_Wavelength(void){
    monochromator.motor->dir(DIRECTION_FORWARD);
    while(HAL_GPIO_ReadPin(END_STOP_FORWARD_GPIO_Port,END_STOP_FORWARD_Pin)==0)
          {
              monochromator.motor->enable();
              monochromator.motor->step();
          }
    monochromator.motor->disable();
}

void autocalibration(void){
	if(HAL_GPIO_ReadPin(END_STOP_FORWARD_GPIO_Port,END_STOP_FORWARD_Pin)==0  && HAL_GPIO_ReadPin(END_STOP_REVERSE_GPIO_Port,END_STOP_REVERSE_Pin)==0){
		Go_To_Min_Wavelength();
        monochromator.current_position=monochromator.Min_Wavelength;
	}
    if(HAL_GPIO_ReadPin(END_STOP_FORWARD_GPIO_Port,END_STOP_FORWARD_Pin)==1  && HAL_GPIO_ReadPin(END_STOP_REVERSE_GPIO_Port,END_STOP_REVERSE_Pin)==0){
		Go_To_Min_Wavelength();
        monochromator.current_position=monochromator.Min_Wavelength;
	}
    if(HAL_GPIO_ReadPin(END_STOP_FORWARD_GPIO_Port,END_STOP_FORWARD_Pin)==0  && HAL_GPIO_ReadPin(END_STOP_REVERSE_GPIO_Port,END_STOP_REVERSE_Pin)==1){
		Go_To_Max_Wavelength();
        monochromator.current_position=monochromator.Max_Wavelength;
	}
    if(HAL_GPIO_ReadPin(END_STOP_FORWARD_GPIO_Port,END_STOP_FORWARD_Pin)==1  && HAL_GPIO_ReadPin(END_STOP_REVERSE_GPIO_Port,END_STOP_REVERSE_Pin)==1){
		monochromator.uart->print("End stop Error");
	}
}
//--
void Go_Up(uint32_t wavelength){
    monochromator.motor->dir(DIRECTION_FORWARD);
    monochromator.motor->execute_steps(wavelength*monochromator.step_divider_settings);
    monochromator.current_position=monochromator.current_position+wavelength;
}
void Go_Down(uint32_t wavelength){
    monochromator.motor->dir(DIRECTION_REVERSE);
    monochromator.motor->execute_steps(wavelength*monochromator.step_divider_settings);
    monochromator.current_position=monochromator.current_position-wavelength;
}
void Go_to_Wavelength(uint32_t go_to_position){
    int32_t position_diference;
	position_diference=go_to_position-monochromator.current_position;
	if(position_diference > 0)
	{
        monochromator.motor->dir(DIRECTION_FORWARD);
        monochromator.motor->execute_steps(go_to_position*monochromator.step_divider_settings);
		monochromator.current_position=go_to_position;
	}
	else if(position_diference < 0)
	{
        monochromator.motor->dir(DIRECTION_REVERSE);
		monochromator.motor->execute_steps((monochromator.current_position-go_to_position)*monochromator.step_divider_settings);
		monochromator.current_position=go_to_position;
	}
}

void buffer_analyze(char buffer[12]){
	uint8_t i;
	char  cmd[5];
	char end_of_packet[6];
	uint32_t end_of_packet_numeric;
    if(buffer[0]=='x')
    {
        for(i=1;i<=5;i++)
        {
            cmd[i-1]=buffer[i];
        }
        for(i=0;i<=5;i++)
        {
            end_of_packet[i]=buffer[i+6];
        }
/*        monochromator.uart->print(cmd);
        monochromator.uart->print("\n");
        monochromator.uart->print(buffer);
        monochromator.uart->print("\n");
        monochromator.uart->print(end_of_packet);
        monochromator.uart->print("\n");*/
        end_of_packet_numeric=atol(end_of_packet);
        if(strncmp(cmd,"aucal",5)==0){autocalibration();}
        else if(strncmp(cmd,"getpo",5)==0){get_current_position();}
        else if(strncmp(cmd,"go_up",5)==0){Go_Up(end_of_packet_numeric);}
        else if(strncmp(cmd,"go_dw",5)==0){Go_Down(end_of_packet_numeric);}
        else if(strncmp(cmd,"go_to",5)==0){Go_to_Wavelength(end_of_packet_numeric);}
        else{
            monochromator.uart->print("\r\nCommand not recognized\r\n");
        }
    }
    else monochromator.uart->print("\r\nTransmission error\r\n");
}
