
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





BYTE readBit() {
	setBusLow(GPIOG, PIN_DATA);
	// 6 mys warten
 	wait(6);
	setBusHigh(GPIOG, PIN_DATA);
	// 9 mys warten
	wait(9);
	BYTE data = readBus(GPIOG, PIN_DATA);
	// 55 mys warten
	wait(55);
	return data;
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