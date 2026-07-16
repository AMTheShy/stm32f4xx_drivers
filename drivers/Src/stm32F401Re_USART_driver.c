#ifndef stm32F401Re_USART_driver.c
#define stm32F401Re_USART_driver.c

#include "stm32F401Re_USART_driver.h"
#include "stm32F401Re_rcc_driver.h"

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegStruct_t* pUSARTx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pUSARTx == USART1)
		{
			USART1_CLOCK_EN();
		}
		else if (pUSARTx == USART2)
		{
			USART2_CLOCK_EN();
		}
		else if (pUSARTx == USART6)
		{
			USART6_CLOCK_EN();
		}
	}
	else
	{
		if (pUSARTx == USART1)
		{
			USART1_CLOCK_DI();
		}
		else if (pUSARTx == USART2)
		{
			USART2_CLOCK_DI();
		}
		else if (pUSARTx == USART6)
		{
			USART6_CLOCK_DI();
		}
	}
}


/*
 * De-initialize USART peripheral
 */
void USART_DeInit(USART_RegStruct_t* pUSARTx)
{
	if (pUSARTx == USART1)
	{
		USART1_REG_RESET();
	}
	else if (pUSARTx == USART2)
	{
		USART2_REG_RESET();
	}
	else if (pUSARTx == USART6)
	{
		USART6_REG_RESET();
	}
}


/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             - Initializes the USART peripheral registers
 *                      using the configuration stored in the handle.
 *                      Configures USART_CR1, USART_CR2, and USART_CR3
 *                      for mode, word length, parity, stop bits,
 *                      and hardware flow control.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 *                      structure
 *
 * @return            - None
 *
 * @Note              - USART baud rate (BRR register) configuration
 *                      is covered in a later lecture.
 *                      USART enable (UE bit) is not set here.
 *                      Use USART_PeripheralControl() to enable the
 *                      peripheral after initialization.
 */
void USART_Init(USART_Handle_t* pUSARTHandle)
{
	uint32_t tempreg = 0U;

	/*
	 * Enable clock for the selected USART peripheral.
	 */
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	/******************************** Configuration of CR1 ******************************************/

	/*
	 * Enable USART Tx and Rx according to USART_Mode.
	 */
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempreg |= (1U << USART_CR1_RE);
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		tempreg |= (1U << USART_CR1_TE);
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TX_RX)
	{
		tempreg |= ((1U << USART_CR1_TE) | (1U << USART_CR1_RE));
	}

	/*
	 * Configure word length.
	 *
	 * M bit in USART_CR1:
	 * 0 = 8 data bits
	 * 1 = 9 data bits
	 */
	tempreg |= ((uint32_t)pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

	/*
	 * Configure parity control.
	 */
	if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EVEN)
	{
		/*
		 * Enable parity control.
		 * Even parity is selected by default when PS = 0.
		 */
		tempreg |= (1U << USART_CR1_PCE);
	}
	else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_ODD)
	{
		tempreg |= (1U << USART_CR1_PCE);
		tempreg |= (1U << USART_CR1_PS);
	}

	pUSARTHandle->pUSARTx->CR1 = tempreg;

	/******************************** Configuration of CR2 ******************************************/

	tempreg = 0U;

	/*
	 * Configure number of stop bits.
	 *
	 * STOP[1:0] field starts at bit position 12 in USART_CR2.
	 */
	tempreg |= ((uint32_t)pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);

	pUSARTHandle->pUSARTx->CR2 = tempreg;

	/******************************** Configuration of CR3 ******************************************/

	tempreg = 0U;

	/*
	 * Configure USART hardware flow control.
	 */
	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_CTS)
	{
		tempreg |= (1U << USART_CR3_CTSEN);
	}
	else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_RTS)
	{
		tempreg |= (1U << USART_CR3_RTSEN);
	}
	else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_RTS_CTS)
	{
		tempreg |= ((1U << USART_CR3_CTSEN) | (1U << USART_CR3_RTSEN));
	}

	pUSARTHandle->pUSARTx->CR3 = tempreg;

	/******************************** Configuration of BRR ******************************************/

	USART_SetBaudRate(pUSARTHandle->pUSARTx,
		pUSARTHandle->USART_Config.USART_Baud);
}


/*********************************************************************
 * @fn      		  - USART_SetBaudRate
 *
 * @brief             - Configures the USART baud rate by programming
 *                      the USART_BRR register based on the APB bus
 *                      clock and oversampling mode.
 *
 * @param[in]         - pUSARTx: Base address of the USART peripheral
 * @param[in]         - BaudRate: Desired baud rate in bits per second
 *
 * @return            - None
 *
 * @Note              - USART1 and USART6 use PCLK2; USART2 uses PCLK1.
 */
void USART_SetBaudRate(USART_RegStruct_t* pUSARTx, uint32_t BaudRate)
{
	uint32_t PCLKx;
	uint32_t usartdiv;
	uint32_t M_part;
	uint32_t F_part;
	uint32_t tempreg = 0U;

	if (pUSARTx == USART1 || pUSARTx == USART6)
	{
		PCLKx = RCC_GetPCLK2FreqValue();
	}
	else
	{
		PCLKx = RCC_GetPCLK1FreqValue();
	}

	if (pUSARTx->CR1 & (1U << USART_CR1_OVER8))
	{
		usartdiv = ((25U * PCLKx) / (2U * BaudRate));
	}
	else
	{
		usartdiv = ((25U * PCLKx) / (4U * BaudRate));
	}

	M_part = usartdiv / 100U;
	tempreg |= M_part << 4U;

	F_part = (usartdiv - (M_part * 100U));

	if (pUSARTx->CR1 & (1U << USART_CR1_OVER8))
	{
		F_part = (((F_part * 8U) + 50U) / 100U) & ((uint8_t)0x07);
	}
	else
	{
		F_part = (((F_part * 16U) + 50U) / 100U) & ((uint8_t)0x0F);
	}

	tempreg |= F_part;

	pUSARTx->BRR = tempreg;
}


/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             - Transmits data in blocking mode.
 *                      Waits for TXE before loading each byte/frame
 *                      into USART_DR, then waits for TC after the
 *                      last byte is sent.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 * @param[in]         - pTxBuffer: Pointer to the transmit buffer
 * @param[in]         - Len: Number of data frames to transmit
 *
 * @return            - None
 *
 * @Note              - Supports 8-bit and 9-bit word length and
 *                      parity-aware buffer pointer advancement.
 */
void USART_SendData(USART_Handle_t* pUSARTHandle,
	uint8_t* pTxBuffer,
	uint32_t Len)
{
	uint16_t* pdata;

	for (uint32_t i = 0U; i < Len; i++)
	{
		while (USART_GetFlagStatus(pUSARTHandle->pUSARTx,
			USART_FLAG_TXE) == FLAG_RESET)
		{
		}

		if (pUSARTHandle->USART_Config.USART_WordLength ==
			USART_WORDLEN_9BITS)
		{
			pdata = (uint16_t*)pTxBuffer;
			pUSARTHandle->pUSARTx->DR =
				(*pdata & (uint16_t)0x01FF);

			if (pUSARTHandle->USART_Config.USART_ParityControl ==
				USART_PARITY_NONE)
			{
				pTxBuffer += 2U;
			}
			else
			{
				pTxBuffer++;
			}
		}
		else
		{
			pUSARTHandle->pUSARTx->DR =
				(*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++;
		}
	}

	while (USART_GetFlagStatus(pUSARTHandle->pUSARTx,
		USART_FLAG_TC) == FLAG_RESET)
	{
	}
}


/*********************************************************************
 * @fn      		  - USART_ReceiveData
 *
 * @brief             - Receives data in blocking mode.
 *                      Waits for RXNE before reading each byte/frame
 *                      from USART_DR into the application buffer.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 * @param[in]         - pRxBuffer: Pointer to the receive buffer
 * @param[in]         - Len: Number of data frames to receive
 *
 * @return            - None
 *
 * @Note              - Supports 8-bit and 9-bit word length and
 *                      parity-aware data masking.
 */
void USART_ReceiveData(USART_Handle_t* pUSARTHandle,
	uint8_t* pRxBuffer,
	uint32_t Len)
{
	for (uint32_t i = 0U; i < Len; i++)
	{
		while (USART_GetFlagStatus(pUSARTHandle->pUSARTx,
			USART_FLAG_RXNE) == FLAG_RESET)
		{
		}

		if (pUSARTHandle->USART_Config.USART_WordLength ==
			USART_WORDLEN_9BITS)
		{
			if (pUSARTHandle->USART_Config.USART_ParityControl ==
				USART_PARITY_NONE)
			{
				*((uint16_t*)pRxBuffer) =
					(uint16_t)(pUSARTHandle->pUSARTx->DR &
						(uint16_t)0x01FF);
				pRxBuffer += 2U;
			}
			else
			{
				*pRxBuffer =
					(uint8_t)(pUSARTHandle->pUSARTx->DR &
						(uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else
		{
			if (pUSARTHandle->USART_Config.USART_ParityControl ==
				USART_PARITY_NONE)
			{
				*pRxBuffer =
					(uint8_t)(pUSARTHandle->pUSARTx->DR &
						(uint8_t)0xFF);
			}
			else
			{
				*pRxBuffer =
					(uint8_t)(pUSARTHandle->pUSARTx->DR &
						(uint8_t)0x7F);
			}

			pRxBuffer++;
		}
	}
}


/*********************************************************************
 * @fn      		  - USART_SendData_IT
 *
 * @brief             - Starts USART data transmission in interrupt
 *                      mode. Saves the application buffer and length,
 *                      marks the USART as busy in transmission, and
 *                      enables TXE and TC interrupts.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 * @param[in]         - pTxBuffer: Pointer to the transmit buffer
 * @param[in]         - Len: Number of data frames to transmit
 *
 * @return            - Previous USART transmission state
 *
 * @Note              - This is a non-blocking API.
 */
uint8_t USART_SendData_IT(USART_Handle_t* pUSARTHandle,
	uint8_t* pTxBuffer,
	uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if (txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TXEIE);
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TCIE);
	}

	return txstate;
}


/*********************************************************************
 * @fn      		  - USART_ReceiveData_IT
 *
 * @brief             - Starts USART data reception in interrupt mode.
 *                      Saves the application buffer and length, marks
 *                      the USART as busy in reception, and enables
 *                      the RXNE interrupt.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 * @param[in]         - pRxBuffer: Pointer to the receive buffer
 * @param[in]         - Len: Number of data frames to receive
 *
 * @return            - Previous USART reception state
 *
 * @Note              - This is a non-blocking API.
 */
uint8_t USART_ReceiveData_IT(USART_Handle_t* pUSARTHandle,
	uint8_t* pRxBuffer,
	uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if (rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_RXNEIE);
	}

	return rxstate;
}


/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	//low bits are not implemented while only high bits are implemented

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);
}


/*********************************************************************
 * @fn      		  - USART_IRQHandling
 *
 * @brief             - Handles USART interrupt events for TC, TXE,
 *                      RXNE, CTS, IDLE, overrun, and error flags.
 *
 * @param[in]         - pUSARTHandle: Pointer to the USART handle
 *
 * @return            - None
 *
 * @Note              - Called from the USART IRQ handler in the
 *                      application interrupt vector table.
 */
void USART_IRQHandling(USART_Handle_t* pUSARTHandle)
{
	uint32_t temp1;
	uint32_t temp2;
	uint32_t temp3;
	uint16_t* pdata;
	uint8_t* pTxBuffer;
	uint8_t* pRxBuffer;

	pTxBuffer = pUSARTHandle->pTxBuffer;
	pRxBuffer = pUSARTHandle->pRxBuffer;

	/*************************Check for TC flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_TC);
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_TCIE);

	if (temp1 && temp2)
	{
		if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			if (!pUSARTHandle->TxLen)
			{
				/*
				 * F401RE (RM0368): clear TC by reading SR
				 * then writing USART_DR.
				 */
				(void)pUSARTHandle->pUSARTx->SR;
				pUSARTHandle->pUSARTx->DR = 0U;
				pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_TCIE);

				pUSARTHandle->TxBusyState = USART_READY;
				pUSARTHandle->pTxBuffer = NULL;
				pUSARTHandle->TxLen = 0U;

				USART_ApplicationEventCallback(pUSARTHandle,
					USART_EVENT_TX_CMPLT);
			}
		}
	}

	/*************************Check for TXE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_TXE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_TXEIE);

	if (temp1 && temp2)
	{
		if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			if (pUSARTHandle->TxLen > 0U)
			{
				if (pUSARTHandle->USART_Config.USART_WordLength ==
					USART_WORDLEN_9BITS)
				{
					pdata = (uint16_t*)pTxBuffer;
					pUSARTHandle->pUSARTx->DR =
						(*pdata & (uint16_t)0x01FF);

					if (pUSARTHandle->USART_Config.USART_ParityControl ==
						USART_PARITY_NONE)
					{
						pTxBuffer += 2U;
						pUSARTHandle->TxLen--;
					}
					else
					{
						pTxBuffer++;
						pUSARTHandle->TxLen--;
					}
				}
				else
				{
					pUSARTHandle->pUSARTx->DR =
						(*pTxBuffer & (uint8_t)0xFF);
					pTxBuffer++;
					pUSARTHandle->TxLen--;
				}

				pUSARTHandle->pTxBuffer = pTxBuffer;
			}

			if (pUSARTHandle->TxLen == 0U)
			{
				pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_TXEIE);
			}
		}
	}

	/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_RXNEIE);

	if (temp1 && temp2)
	{
		if (pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
		{
			if (pUSARTHandle->RxLen > 0U)
			{
				if (pUSARTHandle->USART_Config.USART_WordLength ==
					USART_WORDLEN_9BITS)
				{
					if (pUSARTHandle->USART_Config.USART_ParityControl ==
						USART_PARITY_NONE)
					{
						*((uint16_t*)pRxBuffer) =
							(pUSARTHandle->pUSARTx->DR &
								(uint16_t)0x01FF);
						pRxBuffer += 2U;
						pUSARTHandle->RxLen--;
					}
					else
					{
						*pRxBuffer =
							(pUSARTHandle->pUSARTx->DR &
								(uint8_t)0xFF);
						pRxBuffer++;
						pUSARTHandle->RxLen--;
					}
				}
				else
				{
					if (pUSARTHandle->USART_Config.USART_ParityControl ==
						USART_PARITY_NONE)
					{
						*pRxBuffer =
							(uint8_t)(pUSARTHandle->pUSARTx->DR &
								(uint8_t)0xFF);
					}
					else
					{
						*pRxBuffer =
							(uint8_t)(pUSARTHandle->pUSARTx->DR &
								(uint8_t)0x7F);
					}

					pRxBuffer++;
					pUSARTHandle->RxLen--;
				}

				pUSARTHandle->pRxBuffer = pRxBuffer;
			}

			if (!pUSARTHandle->RxLen)
			{
				pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_RXNEIE);
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,
					USART_EVENT_RX_CMPLT);
			}
		}
	}

	/*************************Check for CTS flag ********************************************/
	/*
	 * CTS is supported on USART1, USART2, and USART6 of STM32F401RE.
	 * When CTSE is set, the CTS flag (SR bit 9) is cleared by
	 * software writing 0 to that bit (RM0368).
	 */

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_CTS);
	temp2 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_CTSEN);
	temp3 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_CTIE);

	if (temp1 && temp2 && temp3)
	{
		pUSARTHandle->pUSARTx->SR &= ~(1U << USART_SR_CTS);
		USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_CTS);
	}

	/*************************Check for IDLE detection flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_IDLE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_IDLEIE);

	if (temp1 && temp2)
	{
		/*
		 * F401RE (RM0368): clear IDLE by reading SR
		 * followed by reading DR.
		 */
		(void)pUSARTHandle->pUSARTx->SR;
		(void)pUSARTHandle->pUSARTx->DR;

		USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
	}

	/*************************Check for Overrun detection flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_ORE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_RXNEIE);

	if (temp1 && temp2)
	{
		USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_ORE);
	}

	/*************************Check for Error Flag ********************************************/

	temp2 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_EIE);

	if (temp2)
	{
		temp1 = pUSARTHandle->pUSARTx->SR;

		if (temp1 & (1U << USART_SR_FE))
		{
			USART_ApplicationEventCallback(pUSARTHandle,
				USART_ERREVENT_FE);
		}

		if (temp1 & (1U << USART_SR_NE))
		{
			USART_ApplicationEventCallback(pUSARTHandle,
				USART_ERREVENT_NE);
		}

		if (temp1 & (1U << USART_SR_ORE))
		{
			USART_ApplicationEventCallback(pUSARTHandle,
				USART_ERREVENT_ORE);
		}
	}
}


/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegStruct_t* pUSARTx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pUSARTx->CR1 |= (1U << USART_CR1_UE);
	}
	else
	{
		pUSARTx->CR1 &= ~(1U << USART_CR1_UE);
	}
}

uint8_t USART_GetFlagStatus(USART_RegStruct_t* pUSARTx, uint32_t FlagName)
{
	/*
	 * Check whether the requested flag bit is set in USART_SR.
	 */
	if (pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;
}


/*
 * Application callback
 */
weak void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandle, uint8_t AppEv)
{
	(void)pUSARTHandle;
	(void)AppEv;
}


#endif
