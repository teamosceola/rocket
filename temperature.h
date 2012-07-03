// *********************************************************************************
// *****	temperature.h													********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _TMPFCTN
#define _TMPFCTN


// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "i2c_functions.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************
#define TMP_ADDRESS		0x90

// ----------  END  Constants  -----------------------------------------------------


// *********************************************************************************
// *****	Variable Declarations											********
// *********************************************************************************
typedef struct temperature_struct
{
	unsigned char v_raw_temp[2];
	double v_Temperature;
} temperature_struct;

int new_temperature_data;
int v_device;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  Temperature_Thread  *****
// return: void
// (1) argument type: void pointer
void *Temperature_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// *****  Get_Temperature_Data  *****
// return: void
// (1) pointer to a temperature_struct
void Get_Temperature_Data(temperature_struct *);
// --------  END  ------------------------------------------------------------------

// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_temperature_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************

void Read_Temperature(temperature_struct *);

// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _TMPFCTN  */
#endif  /* _GUM  */

