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
void GPIO_Init(GPIO_Pin_Handle_t* pGPIOHandle) {

	if (pGPIOHandle->GPIO_pin_config.GPIO_PinMode <= GPIO_MODE_ANALOG) {
	
		uint32_t temp = 0U;

		/* 1. Configure the mode of GPIO pin */
		temp = pGPIOHandle->GPIO_pin_config.GPIO_PinMode
			<< (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber));

		pGPIOHandle->pGPIOx->MODER |= temp;


		/* 2. Configure the speed */
		temp = 0U;

		temp = pGPIOHandle->GPIO_pin_config.GPIO_PinSpeed
			<< (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

		pGPIOHandle->pGPIOx->OTSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber));

		pGPIOHandle->pGPIOx->OTSPEEDR |= temp;


		/* 3. Configure pull-up / pull-down settings */
		temp = 0U;

		temp = pGPIOHandle->GPIO_pin_config.GPIO_PinPuPdControl
			<< (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

		pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber));

		pGPIOHandle->pGPIOx->PUPDR |= temp;


		/* 4. Configure output type */
		temp = 0U;

		temp = pGPIOHandle->GPIO_pin_config.GPIO_PinoutputType
			<< pGPIOHandle->GPIO_pin_config.GPIO_PinNumber;

		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (2 * pGPIOHandle->GPIO_pin_config.GPIO_PinNumber));

		pGPIOHandle->pGPIOx->OTYPER |= temp;

		/* 5. Configure Alternate function type */

		if (pGPIOHandle->GPIO_pin_config.GPIO_PinMode == GPIO_MODE_ALTFN) {
		
			uint8_t temp1, temp2;

			temp1 = pGPIOHandle->GPIO_pin_config.GPIO_PinNumber / 8;

			temp2 = pGPIOHandle->GPIO_pin_config.GPIO_PinNumber % 8;

			pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0XF << (4 * temp2));

			pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_pin_config.GPIO_AltFuncMode << (4 * temp2));
		
		}
	}
	else {
	
		// init  falling/rising edge, init which line of EXTI is used, init which port is connected (ALL PINWISE)
		if (pGPIOHandle->GPIO_pin_config.GPIO_PinMode == GPIO_MODE_IT_FT) {

			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_pin_config.GPIO_PinMode == GPIO_MODE_IT_RT) {

			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_pin_config.GPIO_PinMode == GPIO_MODE_IT_RFT) {

			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

		}

		uint8_t temp1 = pGPIOHandle->GPIO_pin_config.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_pin_config.GPIO_PinNumber % 4;
		uint8_t portBinary = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_CLOCK_EN();
		SYSCFG->EXTICR[temp1] |= (portBinary << 4 * temp2);
		
		
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_pin_config.GPIO_PinNumber);

	
	}





}
void GPIO_DeInit(GPIO_RegStruct_t* pGPIOx)
{	
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
}


/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber) {
	
	uint8_t value;

	value = (uint8_t)(pGPIOx->IDR >> PinNumber) & 0x1;

	return value;

}
uint16_t GPIO_ReadFromInputPort(GPIO_RegStruct_t* pGPIOx) {

	uint16_t value;

	value = (uint16_t)(pGPIOx->IDR);

	return value;

}

void GPIO_WriteToOutputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber, uint8_t Value) {

	if (Value == GPIO_PIN_SET) {
	
		pGPIOx->ODR |= (Value << PinNumber);

	}
	else {
	
		pGPIOx->ODR &= ~(Value << PinNumber);

	}

}
void GPIO_WriteToOutputPort(GPIO_RegStruct_t* pGPIOx, uint16_t Value) {

	pGPIOx->ODR = Value;

}

void GPIO_ToggleOutputPin(GPIO_RegStruct_t* pGPIOx, uint8_t PinNumber) {

	pGPIOx->ODR ^= (1 << PinNumber);

}


/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		/*
		 * Configure ISER register
		 */

		if (IRQNumber <= 31U)
		{
			/* Program ISER0 register */
			*NVIC_ISER0 |= (1U << IRQNumber);
		}
		else if (IRQNumber > 31U && IRQNumber < 64U)
		{
			/* Program ISER1 register */
			*NVIC_ISER1 |= (1U << (IRQNumber % 32U));
		}
		else if (IRQNumber >= 64U && IRQNumber < 96U)
		{
			/* Program ISER2 register */
			*NVIC_ISER2 |= (1U << (IRQNumber % 64U));
		}
	}
	else
	{
		/*
		 * Configure ICER register
		 */

		if (IRQNumber <= 31U)
		{
			/* Program ICER0 register */
			*NVIC_ICER0 |= (1U << IRQNumber);
		}
		else if (IRQNumber > 31U && IRQNumber < 64U)
		{
			/* Program ICER1 register */
			*NVIC_ICER1 |= (1U << (IRQNumber % 32U));
		}
		else if (IRQNumber >= 64U && IRQNumber < 96U)
		{
			/* Program ICER2 register */
			*NVIC_ICER2 |= (1U << (IRQNumber % 64U));
		}
	}

}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	//low bits are not implemented while only high bits are implemented

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{

	//clear the pending register by writing one so that the Nvic can start processing the EXRI_IRQ_handler implemented in main 
	if (EXTI->PR & (1 << PinNumber))
	{
		EXTI->PR |= (1 << PinNumber);
	}
}


#endif // !stm32F401Re_gpio_driver.c
