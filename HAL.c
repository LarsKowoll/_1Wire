
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

int write_1() {
	resetPIN(DATA_PIN);
	setBusLow(GPIOG, DATA_PIN);
	wait(6);
	setBusHigh(GPIOG, DATA_PIN);
	wait(64);
}

int write_0() {
	resetPIN(DATA_PIN);
	setBusLow(GPIOG, DATA_PIN);
	wait(60);
	setBusHigh(GPIOG, DATA_PIN);
	wait(10);
}

void setMode(int pin, int mode) {
	if (mode == OPEN_DRAIN) {
		BUS_PORT->OTYPER |= (1 << pin);
	}
	else if (mode == PUSH_PULL) {
		BUS_PORT->OTYPER &= ~(1 << pin);
	}
}

void resetPIN(int pin) {
	// clear Register
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (pin * 2));
	// Set Output Mode (write)
	BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (pin * 2));
}

int readDuring(uint32_t usSec) {
	BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (DATA_PIN * 2)); // setRW(1);
	uint32_t startTime = getTimeStamp();
	unsigned long long waitTime = usSec * CONVERT2US;
	uint32_t timeDiff = 0;
	BYTE Data = 1;
	while ((timeDiff < waitTime) && Data == 1) {
		Data = (BUS_PORT->IDR & (0x01 << DATA_PIN)) >> DATA_PIN;

		timeDiff = (getTimeStamp() - startTime);
	}
	
	// Den Rest der Zeit warten
	if(timeDiff < waitTime){
		wait((waitTime - timeDiff) / CONVERT2US);
	}
	
	resetPIN(DATA_PIN); // setRW(0);
	//DSetMode(ODrain);	
	setBusHigh(BUS_PORT, DATA_PIN); // setDPin(1);
	int info = BUS_PORT->IDR;
	info = info;
	
	return Data;
}

BYTE readBit() {
	resetPIN(DATA_PIN);
	
	setBusLow(GPIOG, DATA_PIN);
	// 6 mys warten
 	wait(6);
	setBusHigh(GPIOG, DATA_PIN);
	
	BYTE data = readDuring(15);
	
	wait(50);
	
	
	// 9 mys warten
	//wait(9);
	//BYTE data = readBus(GPIOG, DATA_PIN);
	// 55 mys warten
	//wait(55);
	return data;
}

int reset() {
	setBusLow(GPIOG, DATA_PIN);
	wait(480);
	setBusHigh(GPIOG, DATA_PIN);
	wait(70);
	readBus(GPIOG, DATA_PIN);
	wait(410);
	return 0;
}

int wait(int microseconds) {
	uint64_t startTime = getTimeStamp();
	uint64_t time;
	uint64_t timeDiff = 0;
	while (timeDiff < (microseconds * CONVERT2US)) {
		time = getTimeStamp();
		timeDiff = time - startTime;
	}
	timeDiff = timeDiff / CONVERT2US; // nur zu Testzwecken
}