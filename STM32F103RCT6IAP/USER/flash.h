#ifndef __FLASH_H
#define __FLASH_H

#include "stdint.h"
#include "sys.h"

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

#define STM32_FLASH_SIZE 256

#if STM32_FLASH_SIZE<256
#define STM_PAGE_SIZE 1024 //字节
#else 
#define STM_PAGE_SIZE	2048
#endif	
 
uint16_t FLASH_ReadHalfWord_One(uint32_t faddr);   //读出半字
uint32_t FLASH_ReadWord_One(uint32_t faddr);	  	//读出字  
void FLASH_ReadHalfWord(uint32_t Readaddr,uint16_t *pBuffer,uint16_t sNumToRead);
void FLASH_WriteHalfWord(uint32_t Writeaddr,uint16_t *pBuffer,uint16_t sNumToWrite);
void FLASH_Write(uint32_t Writeaddr,uint16_t *pBuffer,uint16_t sNumToWrite);

void Flash_Program_Test(u32 addr,u32 date);
void Flash_ErasePage_Test(uint32_t Pageaddr);
void FLASH_ReadHalfWord(uint32_t Readaddr,uint16_t *pBuffer,uint16_t sNumToRead);

#endif
