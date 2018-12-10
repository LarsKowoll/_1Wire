/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "crc.h"

//--- For GPIOs -----------------------------
//Include instead of "stm32f4xx.h" for
//compatibility between Simulation and Board
//#include "TI_memory_map.h"

//--- For Touch Keypad ----------------------
//#include "keypad.h"

//--- For Timer -----------------------------
#include "timer.h"
#include "hardwareController.h"
#include "HAL.h"


int initHardwareController() {
	timerinit();
	
	// open-drain mode
	BUS_PORT->OTYPER |= (1 << PIN_DATA);
	
	// push-pull mode
	//BUS_PORT->OTYPER &= ~(1 << PIN_DATA);
	BUS_PORT->OTYPER &= ~(1 << PIN_POWER);
	
	// clear Register
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (PIN_DATA * 2));
	// Set Output Mode (write)
	BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (PIN_DATA * 2));
	
	// clear Register
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (PIN_POWER * 2));
	// Set Output Mode (write)
	BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (PIN_POWER * 2));
	setBusHigh(BUS_PORT, PIN_POWER);
	
	write_1();
	wait(100);

}

int setGPIOPin(GPIO_TypeDef* GPIOx, int pin) {
	/* Check second parameter */
	if ((pin < 0) || (pin > 15)) {
		
	}
	GPIOx->BSRRL = (0x01 << pin);
	return 0;
}

int setBusLow(GPIO_TypeDef* GPIOx, int pin) {
	GPIOx->BSRRH = (0x01 << pin);
}

int setBusHigh(GPIO_TypeDef* GPIOx, int pin) {
	GPIOx->BSRRL = (0x01 << pin);
}

int readBus(GPIO_TypeDef* GPIOx, int pin) {
	return (0x01 << pin) != (GPIOx->IDR & (0x01 << pin)); // '!=' has a higher priority than '&'
}

int readByte() {
	int ergebnis;
	for (int i = 0; i < 8; i++) {
		ergebnis += (readBit()<<i);
	}
	return ergebnis;
}

int sendCommand(int command) {
	for(int i = 0; i < 8; i++) {
		((command>>i)%2)==0 ? write_0() : write_1();
	}
}


