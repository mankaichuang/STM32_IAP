/**
  ******************************************************************************
  * @file           : keyboard.h
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "sys.h"

#define KEY_PB8_Pin GPIO_PIN_8
#define KEY_PB8_GPIO_Port GPIOB
#define KEY_PB9_Pin GPIO_PIN_9
#define KEY_PB9_GPIO_Port GPIOB
#define KEY_PB10_Pin GPIO_PIN_10
#define KEY_PB10_GPIO_Port GPIOB
#define KEY_PB11_Pin GPIO_PIN_11
#define KEY_PB11_GPIO_Port GPIOB
#define KEY_PB12_Pin GPIO_PIN_12
#define KEY_PB12_GPIO_Port GPIOB
#define KEY_PB13_Pin GPIO_PIN_13
#define KEY_PB13_GPIO_Port GPIOB
#define KEY_PB14_Pin GPIO_PIN_14
#define KEY_PB14_GPIO_Port GPIOB
#define KEY_PB15_Pin GPIO_PIN_15
#define KEY_PB15_GPIO_Port GPIOB

typedef enum
{ 
  KEY_IN_OUT = 0,
  KEY_OUT_IN
}KEY_PINState;

#define KB8_State HAL_GPIO_ReadPin(GPIOB,KEY_PB8_Pin)
#define KB9_State HAL_GPIO_ReadPin(GPIOB,KEY_PB9_Pin)
#define KB10_State HAL_GPIO_ReadPin(GPIOB,KEY_PB10_Pin)
#define KB11_State HAL_GPIO_ReadPin(GPIOB,KEY_PB11_Pin)
#define KB12_State HAL_GPIO_ReadPin(GPIOB,KEY_PB12_Pin)
#define KB13_State HAL_GPIO_ReadPin(GPIOB,KEY_PB13_Pin)
#define KB14_State HAL_GPIO_ReadPin(GPIOB,KEY_PB14_Pin)
#define KB15_State HAL_GPIO_ReadPin(GPIOB,KEY_PB15_Pin)

void KeyBoard_IO_Init(KEY_PINState KEYState);
uint8_t ReadKey(void);
uint8_t ReadKey_R(void);
uint8_t ReadKey_Over(uint8_t ucOver);
//void Key_Enter_Oled(int x,int y);
//void Key_Oled_Display(int x,int y);


#endif
