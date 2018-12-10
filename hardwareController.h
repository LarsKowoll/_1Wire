#ifndef HEADER_HARDWARECONTROLLER_H_
#define HEADER_HARDWARECONTROLLER_H_

#include <stdint.h>
#include "TI_Lib.h"
#include "general.h"

int initHardwareController();
int sendCommand(int);
int readByte();
int setBusLow(GPIO_TypeDef* GPIOx, int pin);
int setBusHigh(GPIO_TypeDef* GPIOx, int pin);
int readBus(GPIO_TypeDef* GPIOx, int pin);

#endif /* HEADER_HARDWARECONTROLLER_H_ */