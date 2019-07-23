#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "stm32g0xx_hal.h"

#ifndef DEBUG
#define DEBUG 1
#endif

/* Private macros ------------------------------------------------------------*/
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define DATA_BUF_SIZE  128

#define UART_DELAY_MS 100

typedef struct
{
  char aCmdBuff[DATA_BUF_SIZE];
  uint8_t aCmdLenth;
	uint8_t acmdStart;
	uint8_t acmdEnd;
}gCmdGotFromUart;
extern gCmdGotFromUart          gCmdGotFromUart_st;
void InitAll(void);
#endif

