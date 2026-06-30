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
    SPI_RegStruct_t* pSPIx;      /* Holds the base address of SPIx peripheral */
    SPI_Config_t SPIConfig;   /* Holds SPI configuration settings */

} SPI_Handle_t;

#endif