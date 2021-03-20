/**
  ******************************************************************************
  * @file           : updata.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-18
  * Author			: mankaichuang
  ******************************************************************************
**/

#include <stdio.h>
#include "updata.h"
#include "usart.h"
#include "flash.h"

pFunction JumpToApplication;
uint32_t JumpAddress;

uint8_t updataPackage = 0;
uint8_t updatabuf[USART_REC_LEN/2];
uint16_t u16updatabuf[USART_REC_LEN/4];

void sendRequest(uint8_t cmd)
{
	uint8_t CMD_REQUEST_DATA[3];
	CMD_REQUEST_DATA[0] = cmd;
	CMD_REQUEST_DATA[1] = 0x0d;
	CMD_REQUEST_DATA[2] = 0x0a;
	uint8_t i = 0;
	for(i = 0; i < 3; i++)
	{
		printf("%c",CMD_REQUEST_DATA[i]);
	}
}

void updataFlash(void)
{
	uint16_t tempbuf;
	uint16_t i = 0,j = 0;
	uint8_t *buf;
	uint32_t faddr = APPLICATION_ADDRESS;
	while(updataPackage)
	{
		if(USART_RX_STA & 0x8000)
		{
			updataPackage--;
			ASC2BCD(USART_RX_BUF,updatabuf,USART_RX_COUNT);
			buf = updatabuf;
			for(i = 0; i<USART_RX_COUNT/2; i+=2)
			{
				tempbuf = (uint16_t)buf[1]<<8;
				tempbuf += (uint16_t)buf[0];
				buf+=2;
				u16updatabuf[j] = tempbuf;
				j++;
			}
			FLASH_Write(faddr,u16updatabuf,USART_RX_COUNT/4);
			faddr += 2048;
			sendRequest(UPDATA_REDAY);
			USART_RXbuffer_init();
		}
	}
		if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
		{
			JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4); //APP address
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
			JumpToApplication();	
		}
}

//==================================================================
// asc2bcd
//==================================================================
void ASC2BCD(uint8_t *ucASCBuf,uint8_t *ucBCDBuf,uint16_t Len)
{
	int i;
	uint8_t temp;
	
	for(i=0;i<Len;i+=2)
	{
		temp=ucASCBuf[i];
		if(temp>'9')
		{
			temp=(temp-'A')+0x0A;
		}
		else
		{
			temp=temp&0x0F;
		}
		
		ucBCDBuf[i/2]=temp<<4;
		
		temp=ucASCBuf[i+1];
		if(temp>'9')
		{
			temp=(temp-'A')+0x0A;
		}
		else
		{
			temp=temp&0x0F;
		}
		ucBCDBuf[i/2]|=temp;
	}
}