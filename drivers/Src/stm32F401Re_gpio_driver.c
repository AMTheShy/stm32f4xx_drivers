#ifndef stm32F401Re_gpio_driver.c
#define stm32F401Re_gpio_driver.c

#include "stm32F401Re_gpio_driver.h"
/*
 * Peripheral Clock setup
 */
 /*********************************************************************
  * @fn      		  - GPIO_PeriClockControl
  *
  * @brief             - This function enables or disables peripheral clock
  *                      for the given GPIO port.
  *
  * @param[in]         - pGPIOx: Base address of the GPIO peripheral
  * @param[in]         - EnorDi: ENABLE or DISABLE macro
  *
  * @return            - none
  *
  * @note              - none
  */
void GPIO_PeriClockControl(GPIO_RegStruct_t* pGPIOx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_CLOCK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_CLOCK_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_CLOCK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_CLOCK_EN();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_CLOCK_EN();
		}
		
		else if (pGPIOx == GPIOH)
		{
			GPIOH_CLOCK_EN();
		}
		
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_CLOCK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_CLOCK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_CLOCK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_CLOCK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_CLOCK_DI();
		}
	
		else if (pGPIOx == GPIOH)
		{
			GPIOH_CLOCK_DI();
		}
	}
}

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Pin_Handle_t* pGPIOHandle);
void GPIO_DeInit(GPIO_RegStruct_t* pGPIOx);


/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegStruct_t* pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegStruct_t* pGPIOx, uint16_t Value);

void GPIO_ToggleOutputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber);


/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif // !stm32F401Re_gpio_driver.c
