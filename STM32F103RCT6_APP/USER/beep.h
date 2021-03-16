/**
  ******************************************************************************
  * @file           : beep.h
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#ifndef BEEP_H_
#define BEEP_H_

#include "sys.h"

#define BEEP_CTL_Pin GPIO_PIN_1
#define BEEP_CTL_GPIO_Port GPIOA


void BEEP_Init(void);
void BEEP(int time_ms);

void BEEP_P(int t,int f);

void Beep_Alarm(void);

#endif /* BEEP_H_ */
