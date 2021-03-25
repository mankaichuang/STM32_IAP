/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-06
  * Author			: mankaichuang
  ******************************************************************************
**/


#ifndef __USART_H
#define __USART_H

#include "sys.h"

//#define USART_REC_LEN    4096

extern UART_HandleTypeDef huart2;

extern uint8_t  USART_RX_BUF[4096];
extern uint16_t USART_RX_STA; 
extern uint32_t USART_RX_COUNT;

extern uint8_t aRxBuffer[1];//HAL库使用的串口接收缓冲


void USART2_Init(uint32_t bound);
void USART_RXbuffer_init(void);
#endif
