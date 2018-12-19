#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "crc.h"
#include "hardwareController.h"
#include "HAL.h"
#include "temperatur.h"

void printSensors(int numberSensors, Sensor sensorList[numberSensors]) {
	TFT_cursor_off();
	TFT_gotoxy(1,1);
	TFT_puts("Sensors:");
	for(int i = 0; i < numberSensors; i++){
		TFT_gotoxy(1,i+3);
		char string[25];
		snprintf(string, sizeof(string), "Sensor %d: %x", i, sensorList[i].rom.crc);
		TFT_puts(string);
	}
}