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

uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8_t USART_TX_BUF[USART_REC_LEN];     //���ͻ���,���USART_REC_LEN���ֽ�.

//=============================================================================
//����״̬  uart1
//bit15��	���յ�0x03/0x0a
//bit14��	���յ�0x02/0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//=============================================================================
uint16_t USART_RX_STA=0;       //USART1����״̬���	

uint8_t aRxBuffer[1];   //HAL��ʹ�õĴ��ڽ��ջ���

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0)      //ѭ������,ֱ��������
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
// �����жϻص����� 
//==================================================================

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

//-----------------------------------------------------------------//
//receive flag 0x0d 0x0a
//-----------------------------------------------------------------//
	if(huart->Instance==USART2)//����Ǵ���2
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)
				{
					USART_RX_STA = USART_RX_STA & 0x3fff ;     //�����һ�����ݲ���0x0a,˵��֮ǰ��0x0d�����ݣ����Ǳ�ʶ��������Ҫ���֮ǰ��ı�ʶ
					USART_RX_BUF[USART_RX_STA&0X3FFF] = 0x0d;  //Ȼ��0x0d�������ݴ�������
					USART_RX_STA++ ;                           //���ݼ�������1
					USART_RX_BUF[USART_RX_STA&0X3FFF] = aRxBuffer[0];  //��������Ҳ��������
					USART_RX_STA++ ;                           //���ݼ�������1
				}
				else
				{
					USART_RX_STA|=0x8000;	//���������
				}
			}
			else //��û�յ�0X0D
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
						USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	
					}						
				}		 
			}
		}

	}

}



