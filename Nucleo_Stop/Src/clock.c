#include "clock.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile tClockTime sys_tick_count;
/*---------------------------------------------------------------------------*/
void SysCount_Handler(void)
{
    sys_tick_count++;
}
void Clock_Set(void)
{
    sys_tick_count = 0;
}
/*---------------------------------------------------------------------------*/
tClockTime Clock_Time(void)
{
    return sys_tick_count;
}
/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 1 ms.
 *
 */
void Clock_Wait(uint32_t i)
{
    tClockTime start;

    start = Clock_Time();
    while(Clock_Time() - start < (tClockTime)i);
}
/*---------------------------------------------------------------------------*/

