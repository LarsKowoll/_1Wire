#ifndef HEADER_TEMPERATUR_H_
#define HEADER_TEMPERATUR_H_

#include <stdint.h>
#include "general.h"

ROM createROM();
Sensor createSensor();
void initROMList(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]);
int detectSensors(int *numberSensors, int maxSensors, Sensor sensorList[maxSensors]);

#endif /* HEADER_TEMPERATUR_H_ */