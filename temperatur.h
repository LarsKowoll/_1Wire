#ifndef HEADER_TEMPERATUR_H_
#define HEADER_TEMPERATUR_H_

#include <stdint.h>
#include "general.h"

ROM createROM();
Sensor createSensor();
void initROMList(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]);
void detectSensors(int index, unsigned long long romBits);
#endif /* HEADER_TEMPERATUR_H_ */