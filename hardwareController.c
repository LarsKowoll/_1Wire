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

static uint64_t startTime;
static uint64_t time;
static uint64_t timeDiff;

static int setBusLow(GPIO_TypeDef* GPIOx, int pin);
static int setBusHigh(GPIO_TypeDef* GPIOx, int pin);
static int readBus(GPIO_TypeDef* GPIOx, int pin);


int initHardwareController() {
	timerinit();
	
	// open-drain mode
	//BUS_PORT->OTYPER |= (1 << PIN_DATA);
	// push-pull mode
	BUS_PORT->OTYPER &= ~(1 << PIN_DATA);
}

int setBusLow(GPIO_TypeDef* GPIOx, int pin) {
	GPIOx->BSRRH = (1 << pin);
}

int setBusHigh(GPIO_TypeDef* GPIOx, int pin) {
	GPIOx->BSRRL = (1 << pin);
}

int readBus(GPIO_TypeDef* GPIOx, int pin) {
	return (0x01 << pin) != (GPIOx->IDR & (0x01 << pin)); // '!=' has a higher priority than '&'
}

int write_1() {
	setBusLow(GPIOG, PIN_DATA);
	wait(6);
	setBusHigh(GPIOG, PIN_DATA);
	wait(64);
}

int write_0() {
	setBusLow(GPIOG, PIN_DATA);
	wait(60);
	setBusHigh(GPIOG, PIN_DATA);
	wait(10);
}

int readBit() {
	setBusLow(GPIOG, PIN_DATA);
	// 6 mys warten
 	wait(6);
	setBusHigh(GPIOG, PIN_DATA);
	// 9 mys warten
	wait(9);
	readBus(GPIOG, PIN_DATA);
	// 55 mys warten
	wait(55);
	return readBus(GPIOG, PIN_DATA);
}

int readByte() {
	int ergebnis;
	for (int i = 0; i < 8; i++) {
		ergebnis += (readBit()<<i);
	}
	return ergebnis;
}

int reset() {
	setBusLow(GPIOG, PIN_DATA);
	wait(480);
	setBusHigh(GPIOG, PIN_DATA);
	wait(70);
	readBus(GPIOG, PIN_DATA);
	wait(410);
	return 0;
}

int sendCommand(int command) {
	for(int i = 0; i < 8; i++) {
		((command>>i)%2)==0 ? write_0() : write_1();
	}
}

int wait(int microseconds) {
	timeDiff = 0;
	startTime = getTimeStamp();
	while (timeDiff < (microseconds * CONVERT2US)) {
		time = getTimeStamp();
		timeDiff = time - startTime;
	}
	timeDiff = timeDiff / CONVERT2US; // nur zu Testzwecken
}
