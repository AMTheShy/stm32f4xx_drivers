#ifndef stm32F401Re_I2C_driver.c
#define stm32F401Re_I2C_driver.c
#include "stm32F401Re_I2C_driver.h"

static uint8_t I2C_GetFlagStatus(I2C_RegStruct_t* pI2Cx, uint32_t FlagName);
static void I2C_GenerateStartCondition(I2C_RegStruct_t* pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_RegStruct_t* pI2Cx, uint8_t SlaveAddr, uint8_t ROW);
static void I2C_ManageAcking(I2C_RegStruct_t* pI2Cx, uint8_t EnOrDi);
static void I2C_ClearADDRFlag(I2C_Handle_t* pI2CHandle, uint8_t IsMaster);
static void I2C_GenerateStopCondition(I2C_RegStruct_t* pI2Cx);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t* pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t* pI2CHandle);
static void I2C_CloseSendData(I2C_Handle_t* pI2CHandle);
static void I2C_CloseReceiveData(I2C_Handle_t* pI2CHandle);
/*----------------------------------------------------------PRIVATE HELPER FUNCTIONS-----------------------------------------------------------*/

static uint8_t I2C_GetFlagStatus(I2C_RegStruct_t* pI2Cx, uint32_t FlagName)
{
	/*
	 * Check whether the requested flag bit is set in I2C_SR1.
	 *
	 * Example:
	 * FlagName = I2C_FLAG_TXE
	 * This checks whether the TXE bit in SR1 is 1.
	 */
	if (pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;
}


static void I2C_GenerateStartCondition(I2C_RegStruct_t* pI2Cx)
{
	pI2Cx->CR1 |= (1U << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhase(I2C_RegStruct_t* pI2Cx, uint8_t SlaveAddr,uint8_t ROW)
{
	SlaveAddr = SlaveAddr << 1;

	if (ROW == I2C_READ) {
		SlaveAddr |= (1U); /* Slave address + r/w bit = 1 (read) */
		pI2Cx->DR = SlaveAddr;
	}
	else {
		SlaveAddr &= ~(1U); /* Slave address + r/w bit = 0 (write) */
		pI2Cx->DR = SlaveAddr;
	}
	
}

static void I2C_ManageAcking(I2C_RegStruct_t* pI2Cx, uint8_t EnOrDi)
{
	if (EnOrDi == I2C_ACK_ENABLE)
	{
		/* Enable ACK generation */
		pI2Cx->CR1 |= (1U << I2C_CR1_ACK);
	}
	else
	{
		/* Disable ACK generation */
		pI2Cx->CR1 &= ~(1U << I2C_CR1_ACK);
	}
}

static void I2C_ClearADDRFlag(I2C_Handle_t* pI2CHandle, uint8_t IsMaster)
{
	uint32_t dummyRead;

	/*
	 * Master receiver with only one byte:
	 *
	 * ACK must be disabled before clearing ADDR.
	 * Once ADDR is cleared, SCL is released and reception begins.
	 */
	if (IsMaster != 0U)
	{
		if ((pI2CHandle->TxRxState == I2C_BUSY_IN_RX) &&
			(pI2CHandle->RxSize == 1U))
		{
			I2C_ManageAcking(pI2CHandle->pI2Cx,
							 I2C_ACK_DISABLE);
		}
	}

	/*
	 * Clear ADDR:
	 * read SR1 followed by read SR2.
	 */
	dummyRead = pI2CHandle->pI2Cx->SR1;
	dummyRead = pI2CHandle->pI2Cx->SR2;
	(void)dummyRead;
}

static void I2C_GenerateStopCondition(I2C_RegStruct_t* pI2Cx)
{
	pI2Cx->CR1 |= (1U << I2C_CR1_STOP);
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t* pI2CHandle)
{
	/*
	 * TXE means DR is empty and can accept another byte.
	 * It does not mean the entire byte transfer is finished.
	 */
	if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
	{
		if (pI2CHandle->TxLen > 0U)
		{
			/* Load one byte into the data register */
			pI2CHandle->pI2Cx->DR =
					(uint32_t)(*(pI2CHandle->pTxBuffer));

			/* Move to the next byte */
			pI2CHandle->pTxBuffer++;

			/* One fewer byte remains */
			pI2CHandle->TxLen--;
		}
	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t* pI2CHandle)
{
	/*
	 * Execute only when the application is currently
	 * receiving data.
	 */
	if (pI2CHandle->TxRxState != I2C_BUSY_IN_RX)
	{
		return;
	}

	/*
	 * Case 1: originally requested only one byte.
	 *
	 * ACK was already disabled before ADDR was cleared.
	 */
	if (pI2CHandle->RxSize == 1U)
	{
		if (pI2CHandle->RxLen > 0U)
		{
			*(pI2CHandle->pRxBuffer) =
					(uint8_t)pI2CHandle->pI2Cx->DR;

			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
		}
	}

	/*
	 * Case 2: originally requested more than one byte.
	 */
	else if (pI2CHandle->RxSize > 1U)
	{
		if (pI2CHandle->RxLen > 0U)
		{
			/*
			 * When two bytes remain, disable ACK.
			 *
			 * This prepares the hardware to NACK the final byte.
			 */
			if (pI2CHandle->RxLen == 2U)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx,
								 I2C_ACK_DISABLE);
			}

			/* Read the received byte from DR */
			*(pI2CHandle->pRxBuffer) =
					(uint8_t)pI2CHandle->pI2Cx->DR;

			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
		}
	}

	/*
	 * All requested bytes have been received.
	 */
	if (pI2CHandle->RxLen == 0U)
	{
		/*
		 * Generate STOP only when repeated START
		 * was not requested.
		 */
		if (pI2CHandle->Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(
					pI2CHandle->pI2Cx);
		}

		I2C_CloseReceiveData(pI2CHandle);

		I2C_ApplicationEventCallback(
				pI2CHandle,
				I2C_EV_RX_CMPLT);
	}
}

static void I2C_CloseSendData(I2C_Handle_t* pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITERREN);
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0U;
	pI2CHandle->TxRxState = I2C_READY;
}

static void I2C_CloseReceiveData(I2C_Handle_t* pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITERREN);
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0U;
	pI2CHandle->RxSize = 0U;
	pI2CHandle->TxRxState = I2C_READY;

	if (pI2CHandle->I2C_Config.I2C_ACKControl ==
		I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,
						 I2C_ACK_ENABLE);
	}
}


/*--------------------------------------------------------------------------------------------------------------------------------------------*/

/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegStruct_t* pI2Cx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pI2Cx == I2C1)
		{
			I2C1_CLOCK_EN();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_CLOCK_EN();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_CLOCK_EN();
		}
		else
		{
			/* Invalid I2C peripheral pointer */
		}
	}
	else
	{
		if (pI2Cx == I2C1)
		{
			I2C1_CLOCK_DI();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_CLOCK_DI();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_CLOCK_DI();
		}
		
	}
}


/*
 * De-initialize I2C peripheral
 */
void I2C_DeInit(I2C_RegStruct_t* pI2Cx)
{
	if (pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if (pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if (pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
	
}

void I2C_Init(I2C_Handle_t* pI2CHandle)
{
	uint32_t tempreg = 0U;
	uint32_t pclk1 = 0U;
	uint16_t ccr_value = 0U;

	/*
	 * 1. Configure ACK control bit in I2C_CR1
	 *
	 * ACK is bit 10.
	 */
	tempreg = 0U;

	tempreg |=
		((uint32_t)pI2CHandle->I2C_Config.I2C_ACKControl << 10U);

	pI2CHandle->pI2Cx->CR1 = tempreg;


	/*
	 * 2. Configure FREQ field in I2C_CR2
	 *
	 * CR2.FREQ stores PCLK1 frequency in MHz,
	 * not the desired I2C SCL frequency.
	 */
	tempreg = 0U;

	pclk1 = RCC_GetPCLK1FreqValue();

	tempreg |= (pclk1 / 1000000U);

	/*
	 * FREQ field occupies CR2 bits [5:0].
	 */
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3FU);


	/*
	 * 3. Configure device own address in I2C_OAR1
	 *
	 * ADDMODE = 0: 7-bit addressing
	 * Own address is written starting from bit 1.
	 * Bit 14 must always be kept at 1.
	 */
	tempreg = 0U;

	tempreg |=
		((uint32_t)pI2CHandle->I2C_Config.I2C_DeviceAddress << 1U);

	tempreg |= (1U << 14U);

	pI2CHandle->pI2Cx->OAR1 = tempreg;


	/*
	 * 4. Configure CCR register
	 */
	tempreg = 0U;

	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		/*
		 * Standard mode:
		 *
		 * SCL frequency = PCLK1 / (2 * CCR)
		 *
		 * Therefore:
		 * CCR = PCLK1 / (2 * SCL frequency)
		 */
		ccr_value =
			(uint16_t)(pclk1 /
				(2U * pI2CHandle->I2C_Config.I2C_SCLSpeed));

		/*
		 * CCR field occupies bits [11:0].
		 * F/S bit 15 remains 0 for Standard mode.
		 */
		tempreg |= ((uint32_t)ccr_value & 0xFFFU);
	}
	else
	{
		/*
		 * Fast mode:
		 * Set F/S bit 15.
		 */
		tempreg |= (1U << 15U);

		/*
		 * Configure DUTY bit 14.
		 */
		tempreg |=
			((uint32_t)pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14U);

		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle ==
			I2C_FM_DUTY_2)
		{
			/*
			 * Fast mode, DUTY = 0:
			 *
			 * Tlow / Thigh = 2
			 * SCL frequency = PCLK1 / (3 * CCR)
			 */
			ccr_value =
				(uint16_t)(pclk1 /
					(3U *
						pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			/*
			 * Fast mode, DUTY = 1:
			 *
			 * Tlow / Thigh = 16 / 9
			 * SCL frequency = PCLK1 / (25 * CCR)
			 */
			ccr_value =
				(uint16_t)(pclk1 /
					(25U *
						pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}

		tempreg |= ((uint32_t)ccr_value & 0xFFFU);
	}

	pI2CHandle->pI2Cx->CCR = tempreg;


	/*
	 * 5. Configure TRISE register
	 */
	tempreg = 0U;

	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		/*
		 * Standard mode maximum rise time = 1000 ns.
		 *
		 * TRISE = PCLK1 frequency in MHz + 1
		 */
		tempreg = (pclk1 / 1000000U) + 1U;
	}
	else
	{
		/*
		 * Fast mode maximum rise time = 300 ns.
		 *
		 * TRISE = (PCLK1 in MHz * 300 ns / 1000 ns) + 1
		 */
		tempreg =
			(((pclk1 / 1000000U) * 300U) / 1000U) + 1U;
	}

	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3FU);
}


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t* pI2CHandle,uint8_t* pTxBuffer,uint32_t Len,uint8_t SlaveAddr)
{
	/*
	 * 1. Generate the START condition
	 */
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	/*
	 * 2. Confirm that START generation is completed
	 * by checking the SB flag in SR1.
	 *
	 * Note: Until SB is cleared, SCL will be stretched (pulled LOW).
	 */
	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB))
	{
	}

	/*
	 * 3. Send the address of the slave with r/w bit set to write (0)
	 */
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr, I2C_WRITE);

	/*
	 * 4. Confirm that address phase is completed
	 * by checking the ADDR flag in SR1.
	 */
	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR))
	{
	}

	/*
	 * 5. Clear the ADDR flag according to its software sequence.
	 *
	 * Note: Until ADDR is cleared, SCL will be stretched (pulled LOW).
	 */
	I2C_ClearADDRFlag(pI2CHandle, 1U);

	/*
	 * 6. Send the data until Len becomes 0
	 */
	while (Len > 0U)
	{
		while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE))
		{
		}

		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	/*
	 * 7. When Len becomes zero, wait for TXE = 1 and BTF = 1
	 * before generating the STOP condition.
	 *
	 * Note: TXE = 1 and BTF = 1 means both shift register and
	 * data register are empty.
	 * When BTF = 1, SCL will be stretched (pulled LOW).
	 */
	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE))
	{
	}

	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF))
	{
	}

	/*
	 * 8. Generate STOP condition.
	 * Master need not wait for completion of STOP condition.
	 *
	 * Note: Generating STOP automatically clears the BTF flag.
	 */
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}





void I2C_MasterReceiveData(I2C_Handle_t* pI2CHandle,uint8_t* pRxBuffer,uint8_t Len,uint8_t SlaveAddr)
{
	/* 1. Generate the START condition */
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	/*
	 * 2. Wait until START generation is completed.
	 *
	 * SB becomes 1 after the START condition has been generated.
	 * Until SB is cleared, the I2C peripheral may stretch SCL LOW.
	 */
	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB))
	{
		/* Wait */
	}

	/*
	 * 3. Send slave address with R/W bit = 1.
	 *
	 * Your shared address-phase helper is used here with I2C_READ.
	 */
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx,
		SlaveAddr,
		I2C_READ);

	/*
	 * 4. Wait until address phase is completed.
	 *
	 * ADDR becomes 1 after the slave acknowledges its address.
	 */
	while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR))
	{
		/* Wait */
	}

	/* Procedure to read only one byte from slave */
	if (pI2CHandle->RxSize == 1U)

		/* Clear ADDR by reading SR1 followed by SR2 */
		I2C_ClearADDRFlag(pI2CHandle, 1U);
	{
		/*
		 * Disable ACK before clearing ADDR.
		 *
		 * This causes the master to NACK the one and only byte,
		 * telling the slave that no more bytes are required.
		 */
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		/*
		 * Generate STOP before clearing ADDR, following the
		 * single-byte receive sequence shown in the lecture.
		 */
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);


		/* Wait until receive data register is not empty */
		while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE))
		{
			/* Wait */
		}

		/* Read the received byte */
		*pRxBuffer = (uint8_t)pI2CHandle->pI2Cx->DR;

		/*
		 * Restore ACK configuration before leaving the API.
		 * This prepares the peripheral for the next receive operation.
		 */
		if (pI2CHandle->I2C_Config.I2C_ACKControl ==
			I2C_ACK_ENABLE)
		{
			I2C_ManageAcking(pI2CHandle->pI2Cx,
				I2C_ACK_ENABLE);
		}

		return;
	}

	/* Procedure to read more than one byte from slave */
	if (pI2CHandle->RxSize > 1U)
	{
		/*
		 * ACK must normally be enabled so that the master acknowledges
		 * all received bytes except the final byte.
		 */
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);

		/* Clear ADDR and begin the data phase */
		I2C_ClearADDRFlag(pI2CHandle, 1U);

		/* Read until all requested bytes have been received */
		for (uint32_t i = Len; i > 0U; i--)
		{
			/* Wait until a received byte is available */
			while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx,
				I2C_FLAG_RXNE))
			{
				/* Wait */
			}

			/*
			 * When only two bytes remain, disable ACK before reading
			 * the second-last byte.
			 *
			 * After the second-last byte is read, the final byte will
			 * be received with NACK, ending the receive operation.
			 */
			if (i == 2U)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx,
					I2C_ACK_DISABLE);

				/* Generate STOP for the end of the transaction */
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			/* Read the received byte into the application buffer */
			*pRxBuffer = (uint8_t)pI2CHandle->pI2Cx->DR;

			/* Move to the next buffer location */
			pRxBuffer++;
		}
	}

	/*
	 * Re-enable ACK if ACK was enabled in the original I2C
	 * configuration.
	 */
	if (pI2CHandle->I2C_Config.I2C_ACKControl ==
		I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,
			I2C_ACK_ENABLE);
	}
}

uint8_t I2C_MasterSendData_IT(I2C_Handle_t* pI2CHandle,
	uint8_t* pTxBuffer,
	uint32_t Len,
	uint8_t SlaveAddr,
	uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if ((busystate != I2C_BUSY_IN_TX) &&
		(busystate != I2C_BUSY_IN_RX))
	{
		/*
		 * Store transfer information in the handle.
		 * The interrupt handlers will use these values later.
		 */
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		/*
		 * Generate START condition.
		 *
		 * When START is generated successfully, SB becomes 1.
		 * The event ISR will then send the slave address.
		 */
		pI2CHandle->pI2Cx->CR1 |=
			(1U << I2C_CR1_START);

		/*
		 * Enable TxE and RxNE buffer interrupts.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITBUFEN);

		/*
		 * Enable event interrupts:
		 * SB, ADDR, BTF, STOPF, etc.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITEVTEN);

		/*
		 * Enable error interrupts:
		 * BERR, ARLO, AF, OVR, TIMEOUT, etc.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITERREN);
	}

	return busystate;
}

uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t* pI2CHandle,
	uint8_t* pRxBuffer,
	uint32_t Len,
	uint8_t SlaveAddr,
	uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if ((busystate != I2C_BUSY_IN_TX) &&
		(busystate != I2C_BUSY_IN_RX))
	{
		/*
		 * Store transfer information in the handle.
		 * The interrupt handlers will use these values later.
		 */
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;

		/*
		 * RxLen will decrease during reception.
		 * RxSize preserves the original requested length.
		 */
		pI2CHandle->RxSize = Len;

		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		/*
		 * Generate START condition.
		 *
		 * The event ISR will later send:
		 * slave address + read bit.
		 */
		pI2CHandle->pI2Cx->CR1 |=
			(1U << I2C_CR1_START);

		/*
		 * Enable TxE and RxNE buffer interrupts.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITBUFEN);

		/*
		 * Enable event interrupts:
		 * SB, ADDR, BTF, STOPF, etc.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITEVTEN);

		/*
		 * Enable error interrupts:
		 * BERR, ARLO, AF, OVR, TIMEOUT, etc.
		 */
		pI2CHandle->pI2Cx->CR2 |=
			(1U << I2C_CR2_ITERREN);
	}

	return busystate;
}



/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	//low bits are not implemented while only high bits are implemented

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);

}

void I2C_SlaveSendData(I2C_RegStruct_t* pI2Cx, uint8_t Data)
{
	pI2Cx->DR = Data;
}

uint8_t I2C_SlaveReceiveData(I2C_RegStruct_t* pI2Cx)
{
	return (uint8_t)pI2Cx->DR;
}

void I2C_EV_IRQHandling(I2C_Handle_t* pI2CHandle)
{
	uint32_t temp1;
	uint32_t temp2;
	uint32_t temp3;

	uint8_t isMaster;

	I2C_RegStruct_t* pI2Cx = pI2CHandle->pI2Cx;

	/*
	 * IMPORTANT:
	 *
	 * Read MSL before reading SR1.
	 *
	 * ADDR is cleared by reading SR1 followed by SR2.
	 * Therefore, reading SR2 after checking ADDR in SR1 could
	 * accidentally complete the ADDR-clear sequence too early.
	 */
	isMaster =
		((pI2Cx->SR2 & (1U << I2C_SR2_MSL)) != 0U)
		? 1U
		: 0U;

	/*
	 * Read interrupt-enable bits.
	 */
	temp1 = pI2Cx->CR2 & (1U << I2C_CR2_ITEVTEN);
	temp2 = pI2Cx->CR2 & (1U << I2C_CR2_ITBUFEN);


	/**********************************************************
	 * 1. SB event
	 *
	 * Applicable only in master mode.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_SB);

	if ((temp1 != 0U) && (temp3 != 0U))
	{
		/*
		 * START generation succeeded.
		 * Execute the address phase based on application state.
		 */
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhase(
					pI2Cx,
					pI2CHandle->DevAddr,
					I2C_WRITE);
		}
		else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhase(
					pI2Cx,
					pI2CHandle->DevAddr,
					I2C_READ);
		}
	}


	/**********************************************************
	 * 2. ADDR event
	 *
	 * Master mode:
	 *   Address was sent and acknowledged.
	 *
	 * Slave mode:
	 *   Own address matched.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_ADDR);

	if ((temp1 != 0U) && (temp3 != 0U))
	{
		I2C_ClearADDRFlag(pI2CHandle, isMaster);
	}


	/**********************************************************
	 * 3. BTF event
	 *
	 * During master transmission:
	 * BTF + TXE + TxLen == 0 means DR and shift register
	 * are empty and the transmission can be closed.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_BTF);

	if ((temp1 != 0U) && (temp3 != 0U))
	{
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			/*
			 * Confirm TXE is also set.
			 */
			if ((pI2Cx->SR1 &
				(1U << I2C_SR1_TXE)) != 0U)
			{
				/*
				 * Close only after every application byte
				 * has been loaded into the peripheral.
				 */
				if (pI2CHandle->TxLen == 0U)
				{
					if (pI2CHandle->Sr ==
						I2C_DISABLE_SR)
					{
						I2C_GenerateStopCondition(
								pI2Cx);
					}

					I2C_CloseSendData(
							pI2CHandle);

					I2C_ApplicationEventCallback(
							pI2CHandle,
							I2C_EV_TX_CMPLT);
				}
			}
		}

		/*
		 * Nothing is done here for master reception
		 * in this lecture implementation.
		 */
	}


	/**********************************************************
	 * 4. STOPF event
	 *
	 * STOPF applies only to slave mode.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_STOPF);

	if ((temp1 != 0U) && (temp3 != 0U) && (isMaster == 0U))
	{
		/*
		 * STOPF clear sequence:
		 *
		 * 1. Read SR1 -- already done above.
		 * 2. Write CR1.
		 *
		 * OR-ing with zero preserves the register contents,
		 * but still performs a volatile CR1 write.
		 */
		pI2Cx->CR1 |= 0U;

		I2C_ApplicationEventCallback(
				pI2CHandle,
				I2C_EV_STOP);
	}


	/**********************************************************
	 * 5. TXE event
	 *
	 * Requires both ITEVTEN and ITBUFEN.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_TXE);

	if ((temp1 != 0U) &&
		(temp2 != 0U) &&
		(temp3 != 0U))
	{
		/*
		 * This implementation handles master transmission.
		 */
		if (isMaster != 0U)
		{
			I2C_MasterHandleTXEInterrupt(
					pI2CHandle);
		}
		else
		{
			/*
			 * Slave transmitter mode:
			 * TXE means the master is requesting data.
			 */
			if ((pI2Cx->SR2 & (1U << I2C_SR2_TRA)) != 0U)
			{
				I2C_ApplicationEventCallback(
						pI2CHandle,
						I2C_EV_DATA_REQ);
			}
		}
	}


	/**********************************************************
	 * 6. RXNE event
	 *
	 * Requires both ITEVTEN and ITBUFEN.
	 **********************************************************/

	temp3 = pI2Cx->SR1 & (1U << I2C_SR1_RXNE);

	if ((temp1 != 0U) &&
		(temp2 != 0U) &&
		(temp3 != 0U))
	{
		/*
		 * This implementation handles master reception.
		 */
		if (isMaster != 0U)
		{
			I2C_MasterHandleRXNEInterrupt(
					pI2CHandle);
		}
		else
		{
			/*
			 * Slave receiver mode:
			 * RXNE means the master is sending data.
			 */
			if ((pI2Cx->SR2 & (1U << I2C_SR2_TRA)) == 0U)
			{
				I2C_ApplicationEventCallback(
						pI2CHandle,
						I2C_EV_DATA_RCV);
			}
		}
	}
}

void I2C_ER_IRQHandling(I2C_Handle_t* pI2CHandle)
{
	uint32_t temp1;
	uint32_t temp2;

	/* Know the status of ITERREN control bit in CR2 */
	temp2 = pI2CHandle->pI2Cx->CR2 & (1U << I2C_CR2_ITERREN);


	/*********************** Check for bus error ************************************/
	temp1 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_BERR);

	if (temp1 && temp2)
	{
		/* Clear the bus error flag */
		pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_BERR);

		/* Notify the application about the error */
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}


	/*********************** Check for arbitration lost error ************************************/
	temp1 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_ARLO);

	if (temp1 && temp2)
	{
		/* Clear the arbitration lost error flag */
		pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_ARLO);

		/* Notify the application about the error */
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}


	/*********************** Check for ACK failure error ************************************/
	temp1 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_AF);

	if (temp1 && temp2)
	{
		/* Clear the ACK failure error flag */
		pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_AF);

		/* Notify the application about the error */
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}


	/*********************** Check for overrun/underrun error ************************************/
	temp1 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_OVR);

	if (temp1 && temp2)
	{
		/* Clear the overrun/underrun error flag */
		pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_OVR);

		/* Notify the application about the error */
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}


	/*********************** Check for timeout error ************************************/
	temp1 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_TIMEOUT);

	if (temp1 && temp2)
	{
		/* Clear the timeout error flag */
		pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_TIMEOUT);

		/* Notify the application about the error */
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}
}


/*
 * Application callback
 */
weak void I2C_ApplicationEventCallback(I2C_Handle_t* pI2CHandle, uint8_t AppEv)
{
	(void)pI2CHandle;
	(void)AppEv;
}


#endif 
