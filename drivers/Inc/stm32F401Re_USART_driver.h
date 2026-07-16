#ifndef INC_STM32F401RE_USART_DRIVER_H
#define INC_STM32F401RE_USART_DRIVER_H

#include "stm32F401Re.h"

/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
    uint8_t  USART_Mode;            /* TX only, RX only, or TX and RX */
    uint32_t USART_Baud;            /* Baud rate such as 9600, 115200, etc. */
    uint8_t  USART_NoOfStopBits;    /* 0.5, 1, 1.5, or 2 stop bits */
    uint8_t  USART_WordLength;      /* 8-bit or 9-bit data */
    uint8_t  USART_ParityControl;   /* None, even, or odd parity */
    uint8_t  USART_HWFlowControl;   /* None, CTS, RTS, or CTS+RTS */

} USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
    USART_RegStruct_t* pUSARTx;
    USART_Config_t     USART_Config;

    uint8_t*           pTxBuffer;
    uint8_t*           pRxBuffer;
    uint32_t           TxLen;
    uint32_t           RxLen;
    uint8_t            TxBusyState;
    uint8_t            RxBusyState;

} USART_Handle_t;


/*
 * @USART_Mode
 */
#define USART_MODE_ONLY_TX              0U
#define USART_MODE_ONLY_RX              1U
#define USART_MODE_TX_RX                2U

/*
 * USART application states
 */
#define USART_READY                     0U
#define USART_BUSY_IN_RX                1U
#define USART_BUSY_IN_TX                2U

/*
 * USART application events
 */
#define USART_EVENT_TX_CMPLT            1U
#define USART_EVENT_RX_CMPLT            2U
#define USART_EVENT_CTS                 3U
#define USART_EVENT_IDLE                4U
#define USART_EVENT_ORE                 5U
#define USART_ERREVENT_FE               6U
#define USART_ERREVENT_NE               7U
#define USART_ERREVENT_ORE              8U


/*
 * @USART_WordLength
 */
#define USART_WORDLEN_8BITS             0U
#define USART_WORDLEN_9BITS             1U


/*
 * @USART_NoOfStopBits
 *
 * These values are written into the STOP field of USART_CR2.
 */
#define USART_STOPBITS_1                0U
#define USART_STOPBITS_0_5              1U
#define USART_STOPBITS_2                2U
#define USART_STOPBITS_1_5              3U


/*
 * @USART_ParityControl
 */
#define USART_PARITY_NONE               0U
#define USART_PARITY_EVEN               1U
#define USART_PARITY_ODD                2U


/*
 * @USART_HWFlowControl
 */
#define USART_HWFLOW_NONE               0U
#define USART_HWFLOW_RTS                1U
#define USART_HWFLOW_CTS                2U
#define USART_HWFLOW_RTS_CTS            3U


/*
 * @USART_Baud
 * Standard baud rate options
 */
#define USART_STD_BAUD_1200               1200U
#define USART_STD_BAUD_2400               2400U
#define USART_STD_BAUD_9600               9600U
#define USART_STD_BAUD_19200              19200U
#define USART_STD_BAUD_38400              38400U
#define USART_STD_BAUD_57600              57600U
#define USART_STD_BAUD_115200             115200U
#define USART_STD_BAUD_230400             230400U
#define USART_STD_BAUD_460800             460800U
#define USART_STD_BAUD_921600             921600U
#define USART_STD_BAUD_2M                 2000000U
#define USART_STD_BAUD_3M                 3000000U


/*********************************************************************
 * Bit position definitions of USART peripheral
 *********************************************************************/

/*
 * USART_SR bit position definitions
 */
#define USART_SR_PE                     0U   /* Parity error */
#define USART_SR_FE                     1U   /* Framing error */
#define USART_SR_NE                     2U   /* Noise error flag */
#define USART_SR_ORE                    3U   /* Overrun error */
#define USART_SR_IDLE                   4U   /* IDLE line detected */
#define USART_SR_RXNE                   5U   /* Read data register not empty */
#define USART_SR_TC                     6U   /* Transmission complete */
#define USART_SR_TXE                    7U   /* Transmit data register empty */
#define USART_SR_LBD                    8U   /* LIN break detection flag */
#define USART_SR_CTS                    9U   /* CTS flag */


/*
 * USART_CR1 bit position definitions
 */
#define USART_CR1_SBK                   0U   /* Send break */
#define USART_CR1_RWU                   1U   /* Receiver wakeup */
#define USART_CR1_RE                    2U   /* Receiver enable */
#define USART_CR1_TE                    3U   /* Transmitter enable */
#define USART_CR1_IDLEIE                4U   /* IDLE interrupt enable */
#define USART_CR1_RXNEIE                5U   /* RXNE interrupt enable */
#define USART_CR1_TCIE                  6U   /* Transmission complete interrupt enable */
#define USART_CR1_TXEIE                 7U   /* TXE interrupt enable */
#define USART_CR1_PEIE                  8U   /* PE interrupt enable */
#define USART_CR1_PS                    9U   /* Parity selection */
#define USART_CR1_PCE                   10U  /* Parity control enable */
#define USART_CR1_WAKE                  11U  /* Wakeup method */
#define USART_CR1_M                     12U  /* Word length */
#define USART_CR1_UE                    13U  /* USART enable */
#define USART_CR1_OVER8                 15U  /* Oversampling mode */


/*
 * USART_CR2 bit position definitions
 */
#define USART_CR2_ADD                   0U   /* Address of the USART node */
#define USART_CR2_LBDL                  5U   /* LIN break detection length */
#define USART_CR2_LBDIE                 6U   /* LIN break detection interrupt enable */
#define USART_CR2_LBCL                  8U   /* Last bit clock pulse */
#define USART_CR2_CPOL                  9U   /* Clock polarity */
#define USART_CR2_CPHA                  10U  /* Clock phase */
#define USART_CR2_CLKEN                 11U  /* Clock enable */
#define USART_CR2_STOP                  12U  /* STOP bits */
#define USART_CR2_LINEN                 14U  /* LIN mode enable */


/*
 * USART_CR3 bit position definitions
 */
#define USART_CR3_EIE                   0U   /* Error interrupt enable */
#define USART_CR3_IREN                  1U   /* IrDA mode enable */
#define USART_CR3_IRLP                  2U   /* IrDA low-power */
#define USART_CR3_HALFSEL               3U   /* Half-duplex selection */
#define USART_CR3_NACK                  4U   /* Smartcard NACK enable */
#define USART_CR3_SCEN                  5U   /* Smartcard mode enable */
#define USART_CR3_DMAR                  6U   /* DMA enable receiver */
#define USART_CR3_DMAT                  7U   /* DMA enable transmitter */
#define USART_CR3_RTSEN                 8U   /* RTS enable */
#define USART_CR3_CTSEN                 9U   /* CTS enable (CTSE on F401RE) */
#define USART_CR3_CTIE                  10U  /* CTS interrupt enable (F401RE) */


/*
 * USART status flag definitions
 *
 * These are mask values for USART_SR register.
 */
#define USART_FLAG_PE                   (1U << USART_SR_PE)
#define USART_FLAG_FE                   (1U << USART_SR_FE)
#define USART_FLAG_NE                   (1U << USART_SR_NE)
#define USART_FLAG_ORE                  (1U << USART_SR_ORE)
#define USART_FLAG_IDLE                 (1U << USART_SR_IDLE)
#define USART_FLAG_RXNE                 (1U << USART_SR_RXNE)
#define USART_FLAG_TC                   (1U << USART_SR_TC)
#define USART_FLAG_TXE                  (1U << USART_SR_TXE)
#define USART_FLAG_LBD                  (1U << USART_SR_LBD)
#define USART_FLAG_CTS                  (1U << USART_SR_CTS)


/*********************************************************************
 * APIs supported by this driver
 *********************************************************************/

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegStruct_t* pUSARTx, uint8_t EnorDi);


/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t* pUSARTHandle);
void USART_DeInit(USART_RegStruct_t* pUSARTx);
void USART_SetBaudRate(USART_RegStruct_t* pUSARTx, uint32_t BaudRate);


/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len);
uint8_t USART_SendData_IT(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveData_IT(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len);


/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void USART_IRQHandling(USART_Handle_t* pHandle);


/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegStruct_t* pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegStruct_t* pUSARTx, uint32_t FlagName);


/*
 * Application callback
 */
weak void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandle, uint8_t AppEv);


#endif /* INC_STM32F401RE_USART_DRIVER_H */
