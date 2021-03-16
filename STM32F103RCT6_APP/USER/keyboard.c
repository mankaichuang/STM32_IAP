/**
  ******************************************************************************
  * @file           : keyboard.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#include "keyboard.h" 
#include "delay.h"
#include "led.h"


void KeyBoard_IO_Init(KEY_PINState KEYState)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	if(KEYState!=KEY_IN_OUT)
	{
			GPIO_InitStruct.Pin = KEY_PB10_Pin|KEY_PB11_Pin|KEY_PB8_Pin|KEY_PB9_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
			HAL_GPIO_WritePin(GPIOB, KEY_PB10_Pin|KEY_PB11_Pin|KEY_PB8_Pin|KEY_PB9_Pin, GPIO_PIN_RESET);
	
			GPIO_InitStruct.Pin = KEY_PB12_Pin|KEY_PB13_Pin|KEY_PB14_Pin|KEY_PB15_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else
		{
			GPIO_InitStruct.Pin = KEY_PB12_Pin|KEY_PB13_Pin|KEY_PB14_Pin|KEY_PB15_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
			HAL_GPIO_WritePin(GPIOB, KEY_PB12_Pin|KEY_PB13_Pin|KEY_PB14_Pin|KEY_PB15_Pin, GPIO_PIN_RESET);
	
			GPIO_InitStruct.Pin = KEY_PB10_Pin|KEY_PB11_Pin|KEY_PB8_Pin|KEY_PB9_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}



//==========================================================
//键盘程序，获取键值，松手检测 （库函数版）
//==========================================================
uint8_t ReadKey(void)
{
	uint8_t x, y;
	GPIO_PinState keyin;
	
	uint8_t KeyValue[4][4] = {
								{0x01,0x02,0x03,0x12},	 //0x12-->back
								{0x04,0x05,0x06,0x13},   //0x13-->up
								{0x07,0x08,0x09,0x14},   //0x14-->down
								{0x10,0x20,0x11,0x15}    //0x10-->del;0x11-->dot;0x15-->enter
			                 };

	keyin = GPIO_PIN_SET;
	KeyBoard_IO_Init(KEY_IN_OUT);
	while(((KB8_State && KB9_State && KB10_State && KB11_State) !=keyin))  //当有按键按下
	{

		if((KB8_State && KB9_State && KB10_State && KB11_State) !=keyin)   //当有按键按下，检查行
		{
			if(KB8_State !=keyin)              //第一行被按下
			{
				x=0;
			}
			else if(KB9_State !=keyin)         //第二行被按下
			{
				x=1;			
			}
			else if(KB10_State !=keyin)
			{
				x=2;
			}
			else if(KB11_State !=keyin)
			{
				x=3;
			}
		}
		else 
			return 0;

	
		KeyBoard_IO_Init(KEY_OUT_IN);
		
		if((KB12_State && KB13_State && KB14_State && KB15_State) !=keyin)    //当有按键按下，检查列
		{
			if(KB12_State !=keyin)         //第一列被按下
			{
				y=0;
			}
			else if(KB13_State !=keyin)
			{
				y=1;
			}
			else if(KB14_State !=keyin)
			{
				y=2;
			}
			else if(KB15_State !=keyin)
			{
				y=3;
			}
		}
		else 
			return 0;
		
		while((KB12_State && KB13_State && KB14_State && KB15_State) !=keyin);   //检查按键是否松开


		return KeyValue[x][y];
	}
	return 0;
}

//==========================================================
//键盘程序，获取键值，松手检测 （寄存器版）
//==========================================================
uint8_t ReadKey_R(void)
{
	uint8_t x, y;
	uint32_t Keyin;


	
	uint8_t KeyValue[4][4] = {
								{0x01,0x02,0x03,0x12},	 //0x12-->back
								{0x04,0x05,0x06,0x13},   //0x13-->up
								{0x07,0x08,0x09,0x14},   //0x14-->down
								{0x10,0x20,0x11,0x15}    //0x10-->del;0x11-->dot;0x15-->enter
			                 };

	KeyBoard_IO_Init(KEY_IN_OUT);
	
	Keyin =GPIOB->IDR&0x0000ff00;

	while(Keyin!=0x00000f00)  //当有按键按下
	{
		Keyin =GPIOB->IDR&0x0000ff00;
		if(Keyin!=0x00000f00)   //当有按键按下，检查行
		{
				switch(Keyin)
				{
					case 0x00000e00:
						x = 0;
						break;
						
					case 0x00000d00:
						x = 1;
						break;
						
					case 0x00000b00:
						x = 2;
						break;
						
					case 0x00000700:
						x = 3;
						break;
												
					default:
						break;
				}
				if(x == 0xff)
				{
					break;
				}
				
		}
		else
		{
			break;
		}

	
		KeyBoard_IO_Init(KEY_OUT_IN);
		
		Keyin = GPIOB->IDR&0x0000ff00;
			
		
		if(Keyin!=0x0000f000)    //当有按键按下，检查列
		{
				switch(Keyin)
				{
					case 0x0000e000:
						y = 0;
						break;
						
					case 0x0000d000:
						y = 1;
						break;
						
					case 0x0000b000:
						y = 2;
						break;
						
					case 0x00007000:
						y = 3;
						break;
												
					default:
						y = 0xff;
						break;
				}
				if(y == 0xff)
				{
					break;
				}
		}
		else
		{
			break;
		}
		while((GPIOB->IDR&0x0000ff00)!=0x0000f000);     //按键松手检测
		return KeyValue[x][y];
	}
	return 0;
}

//==========================================================
//键盘程序，获取键值， （寄存器版）
//ucOver: 1,松手检测  ; 0,不做松手检测
//==========================================================
uint8_t ReadKey_Over(uint8_t ucOver)
{
	uint8_t x, y;
	uint32_t Keyin;


	
	uint8_t KeyValue[4][4] = {
								{0x01,0x02,0x03,0x12},	 //0x12-->back
								{0x04,0x05,0x06,0x13},   //0x13-->up
								{0x07,0x08,0x09,0x14},   //0x14-->down
							    {0x10,0x20,0x11,0x15}    //0x10-->del;0x11-->dot;0x15-->enter
			                 };

	KeyBoard_IO_Init(KEY_IN_OUT);
	
	Keyin =GPIOB->IDR&0x0000ff00;

	while(Keyin!=0x00000f00)  //当有按键按下
	{
		Keyin =GPIOB->IDR&0x0000ff00;
		if(Keyin!=0x00000f00)   //当有按键按下，检查行
		{
				switch(Keyin)
				{
					case 0x00000e00:
						x = 0;
						break;
						
					case 0x00000d00:
						x = 1;
						break;
						
					case 0x00000b00:
						x = 2;
						break;
						
					case 0x00000700:
						x = 3;
						break;
												
					default:
						break;
				}
				if(x == 0xff)
				{
					break;
				}
				
		}
		else
		{
			break;
		}

	
		KeyBoard_IO_Init(KEY_OUT_IN);
		
		Keyin = GPIOB->IDR&0x0000ff00;
			
		
		if(Keyin!=0x0000f000)    //当有按键按下，检查列
		{
				switch(Keyin)
				{
					case 0x0000e000:
						y = 0;
						break;
						
					case 0x0000d000:
						y = 1;
						break;
						
					case 0x0000b000:
						y = 2;
						break;
						
					case 0x00007000:
						y = 3;
						break;
												
					default:
						y = 0xff;
						break;
				}
				if(y == 0xff)
				{
					break;
				}
		}
		else
		{
			break;
		}
		if(ucOver)
		{
			while((GPIOB->IDR&0x0000ff00)!=0x0000f000);     //按键松手检测
		}
			return KeyValue[x][y];
	}
	return 0;
}

//void Key_Enter_Oled(int x,int y)
//{
//	switch(ReadKey_R())
//	{
//		case 0x01:
//		OLED_ShowString(x,y,(uint8_t *)"1",16,1);
//		break;
//		
//	  case 0x02:
//		OLED_ShowString(x,y,(uint8_t *)"2",16,1); 
//		break;
//		
//		case 0x03:
//		OLED_ShowString(x,y,(uint8_t *)"3",16,1);
//		break;
//		
//		case 0x04:	
//		OLED_ShowString(x,y,(uint8_t *)"4",16,1);
//		break;
//	
//		case 0x05:
//		OLED_ShowString(x,y,(uint8_t *)"5",16,1);
//		break;

//		case 0x06:
//		OLED_ShowString(x,y,(uint8_t *)"6",16,1);
//		break;
//		
//		case 0x07:
//		OLED_ShowString(x,y,(uint8_t *)"7",16,1);
//		break;
//		
//		case 0x08:
//		OLED_ShowString(x,y,(uint8_t *)"8",16,1);
//		break;
//	
//		case 0x09:
//		OLED_ShowString(x,y,(uint8_t *)"9",16,1);
//		break;

//		case 0x20:
//		OLED_ShowString(x,y,(uint8_t *)"0",16,1);
//		break;
//		
//		case 0x10:
//		OLED_ShowString(x,y,(uint8_t *)"DEL",16,1);
//		break;
//		
//		case 0x11:
//		OLED_ShowString(x,y,(uint8_t *)".",16,1);
//		break;
//		
//		case 0x12:
//		OLED_ShowString(x,y,(uint8_t *)"BACK",16,1);
//		break;

//		case 0x13:
//		OLED_ShowString(x,y,(uint8_t *)"UP",16,1);
//		break;

//		case 0x14:
//		OLED_ShowString(x,y,(uint8_t *)"DOWN",16,1);
//		break;

//		case 0x15:
//		OLED_ShowString(x,y,(uint8_t *)"ENTER",16,1);
//		break;		
//	}
//}

//void Key_Oled_Display(int x,int y)
//{
//		OLED_ShowString(x,y,(uint8_t *)" ",16,1);
//		Delay_ms(300);
//		OLED_ShowString(x,y,(uint8_t *)"|",16,1);
//		Delay_ms(300);

//		if(ReadKey_Over(0)!=0)
//		{
//			Key_Enter_Oled(x,y);
//			x+=8;
//			if(x>127)
//			{
//				y=y+2;
//				x=0;
//			}
//			if(y>6)
//			{
//				OLED_Clear();
//				y=0;
//			}
//		}
//}


