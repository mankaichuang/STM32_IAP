/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : STM32F103RCT6_IAP
  * @Created on		: 2021-03-05
  * Author			: mankaichuang
  ******************************************************************************
**/

#include "main.h"
#include "usb_device.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "updata.h"
#include "oled.h"

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	Delay_init(72);
	LED_Init();
	USART2_Init(115200);
	OLED_Init();
	OLED_ShowString(20,2,(uint8_t *)"STM32_BOOT",16,1);
	
	while(1)
	{		
		sendRequest(REQUEST_UPDATA);	//发送升级请求
		Delay_ms(3000);
		//如果收到升级请求
		if((USART_RX_STA & 0x8000) && (USART_RX_BUF[0] == UPDATA_REDAY))
		{
			//获取发送包数量
			updataPackage = USART_RX_BUF[1];
			USART_RXbuffer_init();
			updataFlash();
		}
		else	//跳转到APP
		{
			if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
			{
			  JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4); //APP address
			  JumpToApplication = (pFunction) JumpAddress;
			  /* Initialize user application's Stack Pointer */
			  __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
			  JumpToApplication();	
			}
		}
	}
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
