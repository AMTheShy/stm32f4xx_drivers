#ifndef INC_STM32F401RE_RCC_DRIVER_H
#define INC_STM32F401RE_RCC_DRIVER_H

#include "stm32F401Re.h"

/*
 * Clock source values
 *
 * HSI is fixed at 16 MHz on STM32F401RE.
 *
 * HSE_CLOCK depends on the actual external clock source used
 * by your board. For the course setup, 8 MHz is assumed.
 */
#define HSI_CLOCK_FREQ      16000000U
#define HSE_CLOCK_FREQ       8000000U

#define HSI                 0
#define HSE                 1
#define PLL                 2


/*********************************************************************
 * APIs supported by this driver
 *********************************************************************/

uint32_t RCC_GetPLLClockFreq(void);
uint32_t RCC_GetPCLK1FreqValue(void);
uint32_t RCC_GetPCLK2FreqValue(void);


#endif /* INC_STM32F401RE_RCC_DRIVER_H */
