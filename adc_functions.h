// *********************************************************************************
// *****	adc_functions.h													********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _ADCFCTN
#define _ADCFCTN

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
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****    adc_config()    *****
// Function for setting ADC's configuration parameters
void adc_config(unsigned char, unsigned char, unsigned char, unsigned char);
// --------  END  ------------------------------------------------------------------

// *****    adc_read()    *****
// Function for reading ADC voltage
double adc_read(unsigned char, unsigned char);
// --------  END  ------------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------

#endif  /* _ADCFCTN  */
#endif  /* _GUM  */

