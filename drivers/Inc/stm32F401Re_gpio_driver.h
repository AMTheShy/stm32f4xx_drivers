#ifndef STM32F401RE_GPIO_DRIVER_H
#define STM32F401RE_GPIO_DRIVER_H

#include <stdint.h>
#include "stm32F401Re.h"

/*
 * GPIO pin configuration structure
 */
typedef struct
{
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinoutputType;
    uint8_t GPIO_PinPuPdControl;
    uint8_t GPIO_AltFuncMode;

} GPIO_Pin_Config_t;


/*
 * GPIO pin handle structure
 */
typedef struct
{
    GPIO_RegStruct_t *pGPIOx;
    GPIO_Pin_Config_t GPIO_pin_config;

} GPIO_Pin_Handle_t;


/*
 * Peripheral Clock setup
 */
void GPIO_PeriClockControl(GPIO_RegStruct_t *pGPIOx, uint8_t EnorDi);


/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Pin_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegStruct_t *pGPIOx);


/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegStruct_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegStruct_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegStruct_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegStruct_t *pGPIOx, uint16_t Value);

void GPIO_ToggleOutputPin(GPIO_RegStruct_t *pGPIOx, uint8_t PinNumber);


/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);


/*All valid GPIO Modes Macros

    /*
 * GPIO pin possible modes
 *
 * These values are used with GPIOx_MODER register.
 * Each pin uses 2 bits:
 * MODERy[1:0] = 00: Input mode
 * MODERy[1:0] = 01: General purpose output mode
 * MODERy[1:0] = 10: Alternate function mode
 * MODERy[1:0] = 11: Analog mode
 */
#define GPIO_MODE_IN          0U
#define GPIO_MODE_OUT         1U
#define GPIO_MODE_ALTFN       2U
#define GPIO_MODE_ANALOG      3U

 /*
  * GPIO interrupt modes
  *
  * These are driver-level mode values.
  * They are NOT written directly into GPIOx_MODER.
  *
  * For interrupt mode, the GPIO pin is usually configured as input,
  * and then EXTI/RTSR/FTSR/NVIC are configured separately.
  */
#define GPIO_MODE_IT_FT       4U    /* Interrupt falling edge trigger */
#define GPIO_MODE_IT_RT       5U    /* Interrupt rising edge trigger */
#define GPIO_MODE_IT_RFT      6U    /* Interrupt rising + falling edge trigger */


  /*
   * GPIO pin possible output types
   *
   * These values are used with GPIOx_OTYPER register.
   */
#define GPIO_OP_TYPE_PP       0U    /* Push-pull */
#define GPIO_OP_TYPE_OD       1U    /* Open-drain */


   /*
    * GPIO pin possible output speeds
    *
    * These values are used with GPIOx_OSPEEDR register.
    */
#define GPIO_SPEED_LOW        0U
#define GPIO_SPEED_MEDIUM     1U
#define GPIO_SPEED_FAST       2U
#define GPIO_SPEED_HIGH       3U


    /*
     * GPIO pin pull-up / pull-down configuration
     *
     * These values are used with GPIOx_PUPDR register.
     */
#define GPIO_NO_PUPD          0U
#define GPIO_PIN_PU           1U
#define GPIO_PIN_PD           2U


     /*
      * GPIO pin numbers
      */
#define GPIO_PIN_NO_0         0U
#define GPIO_PIN_NO_1         1U
#define GPIO_PIN_NO_2         2U
#define GPIO_PIN_NO_3         3U
#define GPIO_PIN_NO_4         4U
#define GPIO_PIN_NO_5         5U
#define GPIO_PIN_NO_6         6U
#define GPIO_PIN_NO_7         7U
#define GPIO_PIN_NO_8         8U
#define GPIO_PIN_NO_9         9U
#define GPIO_PIN_NO_10        10U
#define GPIO_PIN_NO_11        11U
#define GPIO_PIN_NO_12        12U
#define GPIO_PIN_NO_13        13U
#define GPIO_PIN_NO_14        14U
#define GPIO_PIN_NO_15        15U



*/



#endif
