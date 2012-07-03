// *********************************************************************************
// *****	i2c_functions.c													********
// *****	Richard Manley	-	21 July 2010								********
// *****	device i2c_open(void)	// opens i2c port						********
// *****	void i2c_close(device)	// closes i2c port						********
// *****	int i2c_read(device, address, register, data array, data count)	********
// *****	int i2c_read(device, address, register, data array, data count) ********
// *********************************************************************************
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
#include "i2c_functions.h"
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	Global Veriables	 											********
int v_print_all;
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	i2c_print_messages												********
// *****	prints all messages in the functions and the error messages		********
void i2c_print_messages(int msg)
{
	if (msg)
	{
		v_print_all = ON;
	}
	else
	{
		v_print_all = OFF;
	}
}



// *********************************************************************************
// *****	i2c_open														********
// *****	Opens the i2c Port and return the Device Number					********
int i2c_open(void)
{
	int v_device;
	v_device = open("/dev/i2c-3", O_RDWR);
	if (v_device < 0)
	{
		if(v_print_all)	perror("error on open");
		return 0;
	}
	// ***** Device Opened *****
	if(v_print_all)	printf("I2C Device Opened....\n\n");
	return v_device;
}
// ---------- END  -----------------------------------------------------------------



// *********************************************************************************
// *****	i2c_close														********
// *****	closes the i2c Port												********
void i2c_close(int v_device)
{
	close(v_device);
}
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	i2c_write 														********
// *****	writes character array out the i2c port							********
// *********************************************************************************
int i2c_write(int v_device, unsigned char v_Address, unsigned char v_Register, unsigned char v_Data[], int v_data_count)
{
	// ***** Inserting Register Address in Beginning of Data Array *****
	int i;
	
	v_data_count++;
	unsigned char *v_temp_array = malloc (sizeof (unsigned char) * (v_data_count));
	v_temp_array[0] = v_Register;
	for(i=1; i <= v_data_count; i++)
	{
		v_temp_array[i] = v_Data[i-1];
	}
	
 	// ***** writes out the address of the slave *****
	if (ioctl(v_device, I2C_SLAVE, v_Address/2) < 0)
	{
		if(v_print_all)	perror("ioctl (I2C_Slave)");
		if(v_print_all)	printf("error in addressing\n");
		return(0);
	}
	
	// ***** Rights Out Address and Registry Data *****
	if(v_print_all)	printf("Writting to address: %#04X.\n",v_Address);
	if (write(v_device, v_temp_array, v_data_count) != v_data_count)
	{
		if(v_print_all)	printf("failed at write!!\n");
		if(v_print_all)	perror("");
		return(0);
	}
	// ***** Free-up allocated memory and exit function *****
	if(v_print_all)	printf("All data written sucsessfuly.\n\n");
	free(v_temp_array);
	return 1;
}
// ---------- END  -----------------------------------------------------------------



// *********************************************************************************
// *****	i2c_read														********
// *****	reads character array in from the i2c port						********
int i2c_read(int v_device, unsigned char v_Address, unsigned char v_Register, unsigned char v_Data[], int v_data_count)
{
	// ***** writes out the address of the slave *****
	if (ioctl(v_device, I2C_SLAVE, v_Address/2) < 0)
	{
		if(v_print_all)	perror("ioctl (I2C_Slave)");
		if(v_print_all)	printf("error in addressing\n");
		return(0);
	}
	
	
	// ***** Writing to Register to Start Read from Right OLED *****
	if (v_Register != SKIP_WRITE_REGISTER)
	{
		if(v_print_all)	printf("Writing to device to Start Read.\n");
		if (write(v_device, &v_Register, 1) != 1)
		{
			if(v_print_all)	printf("failed at write!!\n");
			if(v_print_all)	perror("");
			return(0);
		}
	}
	
	// ***** print number of bytes, address, and register *****
	if (v_Register != SKIP_WRITE_REGISTER)
	{
		if(v_print_all)	printf("Reading %d bytes from device Address %#04X starting at Register %#04X.\n"\
		,v_data_count, v_Address, v_Register);
	}else
	{
			if(v_print_all)	printf("Reading %d bytes from device Address %#04X.\n",v_data_count, v_Address);
	}
	// ***** Starting Read  *****
	if(read(v_device, v_Data, v_data_count) != v_data_count)
	{
		if(v_print_all)	perror("read error");
		return(0);
	}			
	
	return 1;	
}
// ---------- END  -----------------------------------------------------------------

#endif /* _GUM  */


