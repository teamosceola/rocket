// *********************************************************************************
// *****	humidity.h														********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _HUMFCTN
#define _HUMFCTN


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
#define HUMIDITY_ADDRESS		0x9A

// ----------  END  Constants  -----------------------------------------------------


// *********************************************************************************
// *****	Variable Declarations											********
// *********************************************************************************
typedef struct humidity_struct
{
	double v_sen_volt;
	double v_Humidity;
} humidity_struct;

int new_humidity_data;
double v_temperature_c;
int v_device;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  Humidity_Thread  *****
// return: void
// (1) argument type: void pointer
void *Humidity_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// *****  Get_Humidity_Data  *****
// return: void
// (1) pointer to a humidity_struct
void Get_Humidity_Data(humidity_struct *);
// --------  END  ------------------------------------------------------------------

// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_humidity_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************

void Calculate_Humidity(humidity_struct *);

// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _TMPFCTN  */
#endif  /* _GUM  */

