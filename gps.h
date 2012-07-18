// *********************************************************************************
// *****	gps.h															********
// *****	Christopher Crews	-	7 December 2011							********
// *********************************************************************************
#ifdef _GUM
#ifndef _GPSFCTN
#define _GPSFCTN

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include "i2c_functions.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************
#define GPS_ADDRESS		0x84
#define RATE			500

// *********************************************************************************
// *****	dynModel (Dynamic Model) Options								********
#define PORTABLE		0x00
#define STATIONARY		0x02
#define PEDESTRIAN		0x03
#define AUTOMOTIVE		0x04
#define SEA				0x05
#define AIRBORNE_1G		0x06
#define AIRBORNE_2G		0x07
#define AIRBORNE_4G		0x08
// ---------- END DynModel Options -------------------------------------------------

// *********************************************************************************
// *****	fixMode (Fix Mode) Options										********
#define ONLY_2D			0x01
#define ONLY_3D			0x02
#define AUTO			0x03	// Auto 2D/3D fix
// ----------  END  fixMode Options  -----------------------------------------------

// *********************************************************************************
// *****	NMEA String Identifiers											********
#define DTM				(int)('D'*'T'*'M')
#define GBS				(int)('G'*'B'*'S')
#define GGA				(int)('G'*'G'*'A')
#define GLL				(int)('G'*'L'*'L')
#define GPQ				(int)('G'*'P'*'Q')
#define GRS				(int)('G'*'R'*'S')
#define GSA				(int)('G'*'S'*'A')
#define GST				(int)('G'*'S'*'T')
#define GSV				(int)('G'*'S'*'V')
#define RMC				(int)('R'*'M'*'C')
#define TXT				(int)('T'*'X'*'T')
#define VTG				(int)('V'*'T'*'G')
#define ZDA				(int)('Z'*'D'*'A')
// ----------  END  NMEA String ID's  ----------------------------------------------

// *********************************************************************************
// *****	# of fields in each NMEA String									********
#define DTM_fields		9
#define GBS_fields		9
#define GGA_fields		17
#define GLL_fields		7
#define GPQ_fields		2
#define GRS_fields		15
#define GSA_fields		20
#define GST_fields		9
#define GSV_fields		5
#define RMC_fields		13
#define TXT_fields		5
#define VTG_fields		10
#define ZDA_fields		7
// ----------  END  # of fields  ---------------------------------------------------

// ----------  END  Constants  -----------------------------------------------------


// *********************************************************************************
// *****	Variable Declarations											********
// *********************************************************************************
typedef struct gps_struct
{
	float latitude, longitude; 	// RMC (+/- DD.DDDDDD, South and West are negative)
	float ground_speed;			// RMC km/h
	float ground_course;		// RMC (degrees)
	float altitude;				// GGA (meters)
	int day, month, year;		// RMC day, month, year (dd, mm, yy)
	int hours, minutes, seconds;// GGA hours, minutes, seconds (hh, mm, ss)
	int num_sats;				// GGA (# of sats used for calcs)
	int fix_type; 				// GSA (1 = No Fix, 2 = 2D, 3 = 3D)
	int data_valid;				// RMC 1 if data valid, 0 if not valid
} gps_struct;

int new_gps_data;
int v_device;

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  GPS_Thread  *****
// return: void
// (1) argument type: void pointer
void *GPS_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// *****  Get_GPS_Data  *****
// return: void
// (1) pointer to a gps_struct
void Get_GPS_Data(gps_struct *);
// --------  END  ------------------------------------------------------------------

// *****  Save_I2C_device_ID  *****
// return: void
// (1) int:  device ID
void save_gps_i2c_device_id(int);
// ----------  END  ----------------------------------------------------------------

// *****  Init_GPS  *****
// return: void
void Init_GPS(void);
// ----------  END  ----------------------------------------------------------------

// *****  Init_GPS  *****
// return: void
void Wake_GPS(void);
// ----------  END  ----------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************

// ***** Set_GPS_Rate *****
// Sets how often the GPS signals are measured and Calculated. Is called when 
// initializing the GPS.
// return: void
// (1) int: measurement rate in milliseconds
void Set_GPS_Rate(unsigned char rate[]);
// ----------  END  ----------------------------------------------------------------

// ***** Set_Navigation_Mode *****
// Sets the navigation model and fixmode. Is called when initializing the GPS.
// return: void
// (1) unsigned char: dynamic model type (Use defined constants)
void Set_Navigation_Mode(unsigned char dynModel);
// ----------  END  ----------------------------------------------------------------

// ***** AssistNow_Autonomous_Enable *****
// Enables the AssistNow Autonomous feature. Battery Backup must be utilized for this
// feature to work. When enbled, satillite trajectory information will be stored in 
// BBRAM and used on startup for fast fix.
// return: void
// (1) unsigned char: Use constants "ON" or "OFF"
void AssistNow_Autonomous_Enable(unsigned char ON_OFF);
// ----------  END  ----------------------------------------------------------------

// ***** Set_SBAS *****
// Enables DGPS or Differential GPS. This allows the use of additional satellites, 
// and provides more accurate fixes.
// return: void
// (1) unsigned char: Use constants "ON" or "OFF"
void Set_SBAS(unsigned char ON_OFF);
// ----------  END  ----------------------------------------------------------------

// ***** Save_Current_Config *****
// Save the receiver's current configuration to permanant configuration
// return: void
// (1) void
void Save_Current_Config(void);
// ----------  END  ----------------------------------------------------------------

// ***** Set_GP### *****
// The following functions can be used to turn NMEA strings on or off. The Three letters
// following "Set_GP" indicates which NMEA string that function turns on or off.
// return: void
// (1) unsigned char: Use constants "ON" or "OFF"
void Set_GPRMC(unsigned char ON_OFF);
void Set_GPVTG(unsigned char ON_OFF);
void Set_GPGGA(unsigned char ON_OFF);
void Set_GPGSA(unsigned char ON_OFF);
void Set_GPGSV(unsigned char ON_OFF);
void Set_GPGLL(unsigned char ON_OFF);
void Set_GPDTM(unsigned char ON_OFF);
void Set_GPGBS(unsigned char ON_OFF);
void Set_GPGPQ(unsigned char ON_OFF);
void Set_GPGRS(unsigned char ON_OFF);
void Set_GPGST(unsigned char ON_OFF);
void Set_GPTXT(unsigned char ON_OFF);
void Set_GPZDA(unsigned char ON_OFF);
// ----------  END  ----------------------------------------------------------------

// ***** Calc_Checksum *****
// Every configuration message that is sent to the GPS receiver must have the correct
// Checksum at the end of the message. This function calculates the Checksum and 
// inserts it the data array.
// return: void
// (1) unsigned char array: array of unsigned chars that make up the message
// (2) int: number of elements in array
void Calc_Checksum(unsigned char v_data[], int v_num);
// ----------  END  ----------------------------------------------------------------

// ***** Check_ACK *****
// Checks for receiver's ACK NACK signal
// return: int: 1 = Acknowledged, 0 = NOT Acknowledged
int Check_ACK(void);
// ----------  END  ----------------------------------------------------------------

// ***** Parse_### *****
// Parses the ### NMEA string to extract the wanted data
// return: void
// (1) unsigned char *: ### NMEA string
// (2) gps_struct *: pointer to gps data structure
void Parse_RMC(unsigned char NMEA_string[], int v_string_length, gps_struct *);
void Parse_GGA(unsigned char NMEA_string[], int v_string_length, gps_struct *);
void Parse_GSA(unsigned char NMEA_string[], int v_string_length, gps_struct *);
// ----------  END  ----------------------------------------------------------------

// ***** Message_Count *****
// Counts the # of NMEA strings in the raw GPS data
// return: int: # of NMEA strings
// (1) unsigned char *: Raw gps data
int Message_Count(unsigned char v_gps_raw_data[], int v_num_bytes);
// ----------  END  ----------------------------------------------------------------

// ***** Message_Length *****
// Finds the starting position and length of each string
// return: void
// (1) unsigned char *: raw gps data
// (2) int: # of strings
// (3) int *: array for starting positions
// (4) int *; array for string lengths
void Message_Length(unsigned char [],int, int, int [], int []);
// ----------  END  ----------------------------------------------------------------

// ***** Separate_Strings *****
// Separates the raw data into individual NMEA strings
// return: void
// (1) unsigned char *: raw gps data
// (2) int: # of strings
// (3) int array: start positions
// (4) int array: string lengths
// (5) array of unsigned char arrays: for separated NMEA strings
void Separate_Strings(unsigned char [],int, int, int [], int [], unsigned char **);
// ----------  END  ----------------------------------------------------------------

// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _GPSFCTN  */
#endif  /* _GUM  */


