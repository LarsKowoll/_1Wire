#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "crc.h"
#include "hardwareController.h"
#include "HAL.h"

static BYTE checkBits(BYTE bit, BYTE bitKomplement);

int readROM() {
	reset();
	unsigned long long romBits = 0;
	
	for (int i = 0; i < 64; i++) {
		sendCommand(0xF0);
		BYTE bit = readBit();
		BYTE bitKomplement = readBit();
		BYTE combination = checkBits(bit, bitKomplement);
		BYTE output = -1;
		
		switch (combination) {
			case 0: output = 0; // bit = 0 für alle Sensoren
					break;
			case 1: { // bit = 1 für alle Sensoren
						unsigned long long bitMaske = 1;
						romBits = romBits | (bitMaske << i);
						output = 1;
					}
					break;
			case 2: // ungleiche Bits
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