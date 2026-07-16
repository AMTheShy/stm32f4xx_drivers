#ifndef Inc_stm32F401Re_I2C_driver.h
#define Inc_stm32F401Re_I2C_driver.h

#include "stm32F401Re.h"
#include "stm32F401Re_rcc_driver.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
    uint32_t I2C_SCLSpeed;
    uint8_t  I2C_DeviceAddress;
    uint8_t  I2C_ACKControl;
    uint16_t I2C_FMDutyCycle;

} I2C_Config_t;


/*
 * Handle structure for I2Cx peripheral
 */
 
typedef struct
{
	I2C_RegStruct_t* pI2Cx;
	I2C_Config_t     I2C_Config;

	uint8_t*		 pTxBuffer;   /* Store application Tx buffer address */
	uint8_t*		 pRxBuffer;   /* Store application Rx buffer address */
	uint32_t         TxLen;       /* Store remaining Tx length */
	uint32_t         RxLen;       /* Store remaining Rx length */
	uint8_t          TxRxState;   /* Store current communication state */
	uint8_t          DevAddr;     /* Store slave/device address */
	uint32_t         RxSize;      /* Store original Rx size */
	uint8_t          Sr;          /* Store repeated-start value */

} I2C_Handle_t;


/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegStruct_t* pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t* pI2CHandle);
void I2C_DeInit(I2C_RegStruct_t* pI2Cx);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t* pI2CHandle,uint8_t* pTxBuffer,uint32_t Len,uint8_t SlaveAddr);

void I2C_MasterReceiveData(I2C_Handle_t* pI2CHandle, uint8_t* pRxBuffer, uint8_t Len, uint8_t SlaveAddr);

uint8_t I2C_MasterSendData_IT(I2C_Handle_t* pI2CHandle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t* pI2CHandle, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

void I2C_SlaveSendData(I2C_RegStruct_t* pI2Cx, uint8_t Data);

uint8_t I2C_SlaveReceiveData(I2C_RegStruct_t* pI2Cx);


/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t* pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t* pI2CHandle);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegStruct_t* pI2Cx, uint8_t EnorDi);



/*
 * Application callback
 */
weak void I2C_ApplicationEventCallback(I2C_Handle_t* pI2CHandle, uint8_t AppEv);

/*
 * I2C SCL speed configuration options
 */
#define I2C_SCL_SPEED_SM        100000U


 /*
  * I2C ACK control options
  */
#define I2C_ACK_DISABLE         0U
#define I2C_ACK_ENABLE          1U


  /*
   * I2C Fast-mode duty-cycle options
   */
#define I2C_FM_DUTY_2           0U
#define I2C_FM_DUTY_16_9        1U


/* I2C Read Or Write Data options

*/

#define I2C_READ                1U
#define I2C_WRITE               0U

/*
 * I2C repeated-start control options
 */
#define I2C_DISABLE_SR          0U
#define I2C_ENABLE_SR           1U

/*
 * I2C application states
 */
#define I2C_READY               0U
#define I2C_BUSY_IN_RX          1U
#define I2C_BUSY_IN_TX          2U

/*
 * I2C application events
 */
#define I2C_EV_TX_CMPLT         1U
#define I2C_EV_RX_CMPLT         2U
#define I2C_EV_STOP             8U
#define I2C_EV_DATA_RCV         9U
#define I2C_EV_DATA_REQ         10U

/*
 * I2C application error events
 */
#define I2C_ERROR_BERR          3U
#define I2C_ERROR_ARLO          4U
#define I2C_ERROR_AF            5U
#define I2C_ERROR_OVR           6U
#define I2C_ERROR_TIMEOUT       7U





/*I2C Register Bit position structure

*/


/* I2C_CR1 */
#define I2C_CR1_PE              0U
#define I2C_CR1_SMBUS           1U
#define I2C_CR1_SMBTYPE         3U
#define I2C_CR1_ENARP           4U
#define I2C_CR1_ENPEC           5U
#define I2C_CR1_ENGC            6U
#define I2C_CR1_NOSTRETCH       7U
#define I2C_CR1_START           8U
#define I2C_CR1_STOP            9U
#define I2C_CR1_ACK             10U
#define I2C_CR1_POS             11U
#define I2C_CR1_PEC             12U
#define I2C_CR1_ALERT           13U
#define I2C_CR1_SWRST           15U

/* I2C_CR2 */
#define I2C_CR2_FREQ            0U
#define I2C_CR2_ITERREN         8U
#define I2C_CR2_ITEVTEN         9U
#define I2C_CR2_ITBUFEN         10U
#define I2C_CR2_DMAEN           11U
#define I2C_CR2_LAST            12U

/* I2C_SR1 */
#define I2C_SR1_SB              0U
#define I2C_SR1_ADDR            1U
#define I2C_SR1_BTF             2U
#define I2C_SR1_ADD10           3U
#define I2C_SR1_STOPF           4U
#define I2C_SR1_RXNE            6U
#define I2C_SR1_TXE             7U
#define I2C_SR1_BERR            8U
#define I2C_SR1_ARLO            9U
#define I2C_SR1_AF              10U
#define I2C_SR1_OVR             11U
#define I2C_SR1_PECERR          12U
#define I2C_SR1_TIMEOUT         14U
#define I2C_SR1_SMBALERT        15U

/* I2C_SR2 */
#define I2C_SR2_MSL             0U
#define I2C_SR2_BUSY            1U
#define I2C_SR2_TRA             2U
#define I2C_SR2_GENCALL         4U
#define I2C_SR2_SMBDEFAULT      5U
#define I2C_SR2_SMBHOST         6U
#define I2C_SR2_DUALF           7U
#define I2C_SR2_PEC             8U

/* I2C_CCR */
#define I2C_CCR_CCR             0U
#define I2C_CCR_DUTY            14U
#define I2C_CCR_FS              15U

   /*
	* I2C status flag definitions
	* These macros create bit masks for the I2C status-register flags.
	*/

#define I2C_FLAG_TXE        (1U << I2C_SR1_TXE)
#define I2C_FLAG_RXNE       (1U << I2C_SR1_RXNE)
#define I2C_FLAG_SB         (1U << I2C_SR1_SB)
#define I2C_FLAG_OVR        (1U << I2C_SR1_OVR)
#define I2C_FLAG_AF         (1U << I2C_SR1_AF)
#define I2C_FLAG_ARLO       (1U << I2C_SR1_ARLO)
#define I2C_FLAG_BERR       (1U << I2C_SR1_BERR)
#define I2C_FLAG_STOPF      (1U << I2C_SR1_STOPF)
#define I2C_FLAG_ADD10      (1U << I2C_SR1_ADD10)
#define I2C_FLAG_BTF        (1U << I2C_SR1_BTF)
#define I2C_FLAG_ADDR       (1U << I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT    (1U << I2C_SR1_TIMEOUT)


	/*
	 * IRQ numbers of STM32F401xx MCU
	 */

	 /* I2C1 interrupts */
#define IRQ_NO_I2C1_EV          31U
#define IRQ_NO_I2C1_ER          32U

/* I2C2 interrupts */
#define IRQ_NO_I2C2_EV          33U
#define IRQ_NO_I2C2_ER          34U

/* I2C3 interrupts */
#define IRQ_NO_I2C3_EV          72U
#define IRQ_NO_I2C3_ER          73U


#endif 
