/**
  ******************************************************************************
  * @file           : usart.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-06
  * Author			: mankaichuang
  ******************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "main.h"

UART_HandleTypeDef huart2;

uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint8_t USART_TX_BUF[USART_REC_LEN];     //发送缓冲,最大USART_REC_LEN个字节.

//=============================================================================
//接收状态  uart1
//bit15，	接收到0x03/0x0a
//bit14，	接收到0x02/0x0d
//bit13~0，	接收到的有效字节数目
//=============================================================================
uint16_t USART_RX_STA=0;       //USART1接收状态标记	

uint8_t aRxBuffer[1];   //HAL库使用的串口接收缓冲

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0)      //循环发送,直到发送完
	{} 
    USART2->DR = (uint8_t) ch;      
	return ch;
}

//==================================================================
// USART2 init
//==================================================================
void USART2_Init(uint32_t bound)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = bound;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, 1);   //open receive IT

}

void USART_RXbuffer_init(void)
{
	USART_RX_STA=0;
	memset(USART_RX_BUF,0x00,USART_REC_LEN);
}

//==================================================================
// 串口中断回调函数 
//==================================================================

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

//-----------------------------------------------------------------//
//receive flag 0x0d 0x0a
//-----------------------------------------------------------------//
	if(huart->Instance==USART2)//如果是串口2
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)
				{
					USART_RX_STA = USART_RX_STA & 0x3fff ;     //如果下一个数据不是0x0a,说明之前的0x0d是数据，不是标识，所以需要清除之前设的标识
					USART_RX_BUF[USART_RX_STA&0X3FFF] = 0x0d;  //然后将0x0d当作数据储存起来
					USART_RX_STA++ ;                           //数据计数器加1
					USART_RX_BUF[USART_RX_STA&0X3FFF] = aRxBuffer[0];  //将新数据也储存起来
					USART_RX_STA++ ;                           //数据计数器加1
				}
				else
				{
					USART_RX_STA|=0x8000;	//接收完成了
				}
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)
				{
					USART_RX_STA|=0x4000;
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if((USART_RX_STA&0x3FFF)>(USART_REC_LEN-1))
					{
						USART_RX_STA=0;//接收数据错误,重新开始接收	
					}						
				}		 
			}
		}

	}

}



