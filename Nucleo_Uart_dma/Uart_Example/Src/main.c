/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

extern UART_HandleTypeDef hlpuart1;
DataFromUart S_Data_From_Uart;
void SystemClock_Config(void);



int main(void)
{
  uint8_t aTxStartMessage[] = "\r\n****UART-Hyperterminal communication Test ****\r\n";
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
#if defined (UART_POLL)
	
    HAL_UART_Transmit(&hlpuart1, aTxStartMessage, sizeof(aTxStartMessage) - 1, 100);

#elif defined (UART_INIT)

    HAL_UART_Transmit_IT(&hlpuart1, aTxStartMessage, sizeof(aTxStartMessage) - 1);
    HAL_UART_Receive_IT(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, UART_RX_SIZEOF);
		
#elif defined (UART_DMA)
	
    HAL_UART_Transmit_DMA(&hlpuart1,aTxStartMessage,sizeof(aTxStartMessage) - 1);
		HAL_UART_Receive_DMA(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, UART_RX_SIZEOF);
	
#endif

    while (1)
    {
#if defined (UART_POLL)

        if(HAL_UART_Receive(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, UART_RX_SIZEOF, 200) == HAL_OK)
        {
            HAL_UART_Transmit(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, sizeof(S_Data_From_Uart.S_ReceiveBuff), 100);
        }
        //HAL_Delay(200);
#elif defined (UART_INIT)
        if(S_Data_From_Uart.S_DataReceiveFlag == 1)
        {
					  S_Data_From_Uart.S_DataReceiveFlag = 0; 
            HAL_UART_Transmit_IT(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, sizeof(S_Data_From_Uart.S_ReceiveBuff));
					  printf("\n\r");
            HAL_UART_Receive_IT(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, UART_RX_SIZEOF);
					  
        }

#elif defined (UART_DMA)
				
				 if(S_Data_From_Uart.S_DataReceiveFlag == 1)
				 {
				   S_Data_From_Uart.S_DataReceiveFlag = 0; 
					 HAL_UART_Transmit_DMA(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, sizeof(S_Data_From_Uart.S_ReceiveBuff));
					 HAL_UART_Receive_DMA(&hlpuart1, S_Data_From_Uart.S_ReceiveBuff, UART_RX_SIZEOF); 
				 }
			
#endif
    }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == LPUART1)
    {
        printf("Tx Transfer completed callback\r\n");
    }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == LPUART1)
    {
        S_Data_From_Uart.S_DataReceiveFlag = 1;
        printf("Rx Transfer completed callback\r\n");
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
