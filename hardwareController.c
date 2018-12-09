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

static uint64_t startTime;
static uint64_t time;
static uint64_t timer;

static int wait(int microseconds);

int initHardwareController() {
	int pin = 1;
	GPIOG->OTYPER &= ~(1 << pin);
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

int readBit() {
	setBusLow(GPIOG, 1);
	// 6 mys warten
 	wait(6);
	setBusHigh(GPIOG, 1);
	// 9 mys warten
	wait(9);
	readBus(GPIOG, 1);
	// 55 mys warten
	wait(55);
	readBus(GPIOG, 1);
}

int wait(int microseconds) {
	timer = 0;
	startTime = getTimeStamp();
	while (timer < microseconds) {
		time = getTimeStamp();
		timer = (time - startTime) / CONVERT2US;
	}
}
