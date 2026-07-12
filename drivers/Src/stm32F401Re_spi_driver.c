#ifndef STM32F401RE_SPI_DRIVER.C
#define STM32F401RE_SPI_DRIVER.C

#include "stm32F401Re_spi_driver.h"

static void SPI_ClearOVRFlag(SPI_RegStruct_t* pSPIx);
static void SPI_CloseTransmission(SPI_Handle_t* pSPIHandle);
static void SPI_CloseReception(SPI_Handle_t* pSPIHandle);
static void SPI_TXE_InterruptHandle(SPI_Handle_t* pSPIHandle);
static void SPI_RXNE_InterruptHandle(SPI_Handle_t* pSPIHandle);
static void SPI_OVR_ERR_InterruptHandle(SPI_Handle_t* pSPIHandle);
/*----------------------------------------------------------PRIVATE HELPER FUNCTIONS-----------------------------------------------------------*/

static void SPI_ClearOVRFlag(SPI_RegStruct_t* pSPIx)
{
	uint8_t temp;

	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

static void SPI_CloseTransmission(SPI_Handle_t* pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1U << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0U;
	pSPIHandle->TxState = SPI_READY;
}

static void SPI_CloseReception(SPI_Handle_t* pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1U << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0U;
	pSPIHandle->RxState = SPI_READY;
}

static void SPI_TXE_InterruptHandle(SPI_Handle_t* pSPIHandle)
{
	if (pSPIHandle->pSPIx->CR1 & (1U << SPI_CR1_DFF))
	{
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen -= 2U;
		pSPIHandle->pTxBuffer += 2U;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if (!pSPIHandle->TxLen)
	{
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}

static void SPI_RXNE_InterruptHandle(SPI_Handle_t* pSPIHandle)
{
	if (pSPIHandle->pSPIx->CR1 & (1U << SPI_CR1_DFF))
	{
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2U;
		pSPIHandle->pRxBuffer += 2U;
	}
	else
	{
		*pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if (!pSPIHandle->RxLen)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void SPI_OVR_ERR_InterruptHandle(SPI_Handle_t* pSPIHandle)
{
	/*
	 * If SPI is busy in transmission, OVR flag is not cleared here.
	 * Application is notified through the callback.
	 */
	if (pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		SPI_ClearOVRFlag(pSPIHandle->pSPIx);
	}

	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------*/

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegStruct_t* pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_CLOCK_EN();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_CLOCK_EN();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_CLOCK_EN();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_CLOCK_EN();
		}
	}
	else
	{
		if (pSPIx == SPI1)
		{
			SPI1_CLOCK_DI();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_CLOCK_DI();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_CLOCK_DI();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_CLOCK_DI();
		}
	}
}


/*
 * Init and De-init
 */

	void SPI_Init(SPI_Handle_t * pSPIHandle)
	{
		uint32_t tempreg = 0U;

		/*
		 * 1. Configure SPI device mode
		 *
		 * MSTR bit in SPI_CR1:
		 * 0 = slave mode
		 * 1 = master mode
		 *
		 * Important:
		 * Only master produces SCK.
		 * If SPI clock is not coming out, one thing to check is MSTR bit.
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);


		/*
		 * 2. Configure SPI bus configuration
		 *
		 * Full-duplex:
		 * - BIDIMODE = 0
		 * - RXONLY   = 0
		 * - Uses two data lines: MOSI and MISO
		 *
		 * Half-duplex:
		 * - BIDIMODE = 1
		 * - Uses one bidirectional data line
		 *
		 * Simplex receive-only:
		 * - BIDIMODE = 0
		 * - RXONLY   = 1
		 * - Master can receive while still producing SCK
		 */
		if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
		{
			/*
			 * 2-line unidirectional full-duplex mode.
			 * BIDIMODE = 0.
			 * RXONLY = 0.
			 *
			 * tempreg starts from 0, so these bits are already 0.
			 * These lines are written for readability.
			 */
			tempreg &= ~(1U << SPI_CR1_BIDIMODE);
			tempreg &= ~(1U << SPI_CR1_RXONLY);
		}
		else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
		{
			/*
			 * 1-line bidirectional half-duplex mode.
			 * BIDIMODE = 1.
			 *
			 * Direction is controlled later by BIDIOE:
			 * BIDIOE = 1 -> transmit
			 * BIDIOE = 0 -> receive
			 */
			tempreg |= (1U << SPI_CR1_BIDIMODE);
		}
		else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
		{
			/*
			 * 2-line receive-only mode.
			 * BIDIMODE = 0.
			 * RXONLY = 1.
			 *
			 * This is used when master only wants to receive from slave.
			 * SPI master still needs to produce SCK.
			 */
			tempreg &= ~(1U << SPI_CR1_BIDIMODE);
			tempreg |= (1U << SPI_CR1_RXONLY);
		}


		/*
		 * 3. Configure SPI serial clock speed
		 *
		 * BR[2:0] bits start from bit position 3.
		 *
		 * 000 = fPCLK / 2
		 * 001 = fPCLK / 4
		 * 010 = fPCLK / 8
		 * 011 = fPCLK / 16
		 * 100 = fPCLK / 32
		 * 101 = fPCLK / 64
		 * 110 = fPCLK / 128
		 * 111 = fPCLK / 256
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);


		/*
		 * 4. Configure data frame format
		 *
		 * DFF bit:
		 * 0 = 8-bit data frame
		 * 1 = 16-bit data frame
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);


		/*
		 * 5. Configure clock polarity
		 *
		 * CPOL bit:
		 * 0 = SCK idle LOW
		 * 1 = SCK idle HIGH
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);


		/*
		 * 6. Configure clock phase
		 *
		 * CPHA bit:
		 * 0 = first clock transition is the first data capture edge
		 * 1 = second clock transition is the first data capture edge
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);


		/*
		 * 7. Configure software slave management
		 *
		 * SSM bit:
		 * 0 = software slave management disabled
		 * 1 = software slave management enabled
		 *
		 * For master mode with software slave management,
		 * we often also set SSI = 1 later to avoid MODF error.
		 */
		tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

		pSPIHandle->pSPIx->CR1 = tempreg;
	}

void SPI_DeInit(SPI_RegStruct_t* pSPIx) {
	
	if (pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if (pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if (pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if (pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}

}

uint8_t SPI_GetFlagStatus(SPI_RegStruct_t* pSPIx, uint32_t FlagName)
{
	/*
	 * Check whether the requested flag bit is set in SPI_SR.
	 *
	 * Example:
	 * FlagName = SPI_TXE_FLAG
	 * This checks whether TXE bit in SR is 1.
	 */
	if (pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;
}

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegStruct_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len) {

	while (Len > 0) {
	
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET) {


		}

		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
				
			pSPIx->DR = *((uint16_t*)pTxBuffer);

			Len -= 2;

			pTxBuffer += 2;
		
		}
		else {
		
			pSPIx->DR = *(pTxBuffer);

			Len--;

			pTxBuffer++;
		
		}
	
	}

}
void SPI_ReceiveData(SPI_RegStruct_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len) {

	while (Len > 0) {

		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET) {


		}

		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {

			*((uint16_t*)pRxBuffer) = pSPIx->DR;

			Len -= 2;

			pRxBuffer += 2;

		}
		else {

			*pRxBuffer = pSPIx->DR;

			Len--;

			pRxBuffer++;

		}

	}

}

/*********************************************************************
 * @fn      		  - SPI_SendData_IT
 *
 * @brief             - Starts SPI data transmission in interrupt mode.
 *                      This function does not write data directly into
 *                      the SPI data register. It saves the application
 *                      Tx buffer address and length, marks the SPI as
 *                      busy in transmission, and enables the TXE
 *                      interrupt. The actual transmission is handled
 *                      later inside the SPI interrupt handler.
 *
 * @param[in]         - pSPIHandle: Pointer to the SPI handle structure
 *
 * @param[in]         - pTxBuffer: Pointer to the application transmit
 *                      buffer
 *
 * @param[in]         - Len: Number of bytes or data frames to transmit
 *
 * @return            - Previous SPI transmission state
 *                      SPI_READY:
 *                      A new transmission was started
 *
 *                      SPI_BUSY_IN_TX:
 *                      A transmission was already in progress
 *
 * @Note              - This is a non-blocking API.
 *                      The application Tx buffer must remain valid until
 *                      the interrupt-based transmission is completed.
 */
uint8_t SPI_SendData_IT(SPI_Handle_t* pSPIHandle,
	uint8_t* pTxBuffer,
	uint32_t Len) {

	uint8_t status = pSPIHandle->TxState;

	if (status != SPI_BUSY_IN_TX) {

		pSPIHandle->pTxBuffer = pTxBuffer;

		pSPIHandle->TxLen = Len;

		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		pSPIHandle->pSPIx->CR2 |= (1U << SPI_CR2_TXEIE);

	}

	return status;

}


/*********************************************************************
 * @fn      		  - SPI_ReceiveData_IT
 *
 * @brief             - Starts SPI data reception in interrupt mode.
 *                      This function does not read data directly from
 *                      the SPI data register. It saves the application
 *                      Rx buffer address and length, marks the SPI as
 *                      busy in reception, and enables the RXNE
 *                      interrupt. The actual reception is handled later
 *                      inside the SPI interrupt handler.
 *
 * @param[in]         - pSPIHandle: Pointer to the SPI handle structure
 *
 * @param[in]         - pRxBuffer: Pointer to the application receive
 *                      buffer
 *
 * @param[in]         - Len: Number of bytes or data frames to receive
 *
 * @return            - Previous SPI reception state
 *                      SPI_READY:
 *                      A new reception was started
 *
 *                      SPI_BUSY_IN_RX:
 *                      A reception was already in progress
 *
 * @Note              - This is a non-blocking API.
 *                      The application Rx buffer must remain valid until
 *                      the interrupt-based reception is completed.
 */
uint8_t SPI_ReceiveData_IT(SPI_Handle_t* pSPIHandle,
	uint8_t* pRxBuffer,
	uint32_t Len) {

	uint8_t status = pSPIHandle->RxState;

	if (status != SPI_BUSY_IN_RX) {

		pSPIHandle->pRxBuffer = pRxBuffer;

		pSPIHandle->RxLen = Len;

		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		pSPIHandle->pSPIx->CR2 |= (1U << SPI_CR2_RXNEIE);

	}

	return status;

}


/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	//low bits are not implemented while only high bits are implemented

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);
}
void SPI_IRQHandling(SPI_Handle_t* pHandle)
{
	uint8_t temp1 = 0U;
	uint8_t temp2 = 0U;

	/*
	 * Check whether TXE interrupt occurred
	 */
	temp1 = pHandle->pSPIx->SR & SPI_TXE_FLAG;
	temp2 = pHandle->pSPIx->CR2 & (1U << SPI_CR2_TXEIE);

	if (temp1 && temp2)
	{
		SPI_TXE_InterruptHandle(pHandle);
	}

	/*
	 * Check whether RXNE interrupt occurred
	 */
	temp1 = pHandle->pSPIx->SR & SPI_RXNE_FLAG;
	temp2 = pHandle->pSPIx->CR2 & (1U << SPI_CR2_RXNEIE);

	if (temp1 && temp2)
	{
		SPI_RXNE_InterruptHandle(pHandle);
	}

	/*
	 * Check whether overrun error interrupt occurred
	 */
	temp1 = pHandle->pSPIx->SR & SPI_OVR_FLAG;
	temp2 = pHandle->pSPIx->CR2 & (1U << SPI_CR2_ERRIE);

	if (temp1 && temp2)
	{
		SPI_OVR_ERR_InterruptHandle(pHandle);
	}
}


/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1U << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1U << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1U << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1U << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1U << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1U << SPI_CR2_SSOE);
	}
}


/*
 * Application callback
 */
weak void SPI_ApplicationEventCallback(SPI_Handle_t* pSPIHandle, uint8_t AppEv)
{
	(void)pSPIHandle;
	(void)AppEv;
}

#endif
