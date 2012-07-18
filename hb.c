// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	hb.c												********
// *****																	********
// *****	The hb.c file contains all the functions necessary to 			********
// *****	initialize, read, and calculate himidity.						********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "hb.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Hb_Thread														********
// *****	Continuous thread that reads Hb Sensor						********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Hb_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	system("echo 146 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio146/direction");
	system("echo 0 > /sys/class/gpio/gpio146/value");
// ----------  END  ----------------------------------------------------------------
	
// *********************************************************************************
// *****	Main Loop of Hb_Thread										********
	for(;;)
	{
		system("echo 1 > /sys/class/gpio/gpio146/value");
		usleep(50000);
		system("echo 0 > /sys/class/gpio/gpio146/value");
		usleep(50000);
		system("echo 1 > /sys/class/gpio/gpio146/value");
		usleep(50000);
		system("echo 0 > /sys/class/gpio/gpio146/value");
		usleep(850000);
	}
// ----------  END  Main Hb_Thread Loop  -------------------------------------------
}
// ----------  END  Hb_Thread  -----------------------------------------------------

#endif /* _GUM  */


