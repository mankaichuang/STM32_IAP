/**
  ******************************************************************************
  * @file           : beep.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#include "beep.h"
#include "delay.h"


void BEEP_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = BEEP_CTL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BEEP_CTL_GPIO_Port, &GPIO_InitStruct);
}

void BEEP(int time_ms)
{
	 	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_SET);
	 	Delay_ms(time_ms);
	 	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_RESET);
}

void Beep_Alarm(void)
{
		HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_SET);
	 	Delay_ms(200);
	 	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_RESET);
}

void BEEP_P(int t,int f)
{
	int i=1;
	while(i<t)
	{

	 	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_RESET);
	 	Delay_us(f);
	 	HAL_GPIO_WritePin(BEEP_CTL_GPIO_Port, BEEP_CTL_Pin, GPIO_PIN_SET);
	 	Delay_us(f);
	 	i++;
	}

}
