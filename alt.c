// *********************************************************************************
// *****	Author:		Christopher Crews									********
// *****	Date:		10 February 2012									********
// *****	File Name: 	alt.c												********
// *****																	********
// *****	The alt.c file contains all the functions necessary to 			********
// *****	initialize, read, and calculate altimetetr.						********
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "alt.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
// *********************************************************************************
alt_struct strc_alt;
int FR_count, count, count2;
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Alt_Thread														********
// *****	Continuous thread that reads Altimeter							********
// *****	return: void													********
// *****	(1) void pointer												********
// *********************************************************************************
void *Alt_Thread(void *ptr)
{
// *********************************************************************************
// *****	Initialize Variables											********
	alt_struct strc_temp_alt;
	struct timeval init_time;
	struct timeval curr_time;
	FR_count = count = count2 = 0;
	strc_temp_alt.v_status = 0;
	int i;
	double sum;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Read Coefficients												********
	Read_Coefficients(&strc_temp_alt);
	
	// Get Initial Pressure
	for(i=0;i<10;i++)
	{
		Read_Apt_Temperature(&strc_temp_alt);
		Read_Pressure(&strc_temp_alt);
		Calculate_Apt_Temperature(&strc_temp_alt);
		Calculate_Pressure(&strc_temp_alt);
		sum = sum + strc_temp_alt.v_Pressure;
		usleep(100000);
	}
	strc_temp_alt.v_init_pressure = sum/10;
// ----------  END  ----------------------------------------------------------------
	
// *********************************************************************************
// *****	Main Loop of Alt_Thread											********
	gettimeofday(&init_time,NULL);
	for(;;)
	{
		gettimeofday(&curr_time,NULL);
		if(gettimediff(&init_time,&curr_time,1) >= 250000)
		{
			Read_Apt_Temperature(&strc_temp_alt);
			Read_Pressure(&strc_temp_alt);
			Calculate_Apt_Temperature(&strc_temp_alt);
			Calculate_Pressure(&strc_temp_alt);
			Calculate_Altitude(&strc_temp_alt);
			Calculate_FR(&strc_temp_alt);
			Calculate_FRa(&strc_temp_alt);
			Calculate_FRma(&strc_temp_alt);
			Set_Status(&strc_temp_alt);
			strc_alt = strc_temp_alt;
			new_alt_data = 1;
		}
	}
// ----------  END  Main Alt_Thread Loop  ------------------------------------------
}
// ----------  END  Alt_Thread  ----------------------------------------------------



// *********************************************************************************
// *****	Get_Alt_Data													********
// *****	this function is used to get the data out of the 				********
// *****	alt.c file and back to the calling routine						********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to a alt_struct		        		********
// *********************************************************************************
void Get_Alt_Data(alt_struct *temp)
{
	*temp = strc_alt;		// ***** copy data into tmp struct
	new_alt_data = 0;		// ***** set new data flag
}
// ----------  END GET_ALT_DATA  ---------------------------------------------------

// *********************************************************************************
// *****	Save_I2C_device_ID												********
// *****	this functions receives and saves the I2C device ID number		********
// *****	(r) void														********
// ***** 	(1) int: device ID									            ********
// *********************************************************************************
void save_alt_i2c_device_id(int I2C_ID)
{
	v_device = I2C_ID;
	return;
}
// ----------  END SAVE_I2C_DEVICE_ID ----------------------------------------------

// *********************************************************************************
// *****	Read_Coefficients												********
// *****	This functions reads the manufacturer calibrated coeffients		********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Read_Coefficients(alt_struct *temp)
{
	unsigned char v_data[2];
	int v_data_count;
	
	// AC1
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAA,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC1 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAB,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC1 = temp->v_AC1 + v_data[0];
	
	// AC2
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAC,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC2 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAD,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC2 = temp->v_AC2 + v_data[0];
	
	// AC3
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAE,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC3 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xAF,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC3 = temp->v_AC3 + v_data[0];
	
	// AC4
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB0,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC4 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB1,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC4 = temp->v_AC4 + v_data[0];
	
	// AC5
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB2,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC5 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB3,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC5 = temp->v_AC5 + v_data[0];
	
	// AC6
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB4,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC6 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB5,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_AC6 = temp->v_AC6 + v_data[0];
	
	// B1
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB6,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_B1 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB7,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_B1 = temp->v_B1 + v_data[0];
	
	// B2
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB8,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_B2 = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xB9,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_B2 = temp->v_B2 + v_data[0];
	
	// MB
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBA,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MB = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBB,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MB = temp->v_MB + v_data[0];
	
	// MC
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBC,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MC = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBD,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MC = temp->v_MC + v_data[0];
	
	// MD
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBE,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MD = v_data[0]*256;
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xBF,v_data,v_data_count);
	v_data_count = 1;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_MD = temp->v_MD + v_data[0];
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Read_Temperature												********
// *****	This functions reads the uncompensated temperature				********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Read_Apt_Temperature(alt_struct *temp)
{
	unsigned char v_data[3];
	int v_data_count;
	v_data_count = 1;
	v_data[0] = 0x2E;
	i2c_write(v_device,ALT_ADDRESS,0xF4,v_data,v_data_count);
	usleep(5000);
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xF6,v_data,v_data_count);
	v_data_count = 2;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_UT = (double)(v_data[0]*256 + v_data[1]);
	return;
	
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Read_Pressure													********
// *****	This functions reads the uncompensated Pressure					********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Read_Pressure(alt_struct *temp)
{
	unsigned char v_data[3];
	int v_data_count;
	v_data_count = 1;
	v_data[0] = 0x34 + (2 << 6);
	i2c_write(v_device,ALT_ADDRESS,0xF4,v_data,v_data_count);
	usleep(14000);
	v_data_count = 0;
	i2c_write(v_device,ALT_ADDRESS,0xF6,v_data,v_data_count);
	v_data_count = 3;
	i2c_read(v_device,ALT_ADDRESS,SKIP_WRITE_REGISTER,v_data,v_data_count);
	temp->v_UP = (double)((v_data[0]*pow(2,16) + v_data[1]*256 + v_data[2])/pow(2,6));
	return;
	
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_Temperature											********
// *****	This functions calculates the internal aperature temperature	********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_Apt_Temperature(alt_struct *temp)
{
	temp->v_X1 = (temp->v_UT-(double)(temp->v_AC6))*(double)(temp->v_AC5)/pow(2,15);
	temp->v_X2 = (double)(temp->v_MC)*pow(2,11)/(temp->v_X1+(double)(temp->v_MD));
	temp->v_B5 = temp->v_X1 + temp->v_X2;
	temp->v_T = (temp->v_B5 + 8)/pow(2,4);
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_Pressure												********
// *****	This functions calculates temperature compensated pressure		********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_Pressure(alt_struct *temp)
{
	temp->v_B6 = temp->v_B5 - 4000;
	temp->v_X1 = ((double)(temp->v_B2)*(temp->v_B6*temp->v_B6/pow(2,12)))/pow(2,11);
	temp->v_X2 = ((double)(temp->v_AC2))*temp->v_B6/pow(2,11);
	temp->v_X3 = temp->v_X1 + temp->v_X2;
	temp->v_B3 = (((double)(temp->v_AC1)*4+temp->v_X3)*pow(2,2) + 2)/4;
	temp->v_X1 = ((double)(temp->v_AC3))*temp->v_B6/pow(2,13);
	temp->v_X2 = (((double)(temp->v_B1))*(temp->v_B6*temp->v_B6/pow(2,12)))/pow(2,16);
	temp->v_X3 = ((temp->v_X1+temp->v_X2)+2)/pow(2,2);
	temp->v_B4 = ((double)(temp->v_AC4))*(temp->v_X3+32768)/pow(2,15);
	temp->v_B7 = (temp->v_UP-temp->v_B3)*(50000/4);
	if(temp->v_B7 < 0x80000000)
	{
		temp->v_p = (temp->v_B7*2)/temp->v_B4;
	}
	else
	{
		temp->v_p = (temp->v_B7/temp->v_B4)*2;
	}
	temp->v_X1 = (temp->v_p/pow(2,8))*(temp->v_p/pow(2,8));
	temp->v_X1 = (temp->v_X1*3038)/pow(2,16);
	temp->v_X2 = (-7357*temp->v_p)/pow(2,16);
	temp->v_p = temp->v_p + (temp->v_X1+temp->v_X2+3791)/pow(2,4);
	temp->v_Pressure = temp->v_p/100.0;
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_Altitude												********
// *****	This functions calculates the pressure altitude					********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_Altitude(alt_struct *temp)
{
	int i;
	double x, y, sum;
	double T_o = 288.15;
	double L = -0.0065;
	double R = 287.053;
	double g = 9.80665;
	for(i=4;i>0;i--)
	{
		temp->v_z[i] = temp->v_z[i-1];
	}
	x = temp->v_p/(temp->v_init_pressure*100);
	y = -L*R/g;
	temp->v_z[0] = (T_o/L*(pow(x,y) - 1))*3.28;
	for(i=0;i<5;i++)
	{
		sum = sum + temp->v_z[i];
	}
	temp->v_Altitude = sum/5.0;
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_FR													********
// *****	This functions calculates Fall Rate								********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_FR(alt_struct *temp)
{
	temp->v_FR[FR_count] = (temp->v_z[1] - temp->v_z[0])/0.25;
	
	if(FR_count == 3)
	{
		FR_count = 0;
	}
	else
	{
		FR_count++;
	}
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_FRa													********
// *****	This functions calculates average fall rate per second			********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_FRa(alt_struct *temp)
{
	int i;
	double sum = 0;
	if(FR_count == 0)
	{
		for(i=9;i>0;i--)
		{
			temp->v_FRa[i] = temp->v_FRa[i-1];
		}
		for(i=0;i<4;i++)
		{
			sum = sum + temp->v_FR[i];
		}
		temp->v_FRa[0] = sum/4.0;
	}
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Calculate_FRma													********
// *****	This functions calculates a 10 sec. moving average fall rate 	********
// *****	with an update rate of 1 Hz.									********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Calculate_FRma(alt_struct *temp)
{
	int i;
	double sum = 0;
	if(FR_count == 0)
	{
		for(i=0;i<3;i++)
		{
			sum = sum + temp->v_FRa[i];
		}
		temp->v_FRma = sum/3.0;
	}
	return;
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Set_Status														********
// *****	This functions Sets the fall rate status					 	********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
void Set_Status(alt_struct *temp)
{
	switch (temp->v_status)
	{
		case 0:	// Startup
		{
			if(temp->v_FRma <= LAUNCH_THRESHOLD)
			{
				if(count <= 8) count++;
				if(count >= 8)
				{
					printf("Launch Detected!!!!!!!!!!!!!!!!!!\n");
					temp->v_status = 1;
					count = 0;
				}
			}
			break;
		}
		case 1:	// Launched
		{
			if(temp->v_FRma >= 0)
			{
				if(count <= 4) count++;
				if(count >= 4)
				{
					printf("Apogee Detected!!!!!!!!!!!!!!!!!!\n");
					temp->v_status = 2;
					count = 0;
					v_record_data = 1;
				}
			}
			break;
		}
		case 2:	// Decent on Drogue
		{
			if(temp->v_FRma <= 30)
			{
				if(count <= 8) count++;
				if(count >= 8)
				{
					printf("Main Chute Deployment Detected!!!!!!!!!!!!!!!!!!\n");
					temp->v_status = 3;
					count = 0;
					system("echo 1 > /sys/class/gpio/gpio147/value");
					v_tx_gps = 1;
				}
			}
			break;
		}
		case 3:	// Decent on Main
		{
			if(temp->v_FRma <= 5)
			{
				if(count <= 5) count++;
				if(count >= 5)
				{
					printf("Landing Detected!!!!!!!!!!!!!!!!!!\n");
					temp->v_status = 4;
					count = 0;
				}
			}
			break;
		}
		case 4:  // Landed
		{
			if(count <= 1200) count++;
			if(count >= 1200)
			{
				printf("Transmitting Data!!!!!!!!!!!!!!!!!!\n");
				v_tx_data = 1;
				v_record_data = 0;
				count = 0;
			}
		}
		default:
		{
			break;
		}
	}
}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	gettimediff														********
// *****	returns time difference in micro seconds						********
// *****	(r) void														********
// ***** 	(1) alt_struct *: pointer to alt structure			            ********
// *********************************************************************************
double gettimediff(struct timeval *start, struct timeval *end, double div_factor)
{
	double returnvalue;
	returnvalue = (end->tv_sec - start->tv_sec)*1000000 + (end->tv_usec - start->tv_usec);
	return (returnvalue/div_factor);
}
// ----------  END  ----------------------------------------------------------------


// ----------  END  ----------------------------------------------------------------
#endif /* _GUM  */



