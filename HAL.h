#ifndef HEADER_HAL_H_
#define HEADER_HAL_H_

#include <stdint.h>
#include "TI_Lib.h"
#include "general.h"

int write_1();
int write_0();
void setMode(int pin, int mode);
int readDuring(uint32_t usSec);
BYTE readBit();
int reset();
int wait(int microseconds);
int setGPIOPin(GPIO_TypeDef* GPIOx, int pin);
void resetPIN(int pin);

#endif /* HEADER_HAL_H_ */