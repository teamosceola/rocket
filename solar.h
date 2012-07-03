// *********************************************************************************
// *****	solar.h															********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _SOLARFCTN
#define _SOLARFCTN


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
#include "adc_functions.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************
#define SOLAR_ADDRESS		0x98

#define SOLAR_M		1
#define SOLAR_B		0
#define SOLAR_SCALE	1

// ----------  END  Constants  -----------------------------------------------------


// *********************************************************************************
// *****	Variable Declarations											********
// *********************************************************************************
typedef struct solar_struct
{
	double v_sen_volt;
	double v_Solar;
} solar_struct;

int new_solar_data;
int v_device;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  Solar_Thread  *****
// return: void
// (1) argument type: void pointer
void *Solar_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// *****  Get_Solar_Data  *****
// return: void
// (1) pointer to a solar_struct
void Get_Solar_Data(solar_struct *);
// --------  END  ------------------------------------------------------------------

// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_solar_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************

void Calculate_Solar(solar_struct *);

// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _SOLARFCTN  */
#endif  /* _GUM  */

