#include <stdint.h>
#include <string.h>
#include "stm32F401Re.h";
#include "stm32F401Re_gpio_driver.h"
#include "stm32F401Re_I2C_driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define MY_ADDR 0x61
#define SLAVE_ADDR 0x68

static I2C_Handle_t I2C_Handle;

static char Msg[] = "Hello,world!";


/*

PB6 I2C1_SCLK

PB& I2C1_SDA

*/


void delay() {

    for (uint32_t i = 0; i < 500000; i++);

}


static void Button_Init(void) {

	GPIO_Pin_Handle_t Button;

	Button.pGPIOx = GPIOC;

	Button.GPIO_pin_config.GPIO_PinMode = GPIO_MODE_IN;

	Button.GPIO_pin_config.GPIO_PinNumber = GPIO_PIN_NO_13;

	Button.GPIO_pin_config.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;

	Button.GPIO_pin_config.GPIO_PinoutputType = GPIO_OP_TYPE_PP;

	Button.GPIO_pin_config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&Button);

}

static void I2C1_GPIOInit() {

	GPIO_Pin_Handle_t I2C_Pins;

	I2C_Pins.pGPIOx = GPIOB;

	I2C_Pins.GPIO_pin_config.GPIO_PinMode = GPIO_MODE_ALTFN;

	I2C_Pins.GPIO_pin_config.GPIO_AltFuncMode = 4;

	I2C_Pins.GPIO_pin_config.GPIO_PinoutputType = GPIO_OP_TYPE_OD;

	I2C_Pins.GPIO_pin_config.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(GPIOB, ENABLE);

	//SCLK

	I2C_Pins.GPIO_pin_config.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2C_Pins);

	//SDA

	I2C_Pins.GPIO_pin_config.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2C_Pins);

	
}

static void I2C1_Init() {

	I2C_Handle.pI2Cx = I2C1;

	I2C_Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;

	I2C_Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	
	I2C_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;

	I2C_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C_Handle);

}

int main(void)
{

	Button_Init();

	I2C1_GPIOInit();

	I2C1_Init();

	I2C_PeripheralControl(I2C1, ENABLE);

	while (1) {
	
		while (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

		delay();

		I2C_MasterSendData(&I2C_Handle, Msg, strlen((char*)Msg), SLAVE_ADDR);
	
	}


	

}
