/*
 * flash.c
 *
 *  Created on: 2017-08-01
 *      Author: intelligent
 */


#include "flash.h"

extern void    FLASH_PageErase(uint32_t PageAddress);
uint16_t STMFLASH_BUF[STM_PAGE_SIZE/2];//ÿҳ�����2K�ֽ�

//==========================================================
//��ȡָ����ַ�İ���
//faddr:��ַ�� ����Ϊ2����������
//����ֵΪҪ��ȡ������
//==========================================================
uint16_t FLASH_ReadHalfWord_One(uint32_t faddr)
{
	return *(__IO uint16_t *)faddr; 
}	

//==========================================================
//��ȡָ����ַ����
//faddr:��ַ�� ����Ϊ2����������
//����ֵΪҪ��ȡ������
//==========================================================
uint32_t FLASH_ReadWord_One(uint32_t faddr)
{
	return *(__IO uint32_t *)faddr; 
}	

//======================================================================
//��ָ����ַReadaddr��ʼ��ȡָ������sNumToRead�İ��֣������pBuffer��
//Readaddr:��ʼ��ַ������Ϊ2����������
//pBuffer:������ݵ�ָ��
//sNumToRead:��ȡ�����ݸ���
//======================================================================
void FLASH_ReadHalfWord(uint32_t Readaddr,uint16_t *pBuffer,uint16_t sNumToRead)
{
	uint16_t i;
	for(i=0;i<sNumToRead;i++)
	{								
		pBuffer[i]=FLASH_ReadHalfWord_One(Readaddr);   //��ȡ���ּ�2���ֽ�
		Readaddr+=2;      //��ȡ��2���ֽڣ���ַƫ��2
	}
}
//======================================================================
//��ָ����ַWriteaddr��ʼд��ָ������sNumToRead�İ���
//Writeaddr:��ʼ��ַ������Ϊ2����������
//pBuffer:����ָ��
//sNumToWrite:д������ݸ���
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
//��ָ����ַWriteaddr��ʼд�볤��ΪsNumToWrite������
//Writeaddr:��ʼ��ַ������Ϊ2����������
//pBuffer:����ָ��
//sNumToRead:д������ݸ���
//======================================================================
void FLASH_Write(uint32_t Writeaddr,uint16_t *pBuffer,uint16_t sNumToWrite)
{
	uint32_t pageNum;        //ҳ��ţ�Ҫд�ĵ�ַ����һҳ��
	uint16_t pageOff;     //ҳƫ�ƿռ�
	uint16_t pageMain;    //ҳʣ��ռ�
	
	uint32_t offAddr;         //ʵ��ƫ�Ƶ�ַ��д���ַ-flash����ַ��
	
	uint16_t i;

	if((Writeaddr<STM32_FLASH_BASE)||(Writeaddr>(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
	{
		return;
	}
	
	HAL_FLASH_Unlock();       //����
	offAddr=Writeaddr-STM32_FLASH_BASE;       //ʵ��ƫ�Ƶ�ַ
	pageNum=offAddr/STM_PAGE_SIZE;           //ҳ��ţ�ʵ��ƫ�Ƶ�ַ��ȥҳ�Ĵ�С���Ϳ����������һҳ��
	pageOff=(offAddr%STM_PAGE_SIZE)/2;        //Ҫд�������ַ����һҳ��ƫ�ƿռ䣨�԰���λ��λ��
	pageMain=STM_PAGE_SIZE/2-pageOff;         //��һҳ��ʣ����ٿռ䣨����һҳʣ��ռ仹��д���ٰ��֣�
	
	if(sNumToWrite<=pageMain)                 //���д���������ҳ�淶Χ��
	{
		pageMain=sNumToWrite;
	}

	while(1)
	{
		FLASH_ReadHalfWord(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_PAGE_SIZE/2);  //��������ҳ�������
		for(i=0;i<pageMain;i++)
		{
			if(STMFLASH_BUF[pageOff+i]!=0xFFFF)
				break;                                             //��Ҫ����
		}
		if(i<pageMain)
		{
			FLASH_PageErase(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE);     //������һҳ
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);   //cube�����⣬FLASH_PageErase������ִ�����֮��û�а�ҳ�����Ĵ�������
			for(i=0;i<pageMain;i++)
			{
				STMFLASH_BUF[pageOff+i]=pBuffer[i];
			}
			FLASH_WriteHalfWord(pageNum*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_PAGE_SIZE/2);    //���޸Ĺ�������д����һҳ
		}
		else
		{
			FLASH_WriteHalfWord(Writeaddr,pBuffer,pageMain);           //���flash�Ѿ������ˣ���ôֱ�ӽ�����д��ȥ�����ö�
		}
		
		if(sNumToWrite==pageMain)
			break;                       //д���ˣ��˳�
		else        //���һҳûд��
		{
//			LED_On(LED_CTL_W_Pin);    //���Կ�ҳдflash�������ҳ�ˣ��ƻ���
			pageNum++;                //����һҳд
			pageOff=0;                //�µ�һҳƫ����Ϊ0
			pBuffer+=pageMain;        //ָ��ƫ��
			Writeaddr+=pageMain*2;    //д��ַƫ��
			sNumToWrite-=pageMain;    //ȥ����д������
			if(sNumToWrite>STM_PAGE_SIZE/2)     //���дһҳ��д����
			{
				pageMain=STM_PAGE_SIZE/2;         //д����ҳ
			}
			else
			{
				pageMain=sNumToWrite;      //��һҳ����д��
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
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);   //cube�����⣬FLASH_PageErase������ִ�����֮��û�а�ҳ�����Ĵ�������
	HAL_FLASH_Lock();
}


