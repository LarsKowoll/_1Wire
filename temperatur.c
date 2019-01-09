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
static int jumpToBranch(int index, unsigned long long romBits);
static void search_recursive(int current_index, uint64_t romcode);
static void jump_to(int current_index, uint64_t romcode);

// Variablen nur global zum Debuggen
unsigned long long romBits;
BYTE bits[64] = {0};
BYTE bit;
BYTE bitKomplement;
BYTE combination;
BYTE output;
int indexSensor;
int decisionBit;

int numberSensors2 = 0;
Sensor sensorList2[10];

void initROMList(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]) {
	*numberSensors = 0;
	for (int i = 0; i < maxSensors; i++) {
		sensorList[i] = createSensor(createROM());
		sensorList2[i] = createSensor(createROM());
	}
	
	// detect sensors
	decisionBit = 0;
	indexSensor = 0;
	
	int index = 0;
	unsigned long long romBits = 0;
	reset();
	sendCommand(SEARCH_ROM);
	search_recursive(index, romBits);
	detectSensors(index, romBits);
	
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

static void search_recursive(int current_index, uint64_t romcode){
    
	if(current_index >= 64){
        // add sensor and quit recursion
        //
        return;
    }
	
    int bit, inverse;
    bit = readBit();
    inverse = readBit();
    
    if(bit == !inverse){
        writeBit(bit);
        romcode |= ((uint64_t)bit << current_index);
        search_recursive(current_index + 1, romcode);        
    } else if( bit == 0 && inverse == 0){ // Das Bit der Sensoren ist nicht einheitlic
        // Need to decide
        // try 0 first
        write_0();
        romcode |= (0 << current_index);
        search_recursive(current_index + 1, romcode);
        
        // restart from the beginning
        jump_to(current_index, romcode);
        
        // try 1
       write_1();
        romcode |= (1 << current_index);
        search_recursive(current_index + 1, romcode);   
    } else {
        return;
    }
}

static void jump_to(int current_index, uint64_t romcode){
    reset();
    sendCommand(SEARCH_ROM);
			readBit();
			readBit();
	
    for(int i = 0; i < current_index; i++){		
			writeBit((romcode & (1 << i))>>i);
			readBit();
			readBit();			
    }
}

// wird rekursiv aufgerufen // int *numberSensors, int maxSensors, Sensor sensorList[maxSensors], int indexSensor, int decisionBit
void detectSensors(int index, unsigned long long romBits) {
	
	if (index >= 64) {
		// addROM
		//int i = 0;
		
		ROM rom;
		rom = *(ROM *) &romBits;
		
		sensorList2[numberSensors2].rom = rom;
		
		//sensorList[indexSensor].rom = rom;
		//indexSensor++;
		//*numberSensors = *numberSensors + 1;
		
		numberSensors2++;
		printSensors(numberSensors2, sensorList2);	
		
		return;
	}
	
	bit = readBit();
	bitKomplement = readBit();
	combination = checkBits(bit, bitKomplement);
	
	switch (combination) {
		case 0: write_0(); // bit = 0 für alle Sensoren
						detectSensors(index + 1, romBits);
						break;
		case 1: { // bit = 1 für alle Sensoren
							write_1();
							unsigned long long bitMaske = 1;
							romBits = romBits | (bitMaske << index);
							bits[index] = 1;
							detectSensors(index + 1, romBits);
						}
						break;
		case 2: // ungleiche Bits
						write_0();
						detectSensors(index + 1, romBits);
		
						jumpToBranch(index, romBits);
		
						write_1();
						unsigned long long bitMaske = 1;
						romBits = romBits | (bitMaske << index);
						bits[index] = 1;
						detectSensors(index + 1, romBits);
		
						break;
		case 3: break; // Fehlerfall; Verbindung zu den Sensoren ist unterbrochen
		default: break;
	}
}

static int jumpToBranch(int index, unsigned long long romBits) {
	if (index == 0) {
		return 0;
	}
	
	reset();
	sendCommand(SEARCH_ROM);
	
	int i = 0;
	int bit, bitKomplement, antwortBit;
	do {
		bit = readBit();
		bitKomplement = readBit();
		if (!((bit == 1) && (bitKomplement == 1))) {
			antwortBit = (romBits & (1 << i)) >> i;
			if (antwortBit == 0) {
				write_0();
			}
			else if (antwortBit == 1) {
				write_1();
			}
		}
		else {
			return -1; // error
		}
		i++;
	} while (i < index);
	
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