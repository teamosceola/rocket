// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	humidity.c											********
// *****																	********
// *****	The humidity.c file contains all the functions necessary to 	********
// *****	initialize, read, and calculate himidity.						********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "humidity.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
humidity_struct strc_humidity;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Humidity_Thread													********
// *****	Continuous thread that reads Humidity Sensor					********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Humidity_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	humidity_struct strc_temp_humidity;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Configure Humidity Sensor ADC									********
	//adc_config(v_device,HUMIDITY_ADDRESS,0x03,0x00);
// ----------  END  ----------------------------------------------------------------
	
// *********************************************************************************
// *****	Main Loop of Humidity_Thread									********
	for(;;)
	{
		strc_temp_humidity.v_sen_volt = adc_read(v_device,HUMIDITY_ADDRESS);
		Calculate_Humidity(&strc_temp_humidity);
		strc_humidity = strc_temp_humidity;
		new_humidity_data = 1;
		usleep(500000);
	}
// ----------  END  Main Humidity_Thread Loop  -------------------------------------
}
// ----------  END  Humidity_Thread  -----------------------------------------------



// *********************************************************************************
// *****	Get_Humidity_Data												********
// *****	this function is used to get the data out of the 				********
// *****	Humidity.c file and back to the calling routine					********
// *****	(r) void														********
// ***** 	(1) humidity_struct *: pointer to a humidity_struct 			********
// *********************************************************************************
void Get_Humidity_Data(humidity_struct *temp)
{
	*temp = strc_humidity;		// ***** copy data into tmp struct
	new_humidity_data = 0;		// ***** set new data flag
}
// ----------  END GET_ALT_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_humidity_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// *********************************************************************************
// *****	Calculate_Humidity												********
// *****	This functions calculates humidity								********
// *****	(r) void														********
// ***** 	(1) humidity_struct *: pointer to humidity structure  		    ********
// *********************************************************************************
void Calculate_Humidity(humidity_struct *temp)
{
	temp->v_Humidity = (double)((((temp->v_sen_volt/3.3)-0.1515)/0.00636)/(1.0546-0.00216*v_temperature_c));
	return;
}
// ----------  END  ----------------------------------------------------------------

// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */



