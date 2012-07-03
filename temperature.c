// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	temperature.c										********
// *****																	********
// *****	The alt.c file contains all the functions necessary to 			********
// *****	initialize, read, and calculate temperature.					********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "temperature.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
temperature_struct strc_temperature;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Temperature_Thread												********
// *****	Continuous thread that reads Temperature Sensor					********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Temperature_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	temperature_struct strc_temp_temperature;
// ----------  END  ----------------------------------------------------------------

	
// *********************************************************************************
// *****	Main Loop of Temperature_Thread									********
	for(;;)
	{
		Read_Temperature(&strc_temp_temperature);
		strc_temperature = strc_temp_temperature;
		new_temperature_data = 1;
		usleep(250000);
	}
// ----------  END  Main Temperature_Thread Loop  ----------------------------------
}
// ----------  END  Temperature_Thread  --------------------------------------------



// *********************************************************************************
// *****	Get_Temperature_Data											********
// *****	this function is used to get the data out of the 				********
// *****	temperature.c file and back to the calling routine				********
// *****	(r) void														********
// ***** 	(1) temperature_struct *: pointer to a temperature_struct 		********
// *********************************************************************************
void Get_Temperature_Data(temperature_struct *temp)
{
	*temp = strc_temperature;		// ***** copy data into tmp struct
	new_temperature_data = 0;		// ***** set new data flag
}
// ----------  END GET_ALT_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_temperature_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// *********************************************************************************
// *****	Read_Temperature												********
// *****	This functions reads the temperature							********
// *****	(r) void														********
// ***** 	(1) temperature_struct *: pointer to temperature structure		********
// *********************************************************************************
void Read_Temperature(temperature_struct *temp)
{
	unsigned char v_data[3];
	int v_data_count, v_raw_temp, v_register;
	v_data_count = 0;
	int ret;
	v_register = 0x00;
	i2c_write(v_device,TMP_ADDRESS,v_register,v_data,v_data_count);
	v_data_count = 2;
	ret = i2c_read(v_device,TMP_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	v_raw_temp = (int)(v_data[0]*256 + v_data[1]);
	v_raw_temp = v_raw_temp >> 4;
	temp->v_Temperature = ((double)(v_raw_temp))*0.0625;
	return;
}
// ----------  END  ----------------------------------------------------------------

// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */



