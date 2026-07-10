#ifndef INC_STM32F401RE_SPI_DRIVER.H
#define INC_STM32F401RE_SPI_DRIVER.H

#include "stm32F401Re.h"

/*
 * Configuration structure for SPIx peripheral
 */
typedef struct
{
    uint8_t SPI_DeviceMode;   /* Master or Slave */
    uint8_t SPI_BusConfig;    /* Full-duplex, half-duplex, simplex RX-only */
    uint8_t SPI_SclkSpeed;    /* SPI serial clock speed / baud-rate prescaler */
    uint8_t SPI_DFF;          /* Data frame format: 8-bit or 16-bit */
    uint8_t SPI_CPOL;         /* Clock polarity */
    uint8_t SPI_CPHA;         /* Clock phase */
    uint8_t SPI_SSM;          /* Software slave management enable/disable */
} SPI_Config_t;


/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
    SPI_RegStruct_t* pSPIx;
    SPI_Config_t SPIConfig;

    /* Interrupt-based transmission information */
    uint8_t* pTxBuffer;
    uint32_t TxLen;
    uint8_t  TxState;

    /* Interrupt-based reception information */
    uint8_t* pRxBuffer;
    uint32_t RxLen;
    uint8_t  RxState;

} SPI_Handle_t;

/*
 * SPI application states
 */
#define SPI_READY       0U
#define SPI_BUSY_IN_RX  1U
#define SPI_BUSY_IN_TX  2U

/*
 * SPI application events
 */
#define SPI_EVENT_TX_CMPLT       1U
#define SPI_EVENT_RX_CMPLT       2U
#define SPI_EVENT_OVR_ERR        3U
#define SPI_EVENT_CRC_ERR        4U

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_SLAVE              0U
#define SPI_DEVICE_MODE_MASTER             1U


 /*
  * @SPI_BusConfig
  */
#define SPI_BUS_CONFIG_FD                  1U   /* Full-duplex */
#define SPI_BUS_CONFIG_HD                  2U   /* Half-duplex */
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY      3U   /* Simplex receive-only */


  /*
   * @SPI_SclkSpeed
   * BR[2:0] baud rate control bits in SPI_CR1
   *
   * 000: fPCLK / 2
   * 001: fPCLK / 4
   * 010: fPCLK / 8
   * 011: fPCLK / 16
   * 100: fPCLK / 32
   * 101: fPCLK / 64
   * 110: fPCLK / 128
   * 111: fPCLK / 256
   */
#define SPI_SCLK_SPEED_DIV2                0U
#define SPI_SCLK_SPEED_DIV4                1U
#define SPI_SCLK_SPEED_DIV8                2U
#define SPI_SCLK_SPEED_DIV16               3U
#define SPI_SCLK_SPEED_DIV32               4U
#define SPI_SCLK_SPEED_DIV64               5U
#define SPI_SCLK_SPEED_DIV128              6U
#define SPI_SCLK_SPEED_DIV256              7U


   /*
    * @SPI_DFF
    */
#define SPI_DFF_8BITS                      0U
#define SPI_DFF_16BITS                     1U


    /*
     * @SPI_CPOL
     */
#define SPI_CPOL_LOW                       0U
#define SPI_CPOL_HIGH                      1U


     /*
      * @SPI_CPHA
      */
#define SPI_CPHA_LOW                       0U
#define SPI_CPHA_HIGH                      1U


      /*
       * @SPI_SSM
       */
#define SPI_SSM_DI                         0U
#define SPI_SSM_EN                         1U


       /*********************************************************************
         * Bit position definitions of SPI peripheral
         *********************************************************************/

         /*
          * SPI_CR1 bit position definitions
          */
#define SPI_CR1_CPHA        0U   /* Clock phase */
#define SPI_CR1_CPOL        1U   /* Clock polarity */
#define SPI_CR1_MSTR        2U   /* Master selection */

#define SPI_CR1_BR          3U   /* Baud rate control starts at bit 3: BR[2:0] */

#define SPI_CR1_SPE         6U   /* SPI peripheral enable */
#define SPI_CR1_LSBFIRST    7U   /* Frame format: MSB first or LSB first */

#define SPI_CR1_SSI         8U   /* Internal slave select */
#define SPI_CR1_SSM         9U   /* Software slave management */

#define SPI_CR1_RXONLY      10U  /* Receive only mode */
#define SPI_CR1_DFF         11U  /* Data frame format: 8-bit or 16-bit */

#define SPI_CR1_CRCNEXT     12U  /* CRC transfer next */
#define SPI_CR1_CRCEN       13U  /* Hardware CRC calculation enable */

#define SPI_CR1_BIDIOE      14U  /* Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE    15U  /* Bidirectional data mode enable */


          /*
           * SPI_CR2 bit position definitions
           */
#define SPI_CR2_RXDMAEN     0U   /* Rx buffer DMA enable */
#define SPI_CR2_TXDMAEN     1U   /* Tx buffer DMA enable */

#define SPI_CR2_SSOE        2U   /* SS output enable */

#define SPI_CR2_FRF         4U   /* Frame format */
#define SPI_CR2_ERRIE       5U   /* Error interrupt enable */
#define SPI_CR2_RXNEIE      6U   /* RX buffer not empty interrupt enable */
#define SPI_CR2_TXEIE       7U   /* TX buffer empty interrupt enable */


           /*
            * SPI_SR bit position definitions
            */
#define SPI_SR_RXNE         0U   /* Receive buffer not empty */
#define SPI_SR_TXE          1U   /* Transmit buffer empty */

#define SPI_SR_CHSIDE       2U   /* Channel side */
#define SPI_SR_UDR          3U   /* Underrun flag */
#define SPI_SR_CRCERR       4U   /* CRC error flag */
#define SPI_SR_MODF         5U   /* Mode fault */
#define SPI_SR_OVR          6U   /* Overrun flag */
#define SPI_SR_BSY          7U   /* Busy flag */
#define SPI_SR_FRE          8U   /* Frame format error */

            /*
             * SPI status flag definitions
             *
             * These are mask values for SPI_SR register.
             */
#define SPI_RXNE_FLAG     (1U << SPI_SR_RXNE)   /* Receive buffer not empty flag */
#define SPI_TXE_FLAG      (1U << SPI_SR_TXE)    /* Transmit buffer empty flag */
#define SPI_OVR_FLAG      (1U << SPI_SR_OVR)    /* Overrun flag */
#define SPI_BSY_FLAG      (1U << SPI_SR_BSY)    /* Busy flag */



/*********************************************************************
 * APIs supported by this driver
 *********************************************************************/

 /*
  * Peripheral Clock setup
  */
void SPI_PeriClockControl(SPI_RegStruct_t* pSPIx, uint8_t EnorDi);


/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t* pSPIHandle);
void SPI_DeInit(SPI_RegStruct_t* pSPIx);


/*
 * Data Send and Receive
 */
uint8_t SPI_GetFlagStatus(SPI_RegStruct_t* pSPIx, uint32_t FlagName);
void SPI_SendData(SPI_RegStruct_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegStruct_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len);
uint8_t SPI_SendData_IT(SPI_Handle_t* pSPIHandle, uint8_t* pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveData_IT(SPI_Handle_t* pSPIHandle, uint8_t* pRxBuffer, uint32_t Len);


/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t* pHandle);


/*
 * Other Peripheral Control APIs
 */
void SPI_PeripheralControl(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegStruct_t* pSPIx, uint8_t EnOrDi);
void SPI_ClearOVRFlag(SPI_RegStruct_t* pSPIx);
void SPI_CloseTransmission(SPI_Handle_t* pSPIHandle);
void SPI_CloseReception(SPI_Handle_t* pSPIHandle);


/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t* pSPIHandle, uint8_t AppEv);


#endif