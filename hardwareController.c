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
	
	// power pin
	setMode(POWER_PIN, PUSH_PULL);
	// clear register
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (POWER_PIN * 2));
	// set output mode (write)
	BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (POWER_PIN * 2));
	setBusHigh(BUS_PORT, POWER_PIN);
	
	// data pin
	setMode(DATA_PIN, OPEN_DRAIN);
	// clear register
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (DATA_PIN * 2));
	// set output mode (write)
	BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (DATA_PIN * 2));
	setBusHigh(BUS_PORT, DATA_PIN);
	
	wait(100);
	
	reset();
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
		wait(1);
		((command>>i)%2)==0 ? write_0() : write_1();
	}
}

int writeBit(BYTE out) {
	resetPIN(DATA_PIN);
	if ((out & 0x01) == 1) {
		// write 1
		write_1();

	} else {
		// write 0
		write_0();
	}
	return 0;
}

int writeBytes(int numberOfBytes, BYTE out[numberOfBytes]) {
	for (int byteCount = 0; byteCount < numberOfBytes; byteCount++) {
		for (int i = 0; i < 8; i++) {
			wait(1); 
			writeBit(out[byteCount] >> i); 
		}
	}
}

int writeByte(BYTE out){
	writeBytes(1,&out);
	return 0;
}


