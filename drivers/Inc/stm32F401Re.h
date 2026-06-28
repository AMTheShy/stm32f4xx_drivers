/*
 * stm32F4xx.h
 *
 *  Created on: Jun 27, 2026
 *      Author: yuank
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

#include<stdint.h>
#define vo volatile


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

	vo uint32_t  AFRL;

	vo uint32_t  AFRH;

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


/* Clock enable macros for GPIO peripherals */

#define GPIOA_CLOCK_EN      (RCC->AHB1ENR |= (1U << 0))
#define GPIOB_CLOCK_EN      (RCC->AHB1ENR |= (1U << 1))
#define GPIOC_CLOCK_EN      (RCC->AHB1ENR |= (1U << 2))
#define GPIOD_CLOCK_EN      (RCC->AHB1ENR |= (1U << 3))
#define GPIOE_CLOCK_EN      (RCC->AHB1ENR |= (1U << 4))
#define GPIOH_CLOCK_EN      (RCC->AHB1ENR |= (1U << 7))


/* Clock enable macros for SPI peripherals */

#define SPI1_CLOCK_EN       (RCC->APB2ENR |= (1U << 12))
#define SPI2_CLOCK_EN       (RCC->APB1ENR |= (1U << 14))
#define SPI3_CLOCK_EN       (RCC->APB1ENR |= (1U << 15))
#define SPI4_CLOCK_EN       (RCC->APB2ENR |= (1U << 13))


/* Clock enable macros for USART peripherals */

#define USART1_CLOCK_EN     (RCC->APB2ENR |= (1U << 4))
#define USART2_CLOCK_EN     (RCC->APB1ENR |= (1U << 17))
#define USART6_CLOCK_EN     (RCC->APB2ENR |= (1U << 5))


/* Clock enable macros for I2C peripherals */

#define I2C1_CLOCK_EN       (RCC->APB1ENR |= (1U << 21))
#define I2C2_CLOCK_EN       (RCC->APB1ENR |= (1U << 22))
#define I2C3_CLOCK_EN       (RCC->APB1ENR |= (1U << 23))


/* Clock enable macro for SYSCFG peripheral */

#define SYSCFG_CLOCK_EN     (RCC->APB2ENR |= (1U << 14))

/* Clock disable macros for GPIO peripherals */

#define GPIOA_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 0))
#define GPIOB_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 1))
#define GPIOC_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 2))
#define GPIOD_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 3))
#define GPIOE_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 4))
#define GPIOH_CLOCK_DI      (RCC->AHB1ENR &= ~(1U << 7))


/* Clock disable macros for SPI peripherals */

#define SPI1_CLOCK_DI       (RCC->APB2ENR &= ~(1U << 12))
#define SPI2_CLOCK_DI       (RCC->APB1ENR &= ~(1U << 14))
#define SPI3_CLOCK_DI       (RCC->APB1ENR &= ~(1U << 15))
#define SPI4_CLOCK_DI       (RCC->APB2ENR &= ~(1U << 13))


/* Clock disable macros for USART peripherals */

#define USART1_CLOCK_DI     (RCC->APB2ENR &= ~(1U << 4))
#define USART2_CLOCK_DI     (RCC->APB1ENR &= ~(1U << 17))
#define USART6_CLOCK_DI     (RCC->APB2ENR &= ~(1U << 5))


/* Clock disable macros for I2C peripherals */

#define I2C1_CLOCK_DI       (RCC->APB1ENR &= ~(1U << 21))
#define I2C2_CLOCK_DI       (RCC->APB1ENR &= ~(1U << 22))
#define I2C3_CLOCK_DI       (RCC->APB1ENR &= ~(1U << 23))


/* Clock disable macro for SYSCFG peripheral */

#define SYSCFG_CLOCK_DI     (RCC->APB2ENR &= ~(1U << 14))


/*General Macros
 *
 */

#define ENABLE		        0
#define DISABLE 	        1

#define GPIO_ENABLE         ENABLE
#define GPIO_DISABLE        DISABLE
#endif /* INC_STM32F401RE_H_ */
