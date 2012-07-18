// *********************************************************************************
// *****	alt.h															********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _ALTFCTN
#define _ALTFCTN


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
#include "main.h"
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************
#define ALT_ADDRESS				0xEE
#define LAUNCH_THRESHOLD  		-5
// ----------  END  Constants  -----------------------------------------------------


// *********************************************************************************
// *****	Variable Declarations											********
// *********************************************************************************
typedef struct alt_struct
{
	short v_AC1, v_AC2, v_AC3;
	unsigned short v_AC4, v_AC5, v_AC6;
	short v_B1, v_B2, v_MB, v_MC, v_MD;
	double v_UT, v_UP, v_X1, v_X2, v_X3, v_B5;
	double v_T, v_B6, v_B3, v_B4, v_B7, v_p;
	double v_Pressure, v_Altitude, v_init_pressure;
	double v_z[5];
	double v_FR[4];
	double v_FRa[3];
	double v_FRma;
	int v_status;
} alt_struct;

int new_alt_data;
int v_device;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  Alt_Thread  *****
// return: void
// (1) argument type: void pointer
void *Alt_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// *****  Get_Alt_Data  *****
// return: void
// (1) pointer to a gps_struct
void Get_Alt_Data(alt_struct *);
// --------  END  ------------------------------------------------------------------

// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_alt_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************

void Read_Coefficients(alt_struct *);
void Read_Apt_Temperature(alt_struct *);
void Read_Pressure(alt_struct *);
void Calculate_Apt_Temperature(alt_struct *);
void Calculate_Pressure(alt_struct *);
void Calculate_Altitude(alt_struct *);
void Calculate_FR(alt_struct *);
void Calculate_FRa(alt_struct *);
void Calculate_FRma(alt_struct *);
void Set_Status(alt_struct *);
double gettimediff(struct timeval *, struct timeval *, double);

// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _ALTFCTN  */
#endif  /* _GUM  */

