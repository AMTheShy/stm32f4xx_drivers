/*
 * stm32F4xx.h
 *
 *  Created on: Jun 27, 2026
 *      Author: yuank
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

#include<stdint.h>
#include <stddef.h>
#define vo volatile

#define weak   __attribute__((weak))

 /*
  * ARM Cortex-M4 processor specific details
  * ----------------------------------------------------------------------------------------------------------------------------------------------
  */

  /*
   * NVIC ISERx register addresses
   * ISER = Interrupt Set-Enable Register
   */

#define NVIC_ISER0        ((vo uint32_t*)0xE000E100)
#define NVIC_ISER1        ((vo uint32_t*)0xE000E104)
#define NVIC_ISER2        ((vo uint32_t*)0xE000E108)
#define NVIC_ISER3        ((vo uint32_t*)0xE000E10C)


   /*
    * NVIC ICERx register addresses
    * ICER = Interrupt Clear-Enable Register
    */

#define NVIC_ICER0        ((vo uint32_t*)0xE000E180)
#define NVIC_ICER1        ((vo uint32_t*)0xE000E184)
#define NVIC_ICER2        ((vo uint32_t*)0xE000E188)
#define NVIC_ICER3        ((vo uint32_t*)0xE000E18C)


#define NVIC_PR_BASEADDR    ((vo uint32_t*)0xE000E400U)

#define NO_PR_BITS_IMPLEMENTED    4


//--------------------------------------------------------------------------------------------------------------------------------------------






/*Base address of memories*/

#define FLASH_BASEADDR		0x08000000U

#define SRAM_BASEADDR		0x20000000U

#define ROM_BASEADDR		0x1FFF0000U


/*Base address of Communication Buses */

#define PERIPH_BASEADDR            0x40000000U

#define APB1_PERIPH_BASEADDR       PERIPH_BASEADDR

#define APB2_PERIPH_BASEADDR       0x40010000U

#define AHB1_PERIPH_BASEADDR       0x40020000U

#define AHB2_PERIPH_BASEADDR       0x50000000U

/* Base address of RCC*/

#define RCC_BASEADDR 			   (AHB1_PERIPH_BASEADDR + 0x3800U)


/* Base addresses of GPIO peripherals */

#define GPIOA_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x0000U)

#define GPIOB_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x0400U)

#define GPIOC_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x0800U)

#define GPIOD_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x0C00U)

#define GPIOE_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x1000U)

#define GPIOH_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x1C00U)

/* Base addresses of SPI peripherals */

#define SPI1_BASEADDR              (APB2_PERIPH_BASEADDR + 0x3000U)

#define SPI2_BASEADDR              (APB1_PERIPH_BASEADDR + 0x3800U)

#define SPI3_BASEADDR              (APB1_PERIPH_BASEADDR + 0x3C00U)

#define SPI4_BASEADDR              (APB2_PERIPH_BASEADDR + 0x3400U)


/* Base addresses of USART peripherals */

#define USART1_BASEADDR            (APB2_PERIPH_BASEADDR + 0x1000U)

#define USART2_BASEADDR            (APB1_PERIPH_BASEADDR + 0x4400U)

#define USART6_BASEADDR            (APB2_PERIPH_BASEADDR + 0x1400U)


/* Base addresses of I2C peripherals */

#define I2C1_BASEADDR              (APB1_PERIPH_BASEADDR + 0x5400U)

#define I2C2_BASEADDR              (APB1_PERIPH_BASEADDR + 0x5800U)

#define I2C3_BASEADDR              (APB1_PERIPH_BASEADDR + 0x5C00U)


/* Base addresses of EXTI and SYSCFG */

#define EXTI_BASEADDR              (APB2_PERIPH_BASEADDR + 0x3C00U)

#define SYSCFG_BASEADDR            (APB2_PERIPH_BASEADDR + 0x3800U)


/* GPIO Register Structure definition
 *
 */

typedef struct{

	vo uint32_t  MODER;

	vo uint32_t  OTYPER;

	vo uint32_t  OTSPEEDR;

	vo uint32_t  PUPDR;

	vo uint32_t  IDR;

	vo uint32_t  ODR;

	vo uint32_t  BSRR;

	vo uint32_t  LCKR;

	vo uint32_t  AFR[2];        //[0]-> low/[1]->high


}GPIO_RegStruct_t;

#define GPIOA        ((GPIO_RegStruct_t *)GPIOA_BASEADDR)

#define GPIOB        ((GPIO_RegStruct_t *)GPIOB_BASEADDR)

#define GPIOC        ((GPIO_RegStruct_t *)GPIOC_BASEADDR)

#define GPIOD        ((GPIO_RegStruct_t *)GPIOD_BASEADDR)

#define GPIOE        ((GPIO_RegStruct_t *)GPIOE_BASEADDR)

#define GPIOH        ((GPIO_RegStruct_t *)GPIOH_BASEADDR)

/* RCC Register Structure definition
 *
 */

typedef struct
{
    vo uint32_t CR;             /* 0x00: RCC clock control register */
    vo uint32_t PLLCFGR;        /* 0x04: RCC PLL configuration register */
    vo uint32_t CFGR;           /* 0x08: RCC clock configuration register */
    vo uint32_t CIR;            /* 0x0C: RCC clock interrupt register */

    vo uint32_t AHB1RSTR;       /* 0x10: AHB1 peripheral reset register */
    vo uint32_t AHB2RSTR;       /* 0x14: AHB2 peripheral reset register */
    uint32_t    RESERVED0[2];   /* 0x18 - 0x1C: Reserved */

    vo uint32_t APB1RSTR;       /* 0x20: APB1 peripheral reset register */
    vo uint32_t APB2RSTR;       /* 0x24: APB2 peripheral reset register */
    uint32_t    RESERVED1[2];   /* 0x28 - 0x2C: Reserved */

    vo uint32_t AHB1ENR;        /* 0x30: AHB1 peripheral clock enable register */
    vo uint32_t AHB2ENR;        /* 0x34: AHB2 peripheral clock enable register */
    uint32_t    RESERVED2[2];   /* 0x38 - 0x3C: Reserved */

    vo uint32_t APB1ENR;        /* 0x40: APB1 peripheral clock enable register */
    vo uint32_t APB2ENR;        /* 0x44: APB2 peripheral clock enable register */
    uint32_t    RESERVED3[2];   /* 0x48 - 0x4C: Reserved */

    vo uint32_t AHB1LPENR;      /* 0x50: AHB1 peripheral clock enable in low power mode */
    vo uint32_t AHB2LPENR;      /* 0x54: AHB2 peripheral clock enable in low power mode */
    uint32_t    RESERVED4[2];   /* 0x58 - 0x5C: Reserved */

    vo uint32_t APB1LPENR;      /* 0x60: APB1 peripheral clock enable in low power mode */
    vo uint32_t APB2LPENR;      /* 0x64: APB2 peripheral clock enable in low power mode */
    uint32_t    RESERVED5[2];   /* 0x68 - 0x6C: Reserved */

    vo uint32_t BDCR;           /* 0x70: Backup domain control register */
    vo uint32_t CSR;            /* 0x74: Clock control & status register */
    uint32_t    RESERVED6[2];   /* 0x78 - 0x7C: Reserved */

    vo uint32_t SSCGR;          /* 0x80: Spread spectrum clock generation register */
    vo uint32_t PLLI2SCFGR;     /* 0x84: PLLI2S configuration register */
    uint32_t    RESERVED7;      /* 0x88: Reserved */
    vo uint32_t DCKCFGR;        /* 0x8C: Dedicated clocks configuration register */

} RCC_RegStruct_t;

#define RCC        ((RCC_RegStruct_t *)RCC_BASEADDR)


/*EXTI Register Structure definition*/

typedef struct
{
    vo uint32_t IMR;        /* 0x00: Interrupt mask register */
    vo uint32_t EMR;        /* 0x04: Event mask register */
    vo uint32_t RTSR;       /* 0x08: Rising trigger selection register */
    vo uint32_t FTSR;       /* 0x0C: Falling trigger selection register */
    vo uint32_t SWIER;      /* 0x10: Software interrupt event register */
    vo uint32_t PR;         /* 0x14: Pending register */

} EXTI_RegStruct_t;

#define EXTI       ((EXTI_RegStruct_t *)EXTI_BASEADDR)


/* SYSCFG Register Structure definition
 *
 */

typedef struct
{
    vo uint32_t MEMRMP;          /* 0x00: SYSCFG memory remap register */
    vo uint32_t PMC;             /* 0x04: SYSCFG peripheral mode configuration register */
    vo uint32_t EXTICR[4];       /* 0x08 - 0x14: SYSCFG external interrupt configuration registers 1-4 */
    uint32_t    RESERVED0[2];    /* 0x18 - 0x1C: Reserved */
    vo uint32_t CMPCR;           /* 0x20: Compensation cell control register */

} SYSCFG_RegStruct_t;

#define SYSCFG       ((SYSCFG_RegStruct_t *)SYSCFG_BASEADDR)



/* SPI Register Structure definition
 *
 */

typedef struct
{
    vo uint32_t CR1;        /* 0x00: SPI control register 1 */
    vo uint32_t CR2;        /* 0x04: SPI control register 2 */
    vo uint32_t SR;         /* 0x08: SPI status register */
    vo uint32_t DR;         /* 0x0C: SPI data register */
    vo uint32_t CRCPR;      /* 0x10: SPI CRC polynomial register */
    vo uint32_t RXCRCR;     /* 0x14: SPI RX CRC register */
    vo uint32_t TXCRCR;     /* 0x18: SPI TX CRC register */
    vo uint32_t I2SCFGR;    /* 0x1C: SPI_I2S configuration register */
    vo uint32_t I2SPR;      /* 0x20: SPI_I2S prescaler register */

} SPI_RegStruct_t;


#define SPI1        ((SPI_RegStruct_t *)SPI1_BASEADDR)
#define SPI2        ((SPI_RegStruct_t *)SPI2_BASEADDR)
#define SPI3        ((SPI_RegStruct_t *)SPI3_BASEADDR)
#define SPI4        ((SPI_RegStruct_t *)SPI4_BASEADDR)



/*I2C Register Structure definition*/

typedef struct
{
    vo uint32_t CR1;       /* Offset 0x00 */
    vo uint32_t CR2;       /* Offset 0x04 */
    vo uint32_t OAR1;      /* Offset 0x08 */
    vo uint32_t OAR2;      /* Offset 0x0C */
    vo uint32_t DR;        /* Offset 0x10 */
    vo uint32_t SR1;       /* Offset 0x14 */
    vo uint32_t SR2;       /* Offset 0x18 */
    vo uint32_t CCR;       /* Offset 0x1C */
    vo uint32_t TRISE;     /* Offset 0x20 */
    vo uint32_t FLTR;      /* Offset 0x24 */

} I2C_RegStruct_t;

#define I2C1    ((I2C_RegStruct_t *)I2C1_BASEADDR)
#define I2C2    ((I2C_RegStruct_t *)I2C2_BASEADDR)
#define I2C3    ((I2C_RegStruct_t *)I2C3_BASEADDR)




/* Clock enable macros for GPIO peripherals */

#define GPIOA_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 0))
#define GPIOB_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 1))
#define GPIOC_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 2))
#define GPIOD_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 3))
#define GPIOE_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 4))
#define GPIOH_CLOCK_EN()      (RCC->AHB1ENR |= (1U << 7))


/* Clock enable macros for SPI peripherals */

#define SPI1_CLOCK_EN()       (RCC->APB2ENR |= (1U << 12))
#define SPI2_CLOCK_EN()       (RCC->APB1ENR |= (1U << 14))
#define SPI3_CLOCK_EN()       (RCC->APB1ENR |= (1U << 15))
#define SPI4_CLOCK_EN()       (RCC->APB2ENR |= (1U << 13))


/* Clock enable macros for USART peripherals */

#define USART1_CLOCK_EN()     (RCC->APB2ENR |= (1U << 4))
#define USART2_CLOCK_EN()     (RCC->APB1ENR |= (1U << 17))
#define USART6_CLOCK_EN()     (RCC->APB2ENR |= (1U << 5))


/* Clock enable macros for I2C peripherals */

#define I2C1_CLOCK_EN()       (RCC->APB1ENR |= (1U << 21))
#define I2C2_CLOCK_EN()       (RCC->APB1ENR |= (1U << 22))
#define I2C3_CLOCK_EN()       (RCC->APB1ENR |= (1U << 23))


/* Clock enable macro for SYSCFG peripheral */

#define SYSCFG_CLOCK_EN()     (RCC->APB2ENR |= (1U << 14))


/* Clock disable macros for GPIO peripherals */

#define GPIOA_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 0))
#define GPIOB_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 1))
#define GPIOC_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 2))
#define GPIOD_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 3))
#define GPIOE_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 4))
#define GPIOH_CLOCK_DI()      (RCC->AHB1ENR &= ~(1U << 7))


/* Clock disable macros for SPI peripherals */

#define SPI1_CLOCK_DI()       (RCC->APB2ENR &= ~(1U << 12))
#define SPI2_CLOCK_DI()       (RCC->APB1ENR &= ~(1U << 14))
#define SPI3_CLOCK_DI()       (RCC->APB1ENR &= ~(1U << 15))
#define SPI4_CLOCK_DI()       (RCC->APB2ENR &= ~(1U << 13))


/* Clock disable macros for USART peripherals */

#define USART1_CLOCK_DI()     (RCC->APB2ENR &= ~(1U << 4))
#define USART2_CLOCK_DI()     (RCC->APB1ENR &= ~(1U << 17))
#define USART6_CLOCK_DI()     (RCC->APB2ENR &= ~(1U << 5))


/* Clock disable macros for I2C peripherals */

#define I2C1_CLOCK_DI()       (RCC->APB1ENR &= ~(1U << 21))
#define I2C2_CLOCK_DI()       (RCC->APB1ENR &= ~(1U << 22))
#define I2C3_CLOCK_DI()       (RCC->APB1ENR &= ~(1U << 23))


/* Clock disable macro for SYSCFG peripheral */

#define SYSCFG_CLOCK_DI()     (RCC->APB2ENR &= ~(1U << 14))


/*
 * GPIO peripheral reset macros
 *
 * To reset a GPIO peripheral:
 * 1. Set the corresponding bit in RCC_AHB1RSTR
 * 2. Clear the same bit again
 *
 * Do not keep the bit set, otherwise the peripheral stays in reset state.
 */

#define GPIOA_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 0));  (RCC->AHB1RSTR &= ~(1U << 0));  } while (0)
#define GPIOB_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 1));  (RCC->AHB1RSTR &= ~(1U << 1));  } while (0)
#define GPIOC_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 2));  (RCC->AHB1RSTR &= ~(1U << 2));  } while (0)
#define GPIOD_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 3));  (RCC->AHB1RSTR &= ~(1U << 3));  } while (0)
#define GPIOE_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 4));  (RCC->AHB1RSTR &= ~(1U << 4));  } while (0)
#define GPIOH_REG_RESET()    do { (RCC->AHB1RSTR |=  (1U << 7));  (RCC->AHB1RSTR &= ~(1U << 7));  } while (0)

#define SPI1_REG_RESET()   do { (RCC->APB2RSTR |=  (1U << 12)); (RCC->APB2RSTR &= ~(1U << 12)); } while(0)
#define SPI2_REG_RESET()   do { (RCC->APB1RSTR |=  (1U << 14)); (RCC->APB1RSTR &= ~(1U << 14)); } while(0)
#define SPI3_REG_RESET()   do { (RCC->APB1RSTR |=  (1U << 15)); (RCC->APB1RSTR &= ~(1U << 15)); } while(0)
#define SPI4_REG_RESET()   do { (RCC->APB2RSTR |=  (1U << 13)); (RCC->APB2RSTR &= ~(1U << 13)); } while(0)

#define I2C1_REG_RESET()    do { (RCC->APB1RSTR |=  (1U << 21)); (RCC->APB1RSTR &= ~(1U << 21)); } while (0)
#define I2C2_REG_RESET()    do { (RCC->APB1RSTR |=  (1U << 22)); (RCC->APB1RSTR &= ~(1U << 22)); } while (0)
#define I2C3_REG_RESET()    do { (RCC->APB1RSTR |=  (1U << 23)); (RCC->APB1RSTR &= ~(1U << 23)); } while (0)


 

/*General Macros
 *
 */

#define ENABLE		        1
#define DISABLE 	        0

#define SET                 1
#define RESET               0

#define GPIO_ENABLE         ENABLE
#define GPIO_DISABLE        DISABLE

#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET

#define FLAG_RESET     RESET
#define FLAG_SET       SET


 /*
  * This macro returns the port code for a given GPIOx base address.
  * The returned value is written into SYSCFG_EXTICR register fields.
  *
  * STM32F401RE EXTICR port code:
  * GPIOA = 0000
  * GPIOB = 0001
  * GPIOC = 0010
  * GPIOD = 0011
  * GPIOE = 0100
  * GPIOH = 0111
  */

#define GPIO_BASEADDR_TO_CODE(x)       ((x == GPIOA) ? 0U : \
                                        (x == GPIOB) ? 1U : \
                                        (x == GPIOC) ? 2U : \
                                        (x == GPIOD) ? 3U : \
                                        (x == GPIOE) ? 4U : \
                                        (x == GPIOH) ? 7U : 0U)

  /*
   * IRQ(Interrupt Request) numbers for STM32F401RE MCU
   *
   * NOTE:
   * These values come from the STM32F401 vector table in RM0368.
   * They are used when configuring NVIC ISER/ICER/IPR registers.
   */

   /* EXTI IRQ numbers */
#define IRQ_NO_EXTI0          6U
#define IRQ_NO_EXTI1          7U
#define IRQ_NO_EXTI2          8U
#define IRQ_NO_EXTI3          9U
#define IRQ_NO_EXTI4          10U
#define IRQ_NO_EXTI9_5        23U
#define IRQ_NO_EXTI15_10      40U
/*
 * SPI IRQ number definitions
 */
#define IRQ_NO_SPI1        35U    /* SPI1 global interrupt */
#define IRQ_NO_SPI2        36U    /* SPI2 global interrupt */
#define IRQ_NO_SPI3        51U    /* SPI3 global interrupt */
#define IRQ_NO_SPI4        84U    /* SPI4 global interrupt */


#endif /* INC_STM32F401RE_H_ */
