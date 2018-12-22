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
	int zeile = 3;
	for(int i = 0; i < numberSensors; i++){
		TFT_gotoxy(1,zeile);
		char string[25];
		snprintf(string, sizeof(string), "Sensor %d: fc: %X; sn: %X%X%X%X%X%X; crc: %X", i, sensorList[i].rom.familyCode,
																																							sensorList[i].rom.serialNumber[0],
																																							sensorList[i].rom.serialNumber[1],
																																							sensorList[i].rom.serialNumber[2],
																																							sensorList[i].rom.serialNumber[3],
																																							sensorList[i].rom.serialNumber[4],
																																							sensorList[i].rom.serialNumber[5],
																																							sensorList[i].rom.crc);
		TFT_puts(string);
		zeile++;
		TFT_gotoxy(11,zeile);
		//char string[25];
		snprintf(string, sizeof(string), "crc: %X", sensorList[i].rom.crc);
		TFT_puts(string);
		zeile++;
	}
}