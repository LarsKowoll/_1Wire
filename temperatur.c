#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "crc.h"
#include "hardwareController.h"
#include "HAL.h"
#include "temperatur.h"
#include "output.h"

static BYTE checkBits(BYTE bit, BYTE bitKomplement);

unsigned long long romBits;
BYTE bits[64] = {0};
BYTE bit;
BYTE bitKomplement;
BYTE combination;
BYTE output;

void initROMList(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]) {
	*numberSensors = 0;
	for (int i = 0; i < maxSensors; i++) {
		sensorList[i] = createSensor(createROM());
	}
	
	// detect sensors
	detectSensors(numberSensors, maxSensors, sensorList);
	
	// tft output
	printSensors(3, sensorList);
}

ROM createROM() {
	ROM rom;
	rom.crc = 0;
	rom.familyCode = 0;
	for (int i = 0; i < sizeof(rom.serialNumber); i++) {
		rom.serialNumber[i] = 0;
	}
	return rom;
}

Sensor createSensor(ROM rom) {
	Sensor sensor;
	sensor.rom = rom;
	sensor.temperature = 0;
	return sensor;
}

int detectSensors(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]) {
	reset();
	romBits = 0;
	int decisionBit = 0;
	
	sendCommand(SEARCH_ROM);
	for (int i = 0; i < 64; i++) {
		bit = readBit();
		bitKomplement = readBit();
		combination = checkBits(bit, bitKomplement);
		output = -1;
		
		switch (combination) {
			case 0: output = 0; // bit = 0 für alle Sensoren
					break;
			case 1: { // bit = 1 für alle Sensoren
						unsigned long long bitMaske = 1;
						romBits = romBits | (bitMaske << i);
						bits[i] = 1;
						output = 1;
					}
					break;
			case 2: // ungleiche Bits
					romBits = romBits | (0 << i);
					output = 0; // deselecting devices with bit = 1
					decisionBit = i;
					break;
			case 3: break;// Fehlerfall; Verbindung zu den Sensoren ist unterbrochen
			default: break;
		}
		
		switch (output) {
			case 0: write_0();
					break;
			case 1: write_1();
					break;
			default: break;
		}
	}
	
	int i = 0;
	ROM rom;
	rom = *(ROM *) &romBits;
	
	return 0;
}

static BYTE checkBits(BYTE bit, BYTE bitKomplement) {
	if ((bit == 0) && (bitKomplement == 1)) {
		return 0;
	}
	else if ((bit == 1) && (bitKomplement == 0)) {
		return 1;
	}
	else if ((bit == 0) && (bitKomplement == 0)) {
		return 2;
	}
	else if ((bit == 1) && (bitKomplement == 1)) {
		return 3;
	}
}