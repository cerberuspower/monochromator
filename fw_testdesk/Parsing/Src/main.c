/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include <stdlib.h>
#include <stdbool.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t txBuffer[]="Buffer TEST\n\r";
uint8_t rxBuffer[12];
char  aBuffer[12];
bool rx_flag=false;
bool calibration_flag=false;
uint16_t nanometer;
uint16_t tenth_of_nanometer;
uint16_t hundredth_of_nanometer;
uint16_t thousandth_of_nanometer;
uint16_t MAX_WVL=999;
uint16_t MIN_WVL=100;
uint32_t current_position;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
char *ltoa(long N, char *str, int base);
bool buffer_analyze(bool flag,char buffer[10]);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit_IT(&huart2,(uint8_t *)txBuffer,sizeof(txBuffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_UART_Transmit_IT(&huart2,(uint8_t *)txBuffer,sizeof(txBuffer));
    HAL_UART_Receive_IT(&huart2,rxBuffer,12);
		HAL_Delay(1000);
		rx_flag=buffer_analyze(rx_flag,aBuffer);



  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MS0_Pin|MS1_Pin|MS3_Pin|STEP_Pin 
                          |DIR_Pin|ENA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MS0_Pin MS1_Pin MS3_Pin STEP_Pin 
                           DIR_Pin ENA_Pin */
  GPIO_InitStruct.Pin = MS0_Pin|MS1_Pin|MS3_Pin|STEP_Pin 
                          |DIR_Pin|ENA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : END_STOP2_Pin END_STOP1_Pin */
  GPIO_InitStruct.Pin = END_STOP2_Pin|END_STOP1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP ;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//-----------STEP FUNCTION-------------------
void step(void){
	uint32_t i;
	HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
	HAL_Delay(10);
	for (i=0; i<259; i++);

}

//
void GoToMinWVL(){
	uint32_t i;
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==1){
		step();
			}
}
//-----------------------------
uint32_t GoToMaxWVLandCount(void){
	uint32_t STEP_COUNT;
	uint32_t i;
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(END_STOP2_GPIO_Port,END_STOP2_Pin)==1){
		step();
		STEP_COUNT++;
		}
	return STEP_COUNT;	
}
//--------------------------------
uint32_t GoToMinWVLandCount(void){
	uint32_t STEP_COUNT;
	uint32_t i;
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,GPIO_PIN_RESET);
	while(HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==1){
		step();
		STEP_COUNT++;
		}
	return STEP_COUNT;	
}
//---------------------------------------------
void calculate(uint32_t number_of_steps){
char sbuffer[6];
nanometer=number_of_steps/(MIN_WVL-MAX_WVL);
tenth_of_nanometer=nanometer/10;
hundredth_of_nanometer=tenth_of_nanometer/10;
thousandth_of_nanometer=hundredth_of_nanometer/10;
sprintf(sbuffer,"%d",nanometer);
HAL_UART_Transmit(&huart2,sbuffer, sizeof(sbuffer),1000);
}
//-------------------------------------------
void autocalibration(void){
	uint32_t i; uint32_t number_of_steps;	
	if(HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==1  && HAL_GPIO_ReadPin(END_STOP2_GPIO_Port,END_STOP2_Pin)==1){
		GoToMinWVL();
		number_of_steps=GoToMaxWVLandCount();
	  calculate(number_of_steps);
		current_position=MAX_WVL*1000;
	}
	else if (HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==1  && HAL_GPIO_ReadPin(END_STOP2_GPIO_Port,END_STOP2_Pin)==0)
	{
		number_of_steps=GoToMinWVLandCount();
		calculate(number_of_steps);
		current_position=MIN_WVL*1000;
	}
	else if (HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==0  && HAL_GPIO_ReadPin(END_STOP2_GPIO_Port,END_STOP2_Pin)==1)
	{
		number_of_steps=GoToMaxWVLandCount();
	  calculate(number_of_steps);
		current_position=MAX_WVL*1000;
	}
	else if (HAL_GPIO_ReadPin(END_STOP1_GPIO_Port,END_STOP1_Pin)==0  && HAL_GPIO_ReadPin(END_STOP2_GPIO_Port,END_STOP2_Pin)==0)
	{
		HAL_UART_Transmit(&huart2,"\r\nError: END STOP FAILURE\r\n",sizeof("\r\nError: END STOP FAILURE\r\n"),100);
	}
	
}
//-----------------------------------

//-----------------------------------
void steps(uint32_t no_of_steps, bool dir){
	HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,GPIO_PIN_RESET);
	if(dir==true){
		HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_RESET);
		while(no_of_steps!=0)
		{
			step();
			no_of_steps--;
		}
	}
	else if(dir==false){
		HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
		while(no_of_steps!=0)
		{
			step();
			no_of_steps--;
		}
	}	
	HAL_GPIO_WritePin(ENA_GPIO_Port,ENA_Pin,GPIO_PIN_SET);
	
}
//-------------------------------------------
void go_to_wavelength(uint32_t go_to_position)
	{
	int32_t position_diference;
	position_diference=go_to_position-current_position;
	if(position_diference > 0)
	{
		steps(position_diference*thousandth_of_nanometer,true);
		current_position=go_to_position;
	}
	else if(position_diference < 0)
	{
		steps((current_position-go_to_position)*thousandth_of_nanometer,false);
		current_position=go_to_position;
	}
	else if(position_diference==0);
}
//----------------------GO_UP--------------------------
void go_up(uint32_t position_modifier){
	steps(position_modifier*thousandth_of_nanometer,true);
}

//---------------------Go_DOWN-------------------------
void go_down(uint32_t position_modifier){
	steps(position_modifier*thousandth_of_nanometer,true);
}

//-------------------GET Position----------------------
void get_current_position(void){
	sprintf(txBuffer,"%d",current_position);
	HAL_UART_Transmit(&huart2,(uint8_t *)txBuffer,sizeof(txBuffer),1000);
}
//------------------Buffer analyze---------------------
bool buffer_analyze(bool flag,char buffer[12]){
	uint8_t i;
	char  cmd[5];
	char  nm_1[3];
	char end_of_packet[4];
	char c[2]="pe";
	char sbuffer[6];
	if(flag==true)
	{
		if(strcmp(buffer[0],'x')==0)
		{
			for(i=1;i<=5;i++)
			{
				cmd[i-1]=buffer[i];
			}
			for(i=0;i<=5;i++)
			{
				end_of_packet[i]=buffer[i+6];			
			}
			HAL_UART_Transmit(&huart2,cmd, sizeof(cmd),1000);
			if(strcmp(cmd,"aucal")==0) autocalibration();
			else if(strcmp(cmd,"getpo")==0)get_current_position();
			else if(strcmp(cmd,"go_up")==0)go_up(atol(end_of_packet));
			else if(strcmp(cmd,"go_dw")==0)go_down(atol(end_of_packet));
			else if(strcmp(cmd,"go_to")==0)//go_to_wavelength(atol(end_of_packet));
			{	
				go_to_wavelength(atol(end_of_packet));
				sprintf(sbuffer,"%d",atol(nm_1));
				HAL_UART_Transmit(&huart2,sbuffer, sizeof(sbuffer),1000);
				HAL_UART_Transmit(&huart2,nm_1, sizeof(nm_1),1000);
			}
			else{
				sprintf(txBuffer,"\r\nCommand not recognized\r\n");
				HAL_UART_Transmit_IT(&huart2,(uint8_t *)txBuffer,sizeof(txBuffer));
			}
		}
		else HAL_UART_Transmit_IT(&huart2,"\r\nError: Transmision error\r\n",28);
	return false;
	}
}

/**
  * @brief Rx Transfer completed callbacks
  * @param huart: uart handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
	memcpy(aBuffer,rxBuffer,sizeof(rxBuffer));
	rx_flag=true;
    HAL_UART_Transmit(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer),0xFFFF);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
