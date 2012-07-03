// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	adc_functions.c										********
// *****																	********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "adc_functions.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************

// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	adc_read														********
// *****	return: double													********
// *****	(1) unsigned char: Device ID									********
// *********************************************************************************
double adc_read(unsigned char dh, unsigned char Address)
{
	unsigned char v_data[4];
	int v_data_count = 3;
	char test; 
	int PGA;
	int output;
	int Min_Code;
	double return_value, x;
	i2c_read(dh,Address,SKIP_WRITE_REGISTER,v_data,v_data_count);
	output = (int)(v_data[0]*256 + v_data[1]);
	test = v_data[2] & 0x0C;
	if(test == 0x00) Min_Code = -2048;
	if(test == 0x04) Min_Code = -8192;
	if(test == 0x08) Min_Code = -16384;
	if(test == 0x0C) Min_Code = -32768;
	test = v_data[2] & 0x03;
	if(test == 0x00) PGA = 1;
	if(test == 0x01) PGA = 2;
	if(test == 0x02) PGA = 4;
	if(test == 0x03) PGA = 8;
	x = (double)(Min_Code*PGA);
	return_value = ((double)(output))*3.3/(-1*x);
	return return_value;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	adc_config														********
// *****	return: double													********
// *****	(1) unsigned char: Device ID									********
// *********************************************************************************
void adc_config(unsigned char dh, unsigned char Address, unsigned char Gain, unsigned char Data_Rate)
{
	unsigned char v_data[2], v_register;
	int v_data_count = 0;
	v_register = Gain*4 + Data_Rate;
	i2c_write(dh,Address,v_register,v_data,v_data_count);
	return;
}
// ----------  END  ----------------------------------------------------------------


// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */



