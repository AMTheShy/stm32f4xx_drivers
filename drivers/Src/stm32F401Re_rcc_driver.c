#ifndef stm32F401Re_rcc_driver.c
#define stm32F401Re_rcc_driver.c

#include "stm32F401Re_rcc_driver.h"

static const uint32_t AHB_Prescaler[8] =
{
	2U,
	4U,
	8U,
	16U,
	64U,
	128U,
	256U,
	512U
};

static const uint32_t APB_Prescaler[4] =
{
	2U,
	4U,
	8U,
	16U
};


/*********************************************************************
 * @fn      		  - RCC_GetPLLClockFreq
 *
 * @brief             - Returns the PLL output clock frequency (SYSCLK
 *                      when PLL is selected as system clock source).
 *
 * @param[in]         - None
 *
 * @return            - PLL clock frequency in Hz
 *
 * @Note              - PLL source, M, N, and P values are read from
 *                      RCC_PLLCFGR.
 */
uint32_t RCC_GetPLLClockFreq(void)
{
	uint32_t pllclk;
	uint32_t pllsource;
	uint32_t pllm;
	uint32_t plln;
	uint32_t pllp;
	uint32_t pllpdiv;

	pllsource = (RCC->PLLCFGR >> 22) & 0x1U;

	if (pllsource == 0U)
	{
		pllsource = HSI_CLOCK_FREQ;
	}
	else
	{
		pllsource = HSE_CLOCK_FREQ;
	}

	pllm = RCC->PLLCFGR & 0x3FU;
	plln = (RCC->PLLCFGR >> 6) & 0x1FFU;
	pllp = (RCC->PLLCFGR >> 16) & 0x3U;

	if (pllp == 0U)
	{
		pllpdiv = 2U;
	}
	else if (pllp == 1U)
	{
		pllpdiv = 4U;
	}
	else if (pllp == 2U)
	{
		pllpdiv = 6U;
	}
	else
	{
		pllpdiv = 8U;
	}

	pllclk = ((pllsource / pllm) * plln) / pllpdiv;

	return pllclk;
}


/*********************************************************************
 * @fn      		  - RCC_GetPCLK1FreqValue
 *
 * @brief             - Returns the APB1 (PCLK1) bus clock frequency.
 *
 * @param[in]         - None
 *
 * @return            - PCLK1 frequency in Hz
 *
 * @Note              - Uses SWS, HPRE, and PPRE1 fields from RCC_CFGR.
 */
uint32_t RCC_GetPCLK1FreqValue(void)
{
	uint8_t clkBinary;
	uint32_t PCLK1;
	uint32_t clkSourceFreq;
	uint32_t temp;
	uint32_t ahbp;
	uint32_t apb1p;

	clkBinary = ((RCC->CFGR >> 2) & 0x3U);

	if (clkBinary == HSI)
	{
		clkSourceFreq = HSI_CLOCK_FREQ;
	}
	else if (clkBinary == HSE)
	{
		clkSourceFreq = HSE_CLOCK_FREQ;
	}
	else if (clkBinary == PLL)
	{
		clkSourceFreq = RCC_GetPLLClockFreq();
	}
	else
	{
		clkSourceFreq = HSI_CLOCK_FREQ;
	}

	temp = ((RCC->CFGR >> 4) & 0xFU);

	if (temp < 8U)
	{
		ahbp = 1U;
	}
	else
	{
		ahbp = AHB_Prescaler[temp - 8U];
	}

	temp = ((RCC->CFGR >> 10) & 0x7U);

	if (temp < 4U)
	{
		apb1p = 1U;
	}
	else
	{
		apb1p = APB_Prescaler[temp - 4U];
	}

	PCLK1 = (clkSourceFreq / ahbp) / apb1p;

	return PCLK1;
}


/*********************************************************************
 * @fn      		  - RCC_GetPCLK2FreqValue
 *
 * @brief             - Returns the APB2 (PCLK2) bus clock frequency.
 *
 * @param[in]         - None
 *
 * @return            - PCLK2 frequency in Hz
 *
 * @Note              - Uses SWS, HPRE, and PPRE2 fields from RCC_CFGR.
 */
uint32_t RCC_GetPCLK2FreqValue(void)
{
	uint8_t clkBinary;
	uint32_t PCLK2;
	uint32_t clkSourceFreq;
	uint32_t temp;
	uint32_t ahbp;
	uint32_t apb2p;

	clkBinary = ((RCC->CFGR >> 2) & 0x3U);

	if (clkBinary == HSI)
	{
		clkSourceFreq = HSI_CLOCK_FREQ;
	}
	else if (clkBinary == HSE)
	{
		clkSourceFreq = HSE_CLOCK_FREQ;
	}
	else if (clkBinary == PLL)
	{
		clkSourceFreq = RCC_GetPLLClockFreq();
	}
	else
	{
		clkSourceFreq = HSI_CLOCK_FREQ;
	}

	temp = ((RCC->CFGR >> 4) & 0xFU);

	if (temp < 8U)
	{
		ahbp = 1U;
	}
	else
	{
		ahbp = AHB_Prescaler[temp - 8U];
	}

	temp = ((RCC->CFGR >> 13) & 0x7U);

	if (temp < 4U)
	{
		apb2p = 1U;
	}
	else
	{
		apb2p = APB_Prescaler[temp - 4U];
	}

	PCLK2 = (clkSourceFreq / ahbp) / apb2p;

	return PCLK2;
}


#endif
