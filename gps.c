// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		7 December 2011										********
// *****	File Name: 	gps.c												********
// *****																	********
// *****	The gps.c file contains all the functions necessary to 			********
// *****	initialize, read, and parse GPS data from the I2C GPS device.	********
// *****	The GPS device this code was written for is the 				********
// *****	"u-blox 6 GPS Receiver".										********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "gps.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
gps_struct strc_gps;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	GPS_Thread														********
// *****	Continuous thread that reads and parses GPS data				********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *GPS_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	gps_struct strc_temp_gps;	// Temporary, local gps data structure
	int i, v_num_strings, v_num_bytes;
	unsigned char v_temp_data[2];
	unsigned char *v_gps_raw_data;
	int *v_string_length;
	int *v_start_position;
	unsigned char **NMEA_string;
	int *string_ID;
// ----------  END  ----------------------------------------------------------------

	Init_GPS();

// *********************************************************************************
// *****	Main Loop of GPS_Thread											********
	for(;;)
	{
// *********************************************************************************
// *****	Read GPS raw data stream										********
		// Get # of bytes available on data stream
		i2c_write(v_device,GPS_ADDRESS,0xFD,v_temp_data,0);
		i2c_read(v_device,GPS_ADDRESS,SKIP_WRITE_REGISTER,v_temp_data,2);
		v_num_bytes = (int)((v_temp_data[0]*256) + v_temp_data[1]);
		// Read the # of available bytes from the data stream
		v_gps_raw_data = (unsigned char *)malloc (sizeof (unsigned char) * (v_num_bytes+1));
		while(v_gps_raw_data == NULL)
		{
			v_gps_raw_data = (unsigned char *)malloc (sizeof (unsigned char) * (v_num_bytes+1));
		}
		i2c_read(v_device,GPS_ADDRESS,SKIP_WRITE_REGISTER,v_gps_raw_data,v_num_bytes);
		usleep(1000);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Get Needed string info (# of strings, length of each string,	********
// *****		and starting position of each string)						********
		// Get # of NMEA strings
		v_num_strings = Message_Count(v_gps_raw_data,v_num_bytes);
		// Get length and starting position of each string
		v_string_length = (int *)malloc (sizeof (int) * (v_num_strings));
		while(v_string_length == NULL)
		{
			v_string_length = (int *)malloc (sizeof (int) * (v_num_strings));
		}
		v_start_position = (int *)malloc (sizeof (int) * (v_num_strings));
		while(v_start_position == NULL)
		{
			v_start_position = (int *)malloc (sizeof (int) * (v_num_strings));
		}
		Message_Length(v_gps_raw_data,v_num_bytes,v_num_strings,v_start_position,v_string_length);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Separate raw data into individual strings						********
		// Allocate memory for strings
		NMEA_string = (unsigned char **)malloc(v_num_strings * sizeof(unsigned char *));
		while(NMEA_string == NULL)
		{
			NMEA_string = (unsigned char **)malloc(v_num_strings * sizeof(unsigned char *));
		}
		for(i=0;i<v_num_strings;i++)
		{
			NMEA_string[i] = (unsigned char *)malloc((v_string_length[i]+1) * sizeof(unsigned char));
			while(NMEA_string[i] == NULL)
			{
				NMEA_string[i] = (unsigned char *)malloc((v_string_length[i]+1) * sizeof(unsigned char));
			}
		}
		// Separate strings
		Separate_Strings(v_gps_raw_data,v_num_bytes,v_num_strings,v_start_position,v_string_length,NMEA_string);
		free(v_gps_raw_data);
		free(v_start_position);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Create numerical identifier for each string						********
		string_ID = (int *)malloc (sizeof (int) * (v_num_strings));
		while(string_ID == NULL)
		{
			string_ID = (int *)malloc (sizeof (int) * (v_num_strings));
		}
		for(i=0;i<v_num_strings;i++)
		{
			string_ID[i] = (int)(NMEA_string[i][3] * NMEA_string[i][4] * NMEA_string[i][5]);
		}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Loop to parse each NMEA string									********
		for(i=0;i<v_num_strings;i++)
		{
			// Each unique string ID corresponds to a case statement below (If there
			// is no match for the string ID it goes to case "default")
			switch ( string_ID[i] ) 
			{
				case RMC:
				{
					Parse_RMC(NMEA_string[i],v_string_length[i],&strc_temp_gps);
					break;
				}
				case GGA:
				{
					Parse_GGA(NMEA_string[i],v_string_length[i],&strc_temp_gps);
					break;
				}
				case GSA:
				{
					Parse_GSA(NMEA_string[i],v_string_length[i],&strc_temp_gps);
					break;
				}
				default:
				{
					break;
				}
			} // End of Switch Statement
		}
// ----------  END  String Parsing Loop  -------------------------------------------

// *********************************************************************************
// *****	Clean-up Code													********
		for(i=0;i<v_num_strings;i++)
		{
			free(NMEA_string[i]);
		}
		free(NMEA_string);
		free(v_string_length);
		free(string_ID);
		strc_gps = strc_temp_gps;
		new_gps_data = 1;
		usleep(1000000);	
// ----------  END  ----------------------------------------------------------------
	}
// ----------  END  Main GPS_Thread Loop  ------------------------------------------
}
// ----------  END  GPS_Thread  ----------------------------------------------------



// *********************************************************************************
// *****	Get_GPS_Data													********
// *****	this function is used to get the data out of the 				********
// *****	gps.c file and back to the calling routine						********
// *****	(r) void														********
// ***** 	(1) gps_struct *: pointer to a gps_struct		        		********
// *********************************************************************************
void Get_GPS_Data(gps_struct *temp)
{
	*temp = strc_gps;		// ***** copy data into tmp struct
	new_gps_data = 0;		// ***** set new data flag
}
// ----------  END GET_GPS_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_gps_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// *********************************************************************************
// *****	Init_GPS														********
// *****	Wakes up and initializes the GPS receiver						********
// *****	return: void													********
// *********************************************************************************
void Init_GPS(void)
{
	Wake_GPS();

	unsigned char *v_rate = malloc (sizeof (unsigned char) * (2));
	v_rate[0] = 0x01;
	v_rate[1] = 0xF4;
	Set_GPS_Rate(v_rate);
	free(v_rate);
	if(!Check_ACK()) printf("Set_GPS_Rate() Not Acknowledged!\n");
	
	Set_Navigation_Mode(PORTABLE);
	if(!Check_ACK()) printf("Set_Navigation_Mode() Not Acknowledged!\n");
	
	AssistNow_Autonomous_Enable(ON);
	if(!Check_ACK()) printf("AssistNow() Not Acknowledged!\n");
	
	Set_SBAS(ON);
	if(!Check_ACK()) printf("Set_SBAS() Not Acknowledged!\n");
	
	Set_GPRMC(ON);
	if(!Check_ACK()) printf("Set_GPRMC() Not Acknowledged!\n");
	
	Set_GPVTG(OFF);
	if(!Check_ACK())printf("Set_GPVTG() Not Acknowledged!\n");
	
	Set_GPGGA(ON);
	if(!Check_ACK()) printf("Set_GPGGA() Not Acknowledged!\n");
	
	Set_GPGSA(ON);
	if(!Check_ACK()) printf("Set_GPGSA() Not Acknowledged!\n");
	
	Set_GPGSV(OFF);
	if(!Check_ACK()) printf("Set_GPGSV() Not Acknowledged!\n");
	
	Set_GPGLL(OFF);
	if(!Check_ACK()) printf("Set_GPGLL() Not Acknowledged!\n");
	
	Set_GPDTM(OFF);
	if(!Check_ACK()) printf("Set_GPDTM() Not Acknowledged!\n");
	
	Set_GPGBS(OFF);
	if(!Check_ACK()) printf("Set_GPGBS() Not Acknowledged!\n");
	
	Set_GPGRS(OFF);
	if(!Check_ACK()) printf("Set_GPGRS() Not Acknowledged!\n");
	
	Set_GPGST(OFF);
	if(!Check_ACK()) printf("Set_GPGST() Not Acknowledged!\n");
	
	Set_GPZDA(OFF);
	if(!Check_ACK()) printf("Set_GPZDA() Not Acknowledged!\n");
	
	Save_Current_Config();
	if(!Check_ACK()) printf("Save_Current_Config() Not Acknowledged!\n");

	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Wake_GPS														********
// *****	Wakes up the GPS receiver										********
// *****	return: void													********
// *********************************************************************************
void Wake_GPS(void)
{
	unsigned char v_data[2];
	i2c_read(v_device,GPS_ADDRESS,0xFF,v_data,1);
	usleep(500000);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPS_Rate													********
// *****	Sets the rate at which the GPS location is measured and 		********
// *****	Calculated.														********
// *****	return: void, (1) int: measure_rate in milliseconds. It gets	********
// *****	converted to 2 bytes in length - is equal to the measurement	********
// *****	rate in milliseconds - rate[0] is MSB and rate[1] is LSB		********
// *********************************************************************************
void Set_GPS_Rate(unsigned char rate[])
{
	unsigned char v_data[13] = {0x62,0x06,0x08,0x06,0x00,rate[1],rate[0],0x01,0x00,0x00,0x01,0x0B,0x76};
	int v_num = 13;
	Calc_Checksum(v_data,v_num);
	i2c_write(v_device,GPS_ADDRESS,0xB5,v_data,v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_Navigation_Mode												********
// *****	Sets the dynamic model used and the fix mode. The dynamic model	********
// *****	simply sets the error thresholds and limits. The fix mode can	********
// *****	be set to 2D, 3D, or AUTO.										********
// *****	return: void													********
// *****	(1) unsigned char: dynModel - dynModel Constants are defined 	********
// *****	in "gps.h".														********
// *********************************************************************************
void Set_Navigation_Mode(unsigned char dynModel)
{
	unsigned char v_data[43] = {0x62,0x06,0x24,0x24,0x00,0x05,0x00,dynModel,0x03,0x00, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x00,0x00};
	int v_num = 43;
	if(dynModel == PORTABLE)
	{
		v_data[41] = 0x56;
		v_data[42] = 0xDB;
	}
	else if(dynModel == AIRBORNE_1G)
	{
		v_data[41] = 0x5C;
		v_data[42] = 0xA7;
	}
	else if(dynModel == AIRBORNE_2G)
	{
		v_data[41] = 0x5D;
		v_data[42] = 0xC9;
	}
	else if(dynModel == AIRBORNE_4G)
	{
		v_data[41] = 0x5E;
		v_data[42] = 0xEB;
	}
	else if(dynModel == SEA)
	{
		v_data[41] = 0x5B;
		v_data[42] = 0x85;
	}
	else if(dynModel == AUTOMOTIVE)
	{
		v_data[41] = 0x5A;
		v_data[42] = 0x63;
	}
	else if(dynModel == PEDESTRIAN)
	{
		v_data[41] = 0x59;
		v_data[42] = 0x41;
	}
	else if(dynModel == STATIONARY)
	{
		v_data[41] = 0x58;
		v_data[42] = 0x1F;
	}
	i2c_write(v_device,GPS_ADDRESS,0xB5,v_data,v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	AssistNow_Autonomous_Enable										********
// *****	Turns on or off the AssistNow Autonomous feature. Requires that	********
// *****	the backup battery be installed. This feature automatically 	********
// *****	saves and updates satellite information BBRAM for faster		********
// *****	fixes on cold starts.											********
// *****	return: void, (1) unsigned char: Use 							********
// *****	constants "ON" = 1 or "OFF" = 0 if defined.						********
// *********************************************************************************
void AssistNow_Autonomous_Enable(unsigned char ON_OFF)
{
	unsigned char v_data[47] = {0x62,0x06,0x23,0x28,0x00,0,0,0x00,0x40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,ON_OFF,0,0,0x7c,0x01,0,0,0,0,0,0,0,0,0x0F,0xA7};
	int v_num = 47;
	if(ON_OFF == OFF)
	{
		v_data[39] = 0x0E;
		v_data[40] = 0x9A;
	}
	i2c_write(v_device,GPS_ADDRESS,0xB5,v_data,v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_SBAS														********
// *****	Enables DGPS or Differential GPS, which allows the use of 		********
// *****	additional satellites.											********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants 								********
// *****	"ON" or "OFF".													********
// *********************************************************************************
void Set_SBAS(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x16,0x08,0x00,ON_OFF,0x07,0x02,0x00,0x00,0x00,0x00,0x00,0x2E,0xCF};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x2D;
		v_data[14] = 0xC7;
	}
	i2c_write(v_device,GPS_ADDRESS,0xB5,v_data,v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Save_Current_Config												********
// *****	Saves receiver's current config to permanant config				********
// *****	return: void													********
// *****	(1) void														********
// *********************************************************************************
void Save_Current_Config(void)
{
	unsigned char v_data[19] = {0x62,0x06,0x09,0x0C,0x00,0x00,0x00,0x00,0x00,0x07, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0xC7};
	int v_num = 19;
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPRMC														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPRMC(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x04,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x04,0x45};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x03;
		v_data[14] = 0x3F;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPVTG														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPVTG(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x05,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x05,0x4C};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x04;
		v_data[14] = 0x46;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGGA														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGGA(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x00,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x00,0x29};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0xFF;
		v_data[14] = 0x23;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGSA														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGSA(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x02,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x02,0x37};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x01;
		v_data[14] = 0x31;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGSV														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGSV(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x03,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x03,0x3E};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x02;
		v_data[14] = 0x38;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGLL														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGLL(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x01,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x01,0x30};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x00;
		v_data[14] = 0x2A;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPDTM														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPDTM(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x0A,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x0A,0x6F};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x09;
		v_data[14] = 0x69;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGBS														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGBS(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x09,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x09,0x68};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x08;
		v_data[14] = 0x62;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGPQ														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGPQ(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x40,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	int v_num = 15;
	Calc_Checksum(v_data,v_num);
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGRS														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGRS(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x06,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x06,0x53};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x05;
		v_data[14] = 0x4D;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPGST														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPGST(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x07,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x07,0x5A};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x06;
		v_data[14] = 0x54;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPTXT														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPTXT(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x41,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	int v_num = 15;
	Calc_Checksum(v_data,v_num);
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_GPZDA														********
// *****	Enables or disables the respective NMEA string.					********
// *****	return: void													********
// *****	(1) unsigned char: Use Constants "ON" or "OFF".					********
// *********************************************************************************
void Set_GPZDA(unsigned char ON_OFF)
{
	unsigned char v_data[15] = {0x62,0x06,0x01,0x08,0x00,0xF0,0x08,ON_OFF,0x00,0x00,0x00,0x00,0x00,0x08,0x61};
	int v_num = 15;
	if(ON_OFF == OFF)
	{
		v_data[13] = 0x07;
		v_data[14] = 0x5B;
	}
	i2c_write(v_device,GPS_ADDRESS, 0xB5, v_data, v_num);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calc_Checksum													********
// *****	Calculates the 2 byte checksum of the configuration messages.	********
// *****	return: void													********
// *****	(1) unsigned char arrary: Configuration message data array		********
// *****	(2) int: number of elements in data array						********
// *********************************************************************************
void Calc_Checksum(unsigned char v_data[], int v_num)
{
	int i;
	unsigned char *check = malloc (sizeof (unsigned char) * (2));
	unsigned char *v_temp_array = malloc (sizeof (unsigned char) * (v_num));
	for(i=0;i<v_num;i++)
	{
		v_temp_array[i] = v_data[i];
	}
	for(i=1;i<=v_num-3;i++)
	{
		check[0] = check[0] + v_temp_array[i];
		check[1] = check[1] + check[0];
	}
	v_temp_array[v_num-2] = check[0];
	v_temp_array[v_num-1] = check[1];
	for(i=0;i<=v_num-1;i++)
	{
		v_data[i] = v_temp_array[i];
	}
	free(v_temp_array);
	free(check);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Check_ACK														********
// *****	Checks to see if a message was Acknowledged						********
// *****	return: int: 1 = Acknowledged, 0 = NOT Acknowledged,			********
// *********************************************************************************
int Check_ACK(void)
{
	usleep(500000);
	int x = 0, i, num_bytes;
	unsigned char *data = malloc (sizeof (unsigned char) * (2));
	i2c_write(v_device,GPS_ADDRESS,0xFD,data,0);
	i2c_read(v_device,GPS_ADDRESS,SKIP_WRITE_REGISTER,data,2);
	num_bytes = (data[0]*256) + data[1];
	free(data);
	unsigned char *gps_data = malloc (sizeof (unsigned char) * (num_bytes+1));
	i2c_read(v_device,GPS_ADDRESS,SKIP_WRITE_REGISTER,gps_data,num_bytes);
	for(i=0;i<num_bytes;i++)
	{
		if(gps_data[i] == 0xB5)
		{
			if(gps_data[i+1] == 0x62)
			{
				if(gps_data[i+2] == 0x05)
				{
					if(gps_data[i+3] == 0x01)
					{
						x = 1;
						break;
					}
					else if(gps_data[i+3] == 0x00)
					{
						x = 0;
						break;
					}
				}
			}
		}
		if(i == num_bytes-1) printf("No ACK/NACK Received!\n");
	}
	free(gps_data);
	return(x);
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Parse_RMC														********
// *****	Parses the RMC NMEA string to extract the wanted data			********
// *****	return: void													********
// *****	(1) unsigned char arrary: NMEA string data						********
// *****	(2) int: length of string										********
// *****	(3) gps_struct pointer: pointer to gps data structure			********
// *********************************************************************************
void Parse_RMC(unsigned char NMEA_string[], int v_string_length, gps_struct *strc_temp_gps)
{
	int j, k, x;
	
	// RMC has 13 data fields. Allocate memory for each field
	unsigned char **field;
	field = (unsigned char **)malloc(RMC_fields * sizeof(unsigned char *));
	while(field == NULL)
	{
		field = (unsigned char **)malloc(RMC_fields * sizeof(unsigned char *));
	}
	for(j=0;j<RMC_fields;j++)
	{
		field[j] = malloc(20 * sizeof(unsigned char));
		while(field[j] == NULL)
		{
			field[j] = (unsigned char *)malloc(RMC_fields * sizeof(unsigned char));
		}
	}
	
	// Zero out the fields
	for(j=0;j<RMC_fields;j++)
	{
		for(k=0;k<20;k++)
		{
			field[j][k] = 0;
		}
	}
	
	// Separate the string into individual data fields
	k = x = 0;
	for(j=0;j<v_string_length;j++)
	{
		if(NMEA_string[j] != ',')
		{
			field[k][x] = NMEA_string[j];
			x++;
		}
		else
		{
			field[k][x] = '\0';
			k++;
			x = 0;
		}
	}
	
// *********************************************************************************
// *****	Extract Data Valid Indicator									********

	if(field[2][0] == 'A')
	{
		strc_temp_gps->data_valid = 1;
	}else
	{
		strc_temp_gps->data_valid = 0;
	}

// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract and Convert Latitude									********

	// Allocate memory for temporary variables
	unsigned char degrees[2];
	unsigned char minutes[8];
	unsigned char ind[1];
	float f_minutes[1];
	
	// Extract degrees and convert to float
	degrees[0] = field[3][0];
	degrees[1] = field[3][1];
	strc_temp_gps->latitude = atof(degrees);
	
	// Extract minutes
	for(j=2;j<10;j++)
	{
		minutes[j-2] = field[3][j];
	}
	
	// Convert minutes to fractions of a degree and add to latitude
	*f_minutes = atof(minutes);
	*f_minutes = *f_minutes/60.0;
	strc_temp_gps->latitude += *f_minutes;
	
	// Extract N/S indicator
	ind[0] = field[4][0];
	
	// Change sign of latitude if ind is 'S'
	if(ind[0] == 'S') strc_temp_gps->latitude = -(strc_temp_gps->latitude);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract and Convert Longitude									********
	
	// Extract degrees and convert to float
	degrees[0] = field[5][0];
	degrees[1] = field[5][1];
	degrees[2] = field[5][2];
	strc_temp_gps->longitude = atof(degrees);
	
	// Extract minutes
	for(j=3;j<11;j++)
	{
		minutes[j-3] = field[5][j];
	}
	
	// Convert minutes to fractions of a degree and add to latitude
	*f_minutes = atof(minutes);
	*f_minutes = *f_minutes/60.0;
	strc_temp_gps->longitude += *f_minutes;
	
	// Extract N/S indicator
	ind[0] = field[6][0];
	
	// Change sign of latitude if ind is 'S'
	if(ind[0] == 'W') strc_temp_gps->longitude = -(strc_temp_gps->longitude);
	
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract Speed and Course										********
	// Speed in Km/h
	strc_temp_gps->ground_speed = (atof(field[7]) * 1.852);
	
	// Course in Degrees
	strc_temp_gps->ground_course = atof(field[8]);
				
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract Date													********
	// Allocate memory for temporary variables
	unsigned char dd[2];
	unsigned char mm[2];
	unsigned char yy[2];
	
	// Extract and convert day to int
	dd[0] = field[9][0];
	dd[1] = field[9][1];
	strc_temp_gps->day = atoi(dd);
	
	// Extract and convert month to int
	mm[0] = field[9][2];
	mm[1] = field[9][3];
	strc_temp_gps->month = atoi(mm);
	
	// Extract and convert year to int
	yy[0] = field[9][4];
	yy[1] = field[9][5];
	strc_temp_gps->year = atoi(yy);

// ----------  END  ----------------------------------------------------------------
				
	// Free the allocated memory for the fields
	for(j=0;j<RMC_fields;j++)
	{
		free(field[j]);
	}
	free(field);
	return;
}

// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Parse_GGA														********
// *****	Parses the GGA NMEA string to extract the wanted data			********
// *****	return: void													********
// *****	(1) unsigned char arrary: NMEA string data						********
// *****	(2) int: length of string										********
// *****	(3) gps_struct pointer: pointer to gps data structure			********
// *********************************************************************************
void Parse_GGA(unsigned char NMEA_string[], int v_string_length, gps_struct *strc_temp_gps)
{
	int j, k, x;
	// GGA has 17 data fields. Allocate memory for each field
	unsigned char **field;
	field = (unsigned char **)malloc(GGA_fields * sizeof(unsigned char *));
	while(field == NULL)
	{
		field = (unsigned char **)malloc(GGA_fields * sizeof(unsigned char *));
	}
	for(j=0;j<GGA_fields;j++)
	{
		field[j] = malloc(20 * sizeof(unsigned char));
		while(field[j] == NULL)
		{
			field[j] = (unsigned char *)malloc(GGA_fields * sizeof(unsigned char));
		}
	}
	
	// Zero out the fields
	for(j=0;j<GGA_fields;j++)
	{
		for(k=0;k<20;k++)
		{
			field[j][k] = 0;
		}
	}
	
	// Separate the string into individual data fields
	k = x = 0;
	for(j=0;j<(int)(v_string_length);j++)
	{
		if(NMEA_string[j] != ',')
		{
			field[k][x] = NMEA_string[j];
			x++;
		}
		else
		{
			field[k][x] = '\0';
			k++;
			x = 0;
		}
	}
				
// *********************************************************************************
// *****	Extract Altitude												********

	// Altitude in meters
	strc_temp_gps->altitude = atof(field[9]);
				
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract and Convert Time										********

	// Allocate memory for temporary variables
	unsigned char hh[2];
	unsigned char nn[2];
	unsigned char ss[2];
	
	// Extract and convert day to int
	hh[0] = field[1][0];
	hh[1] = field[1][1];
	strc_temp_gps->hours = atoi(hh);
	
	// Extract and convert month to int
	nn[0] = field[1][2];
	nn[1] = field[1][3];
	strc_temp_gps->minutes = atoi(nn);
	
	// Extract and convert year to int
	ss[0] = field[1][4];
	ss[1] = field[1][5];
	strc_temp_gps->seconds = atoi(ss);
	
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Extract Number of Satellites									********

	// Number of Satellites being used for calculations
	strc_temp_gps->num_sats = atoi(field[7]);
				
// ----------  END  ----------------------------------------------------------------
				
	// Free the allocated memory for the fields
	for(j=0;j<GGA_fields;j++)
	{
		free(field[j]);
	}
	free(field);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Parse_GSA														********
// *****	Parses the GSA NMEA string to extract the wanted data			********
// *****	return: void													********
// *****	(1) unsigned char arrary: NMEA string data						********
// *****	(2) int: length of string										********
// *****	(3) gps_struct pointer: pointer to gps data structure			********
// *********************************************************************************
void Parse_GSA(unsigned char NMEA_string[], int v_string_length, gps_struct *strc_temp_gps)
{
	int j, k, x;
	// GSA has 20 data fields. Allocate memory for each field
	unsigned char **field;
	field = (unsigned char **)malloc(GSA_fields * sizeof(unsigned char *));
	while(field == NULL)
	{
		field = (unsigned char **)malloc(GSA_fields * sizeof(unsigned char *));
	}
	for(j=0;j<GSA_fields;j++)
	{
		field[j] = malloc(20 * sizeof(unsigned char));
		while(field[j] == NULL)
		{
			field[j] = (unsigned char *)malloc(GSA_fields * sizeof(unsigned char));
		}
	}
	
	// Zero out the fields
	for(j=0;j<GSA_fields;j++)
	{
		for(k=0;k<20;k++)
		{
			field[j][k] = 0;
		}
	}
	
	// Separate string into individual data fields
	k = x = 0;
	for(j=0;j<v_string_length;j++)
	{
		if(NMEA_string[j] != ',')
		{
			field[k][x] = NMEA_string[j];
			x++;
		}
		else
		{
			field[k][x] = '\0';
			k++;
			x = 0;
		}
	}
	
// *********************************************************************************
// *****	Extract Fix Type												********
				
	// Extract Fix Status
	strc_temp_gps->fix_type = atoi(field[2]);
	
// ----------  END  ----------------------------------------------------------------
				
	// Free the allocated memory for the fields
	for(j=0;j<GSA_fields;j++)
	{
		free(field[j]);
	}
	free(field);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Message_Count													********
// *****	Counts the # of NMEA strings in the raw gps data by counting	********
// *****	the # of dollar signs in the raw data							********
// *****	return: int: # of stings										********
// *****	(1) unsigned char arrary: Raw gps data							********
// *********************************************************************************
int Message_Count(unsigned char v_gps_raw_data[], int v_num_bytes)
{
	int i, v_num_strings;
	for(i=0;i<v_num_bytes;i++)
	{
		if(v_gps_raw_data[i] == '$')
		{
			v_num_strings++;
		}
	}
	return(v_num_strings);
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Message_Length 													********
// *****	Finds the starting position and length of each string			********
// *****	return: void													********
// *****	(1) unsigned char *: raw gps data								********
// *****	(2) int: # of strings											********
// *****	(3) int *: array for starting positions							********
// *****	(4) int *; array for string lengths								********
// *********************************************************************************
void Message_Length(unsigned char v_gps_raw_data[],int v_num_bytes, int v_num_strings, int v_start_position[], int v_string_length[])
{
	int i, j;
	// Initialize all string lengths to zero
	for(i=0;i<v_num_strings;i++)
	{
		v_string_length[i] = 0;
	}
	// Initialize counters to zero for looping
	i = j = 0;
	// Find starting position of each string in the raw data array
	for(i=0;i<v_num_bytes;i++)
	{
		if(v_gps_raw_data[i] == '$')
		{
			v_start_position[j] = i;
			j++;
		}
	}
	// Loop to count # of bytes in each string. Loop iterates once for each string
	for(i=0;i<v_num_strings;i++)
	{
		// Loop from strings starting position until a '*' is reached
		for(j=v_start_position[i];j<v_num_bytes;j++)
		{
			if(v_gps_raw_data[j] != '$' || j == v_start_position[i])
			{
				if(v_gps_raw_data[j] == '*')
				{
					v_string_length[i]++;
					break;
				}
				v_string_length[i]++;
			}
		}
	}
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Separate_Strings												********
// *****	Separates raw data into individual strings						********
// *****	return: void													********
// *****	(1) unsigned char *: raw gps data								********
// *****	(2) int: # of strings											********
// *****	(3) int *: array for starting positions							********
// *****	(4) int *; array for string lengths								********
// *****	(5) array of unsigned char arrays: for the NMEA strings			********
// *********************************************************************************
void Separate_Strings(unsigned char raw[],int v_num_bytes,int num_strings,int start[], int length[], unsigned char **NMEA)
{
	int i = 0, j = 0, k = 0;
	// Loop to separate Raw data into strings
	for(i=0;i<num_strings;i++)
	{
		// Loops through the raw data starting at the starting position for each
		// string, and loops for the length of the string
		for(j=start[i];j<v_num_bytes;j++)
		{
			if(k < length[i])
			{
				NMEA[i][k] = raw[j];
				k++;
			}
			else
			{
				k = 0;
				break;
			}
		}
	}
	return;
}
// ----------  END  ----------------------------------------------------------------

// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */








