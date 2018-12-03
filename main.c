/**
  ******************************************************************************
  * @file    	main.c 
  * @author  	Alfred Lohmann
  *        	  HAW-Hamburg
  *          	Labor für technische Informatik
  *          	Berliner Tor  7
  *          	D-20099 Hamburg
  * @version V1.0
  * @date    23.05.2013
  * @brief   Main program body
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "crc.h"

//--- For GPIOs -----------------------------
//Include instead of "stm32f4xx.h" for
//compatibility between Simulation and Board
//#include "TI_memory_map.h"

//--- For Touch Keypad ----------------------
//#include "keypad.h"

//--- For Timer -----------------------------
//#include "timer.c"

/**
  * @brief  Main program
  * @param  None
  */
int main(void)
{
	
  BYTE buff[] = { //x^8 + x^5 + x^4 +1 = 1 0011 0001
		//0xA2, 0x00, 0x00, 0x00, 0x01, 0xB8, 0x1C, 0x02
		0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xA2
	};
	
	if(isValidCRC(8,buff)){
		printf("isValid");
	}

	BYTE crc = 0;
	calcCRC(7, buff, &crc);
	printf("\n %x", crc);

  return 0;

}
// EOF
