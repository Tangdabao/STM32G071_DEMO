/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/* USER CODE END 0 */

UART_HandleTypeDef hlpuart1;

/* LPUART1 init function */

void MX_LPUART1_UART_Init(void)
{

    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 115200;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;
    hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }

}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(uartHandle->Instance == LPUART1)
    {
        /* USER CODE BEGIN LPUART1_MspInit 0 */

        /* USER CODE END LPUART1_MspInit 0 */
        /* LPUART1 clock enable */
        __HAL_RCC_LPUART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**LPUART1 GPIO Configuration
        PA2     ------> LPUART1_TX
        PA3     ------> LPUART1_RX
        */
        GPIO_InitStruct.Pin = LPUART1_TX_Pin | LPUART1_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_LPUART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* LPUART1 interrupt Init */
        __HAL_UART_DISABLE(&hlpuart1);
				CLEAR_BIT(LPUART1->ISR, USART_ISR_RXNE_RXFNE);
			 /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			  SET_BIT(LPUART1->CR3, USART_CR3_EIE);
        SET_BIT(LPUART1->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE_RXFNEIE);
        HAL_NVIC_SetPriority(USART3_4_LPUART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART3_4_LPUART1_IRQn);
         __HAL_UART_ENABLE(&hlpuart1);
        /* USER CODE BEGIN LPUART1_MspInit 1 */

        /* USER CODE END LPUART1_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

    if(uartHandle->Instance == LPUART1)
    {
        /* USER CODE BEGIN LPUART1_MspDeInit 0 */

        /* USER CODE END LPUART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_LPUART1_CLK_DISABLE();

        /**LPUART1 GPIO Configuration
        PA2     ------> LPUART1_TX
        PA3     ------> LPUART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, LPUART1_TX_Pin | LPUART1_RX_Pin);

        /* LPUART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART3_4_LPUART1_IRQn);
        /* USER CODE BEGIN LPUART1_MspDeInit 1 */

        /* USER CODE END LPUART1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

void UART_SendByte(UART_HandleTypeDef *uartHandle, uint8_t data)
{
    if (uartHandle->Instance == LPUART1)
    {
        while((LPUART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
        LPUART1->TDR = data;
    }
}

void UART1_SendString(UART_HandleTypeDef *uartHandle, char *str)
{
    if (uartHandle->Instance == LPUART1)
    {
        while((*str) != 0)
        {
            while((LPUART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
            LPUART1->TDR = *str++;
        }
    }
}

PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
