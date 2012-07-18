// *********************************************************************************
// *****	uart_functions.c												********
// *****	Christopher Crews	-	12 November 2011						********
// *********************************************************************************
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include "uart_functions.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Veriables	 											********
// *********************************************************************************
int v_print_all;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	i2c_print_messages												********
// *****	prints all messages in the functions and the error messages		********
// *********************************************************************************
void uart_print_messages(int msg)
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
// ---------- END ------------------------------------------------------------------



// *********************************************************************************
// *****	uart_open														********
// *****	Opens the uart Port and return the Device Number				********
// *********************************************************************************
int uart_open(void)
{
	int v_device;
	struct termios options;
	v_device = open("/dev/ttyO0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (v_device < 0)
	{
		if(v_print_all)	perror("Error opening uart port");
		return 0;
	}
	else
	{
		// ***** Device Opened *****
		if(v_print_all)	printf("UART Device Opened!\n\n");
	}
	
	/* set baud rate to 38400 */
	fcntl(v_device, F_SETFL, 0);
	tcgetattr(v_device, &options);
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	
	options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
	options.c_cflag |= (CLOCAL | CREAD | CS8);
	
	options.c_lflag &= ~(ECHO | ECHOE | ISIG);
	options.c_lflag |= ICANON;
	
	options.c_iflag &= ~(IXON | IXOFF | IXANY | INPCK);
	options.c_iflag |= ICRNL;
	
	options.c_oflag &= ~OPOST;
	
	tcsetattr(v_device, TCSANOW, &options);
	
	return v_device;
}
// ---------- END  -----------------------------------------------------------------



// *********************************************************************************
// *****	uart_close														********
// *****	closes the uart Port											********
// *********************************************************************************
void uart_close(int v_device)
{
	close(v_device);
}
// ----------  END  ----------------------------------------------------------------



// *********************************************************************************
// *****	uart_write 														********
// *****	writes character array out the uart port						********
// *********************************************************************************
int uart_write(int v_device, unsigned char v_data[])
{
	int i = 0;
	int v_data_count = 0;
	unsigned char *v_temp_array;
	
	for(;;)
	{
		if(v_data[i] == '\n')
		{
			v_data_count = i+1;
			break;
		}
		i++;
	}
	v_temp_array = (unsigned char *)malloc (sizeof (unsigned char) * (v_data_count));
	
	for(i=0; i <= v_data_count; i++)
	{
		v_temp_array[i] = v_data[i];
	}
	

	if (write(v_device, v_temp_array, v_data_count) != v_data_count)
	{
		if(v_print_all)	printf("failed at write!!\n");
		if(v_print_all)	perror("");
		free(v_temp_array);
		return(-1);
	}
	else
	{
		// ***** Free-up allocated memory and exit function *****
		if(v_print_all)	printf("All data written sucsessfuly.\n\n");
	}
	
	free(v_temp_array);
	return 0;
}
// ---------- END  -----------------------------------------------------------------



// *********************************************************************************
// *****	uart_read														********
// *****	reads character array in from the uart port						********
// *********************************************************************************
int uart_read(int v_device, unsigned char v_data[])
{
	int v_bytes_read, v_max_bytes;
	
	v_max_bytes = sizeof(v_data);
	
	// ***** Starting Read  *****
	v_bytes_read = read(v_device, v_data, v_max_bytes);
	if(v_bytes_read < 0)
	{
		if(v_print_all)	perror("read error");
		return(-1);
	}
	else
	{
		return v_bytes_read;
	}	
}
// ---------- END  -----------------------------------------------------------------

#endif /* _GUM  */


