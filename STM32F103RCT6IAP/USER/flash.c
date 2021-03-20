/*
 * flash.c
 *
 *  Created on: 2017-08-01
 *      Author: intelligent
 */


#include "flash.h"

extern void    FLASH_PageErase(uint32_t PageAddress);
uint16_t STMFLASH_BUF[STM_PAGE_SIZE/2];//每页最多是2K字节

//==========================================================
//读取指定地址的半字
//faddr:地址（ 必须为2的整数倍）
//返回值为要读取的数据
//==========================================================
uint16_t FLASH_ReadHalfWord_One(uint32_t faddr)
{
	return *(__IO uint16_t *)faddr; 
}	

//==========================================================
//读取指定地址的字
//faddr:地址（ 必须为2的整数倍）
//返回值为要读取的数据
//==========================================================
uint32_t FLASH_ReadWord_One(uint32_t faddr)
{
	return *(__IO uint32_t *)faddr; 
}	

//======================================================================
//从指定地址Readaddr开始读取指定长度sNumToRead的半字，存放在pBuffer中
//Readaddr:起始地址（必须为2的整数倍）
//pBuffer:存放数据的指针
//sNumToRead:读取的数据个数
//======================================================================
void FLASH_ReadHalfWord(uint32_t Readaddr,uint16_t *pBuffer,uint16_t sNumToRead)
{
	uint16_t i;
	for(i=0;i<sNumToRead;i++)
	{								
		pBuffer[i]=FLASH_ReadHalfWord_One(Readaddr);   //读取半字即2个字节
		Readaddr+=2;      //读取了2个字节，地址偏移2
	}
}
//======================================================================
//从指定地址Writeaddr开始写入指定长度sNumToRead的半字
//Writeaddr:起始地址（必须为2的整数倍）
//pBuffer:数据指针
//sNumToWrite:写入的数据个数
//======================================================================
void FLASH_WriteHalfWord(uint32_t Writeaddr,uint16_t *pBuffer,uint16_t sNumToWrite)
{
	uint16_t i;
	for(i=0;i<sNumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,Writeaddr,pBuffer[i]);
		Writeaddr+=2;
	}
}
//======================================================================
//从指定地址Writeaddr开始写入长度为sNumToWrite的数据
//Writeaddr:起始地址（必须为2的整数倍）
//pBuffer:数据指针
//sNumToRead:写入的数据个数
//======================================================================
void FLASH_Write(uint32_t Writeaddr,uint16_t *pBuffer,uint16_t sNumToWrite)
{
	uint32_t pageNum;        //页编号（要写的地址在哪一页）
	uint16_t pageOff;     //页偏移空间
	uint16_t pageMain;    //页剩余空间
	
	uint32_t offAddr;         //实际偏移地址（写入地址-flash基地址）
	
	uint16_t i;

	if((Writeaddr<STM32_FLASH_BASE)||(Writeaddr>(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
	{
		return;
	}
	
	HAL_FLASH_Unlock();       //解锁
	offAddr=Writeaddr-STM32_FLASH_BASE;       //实际偏移地址
	pageNum=offAddr/STM_PAGE_SIZE;           //页编号（实际偏移地址除去页的大小，就可以算出在哪一页）
	pageOff=(offAddr%STM_PAGE_SIZE)/2;        //要写的这个地址在这一页的偏移空间（以半字位单位）
	pageMain=STM_PAGE_SIZE/2-pageOff;         //这一页还剩余多少空间（即这一页剩余空间还能写多少半字）
	
	if(sNumToWrite<=pageMain)                 //如果写入的数据在页面范围内
	{
		pageMain=sNumToWrite;
	}

	while(1)
	{
		FLASH_ReadHalfWord(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_PAGE_SIZE/2);  //读出整个页面的数据
		for(i=0;i<pageMain;i++)
		{
			if(STMFLASH_BUF[pageOff+i]!=0xFFFF)
				break;                                             //需要擦除
		}
		if(i<pageMain)
		{
			FLASH_PageErase(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE);     //擦除这一页
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);   //cube库问题，FLASH_PageErase函数在执行完成之后，没有把页擦除寄存器清零
			for(i=0;i<pageMain;i++)
			{
				STMFLASH_BUF[pageOff+i]=pBuffer[i];
			}
			FLASH_WriteHalfWord(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_PAGE_SIZE/2);    //将修改过的数据写入这一页
		}
		else
		{
			FLASH_WriteHalfWord(Writeaddr,pBuffer,pageMain);           //如果flash已经擦过了，那么直接将数据写进去，不用读
		}
		
		if(sNumToWrite==pageMain)
			break;                       //写完了，退出
		else        //如果一页没写完
		{
//			LED_On(LED_CTL_W_Pin);    //测试跨页写flash，如果跨页了，灯会亮
			pageNum++;                //向下一页写
			pageOff=0;                //新的一页偏移量为0
			pBuffer+=pageMain;        //指针偏移
			Writeaddr+=pageMain*2;    //写地址偏移
			sNumToWrite-=pageMain;    //去掉已写数据量
			if(sNumToWrite>STM_PAGE_SIZE/2)     //如果写一页还写不完
			{
				pageMain=STM_PAGE_SIZE/2;         //写满整页
			}
			else
			{
				pageMain=sNumToWrite;      //下一页可以写完
			}
		}
		
	}
	HAL_FLASH_Lock();
	
}
void Flash_Program_Test(u32 addr,u32 date)
{
	HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr,date);
	FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	HAL_FLASH_Lock();
}

void Flash_ErasePage_Test(uint32_t Pageaddr)
{
	HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	FLASH_PageErase(Pageaddr);
	FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);   //cube库问题，FLASH_PageErase函数在执行完成之后，没有把页擦除寄存器清零
	HAL_FLASH_Lock();
}


