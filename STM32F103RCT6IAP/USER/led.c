/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	HAL_GPIO_WritePin(GPIOB, LED_CTL_G_Pin|LED_CTL_W_Pin, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = LED_CTL_G_Pin|LED_CTL_W_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

void LED_On(uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin, GPIO_PIN_SET);
}
