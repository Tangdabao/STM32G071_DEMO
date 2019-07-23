/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <osal.h>

/**
 * Osal_MemCpy
 *
 */

void *Osal_MemCpy(void *dest, const void *src, unsigned int size)
{
    return(memcpy(dest, src, size));
}

/**
 * Osal_MemSet
 *
 */

void *Osal_MemSet(void *ptr, int value, unsigned int size)
{
    return(memset(ptr, value, size));
}

/**
 * Osal_MemCmp
 *
 */
int Osal_MemCmp(void *s1, void *s2, unsigned int size)
{
    return(memcmp(s1, s2, size));
}

