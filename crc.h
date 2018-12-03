



#ifndef HEADER_CRC_CALC_H_
#define HEADER_CRC_CALC_H_
#include <stdint.h>




int gueltigeCRC(int size, BYTE data[size]);
int berechneCRC(int size, BYTE data[size], BYTE* result);


#endif /* HEADER_CRC_CALC_H_ */
