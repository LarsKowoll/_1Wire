#ifndef HEADER_HARDWARECONTROLLER_H_
#define HEADER_HARDWARECONTROLLER_H_

#include <stdint.h>

int readBit();
int reset();
int sendCommand(int);
int wait(int microseconds);
int readByte();

#endif /* HEADER_HARDWARECONTROLLER_H_ */