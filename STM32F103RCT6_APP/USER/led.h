#ifndef __LED_H
#define __LED_H	 

#include "sys.h"

#define LED_CTL_W_Pin GPIO_PIN_0
#define LED_CTL_W_GPIO_Port GPIOB
#define LED_CTL_G_Pin GPIO_PIN_1
#define LED_CTL_G_GPIO_Port GPIOB

void LED_Init(void);
void LED_On(uint16_t GPIO_Pin);
void LED_Off(uint16_t GPIO_Pin);

		 				    
#endif
