// *********************************************************************************
// *****	Christopher Crews	-	February 2012							********
// *****	Program Name:		main.c										********
// *****	main - initalizes all the hardware and veriables.  it sets up	********
// *****	and starts all the threads.	this c file also holds the Main_Thread.*****
// *********************************************************************************
// *****             Formated for Monospaced Type							********
#ifdef _GUM

// *********************************************************************************
// *****	Include Statments	 											********
#include "main.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Global Variables	 											********
int v_i2c_device, v_uart_device;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Main															********
// *********************************************************************************
int main (int argc, char **argv)
{

// *********************************************************************************
// *****	Variable Declaratins 											********
	int v_returnvalue;
	pthread_t main_thread_ID;
	pthread_t gps_thread_ID;
	pthread_t alt_thread_ID;
	pthread_t humidity_thread_ID;
	pthread_t temperature_thread_ID;
	pthread_t uv_thread_ID;
	pthread_t solar_thread_ID;
	pthread_t hb_thread_ID;
// ----------  END  ----------------------------------------------------------------

	// Set system CPU frequency
	system("cpufreq-set -f 600MHz");
	
// *********************************************************************************
// *****	Initalize GPIO pins												********
	system("echo 147 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio147/direction");
	system("echo 0 > /sys/class/gpio/gpio147/value");
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	OPEN I2C PORT													********
	v_i2c_device = i2c_open();
	i2c_print_messages(OFF);
	if (v_i2c_device == 0)
	{
		printf("error on opening i2c port...exiting program.\n\n");
		return 0;
	}
// ----------  END  ----------------------------------------------------------------
 
// *********************************************************************************
// *****	Save I2C device ID in all files that use I2C					********
	save_alt_i2c_device_id(v_i2c_device);
	save_gps_i2c_device_id(v_i2c_device);
	save_temperature_i2c_device_id(v_i2c_device);
	save_humidity_i2c_device_id(v_i2c_device);
	save_uv_i2c_device_id(v_i2c_device);
	save_solar_i2c_device_id(v_i2c_device);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	OPEN UART PORT													********
	v_uart_device = uart_open();
	uart_print_messages(OFF);
	if (v_uart_device == 0)
	{
		printf("error on opening uart port...exiting program.\n\n");
		return 0;
	}
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	create threads													********
	v_returnvalue = pthread_create(&main_thread_ID, NULL, main_thread, NULL);
	v_returnvalue = pthread_create(&gps_thread_ID, NULL, GPS_Thread, NULL);
	v_returnvalue = pthread_create(&alt_thread_ID, NULL, Alt_Thread, NULL);
	v_returnvalue = pthread_create(&humidity_thread_ID, NULL, Humidity_Thread, NULL);
	v_returnvalue = pthread_create(&temperature_thread_ID, NULL, Temperature_Thread, NULL);
	v_returnvalue = pthread_create(&uv_thread_ID, NULL, UV_Thread, NULL);
	v_returnvalue = pthread_create(&solar_thread_ID, NULL, Solar_Thread, NULL);
	v_returnvalue = pthread_create(&hb_thread_ID, NULL, Hb_Thread, NULL);
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Closing Code													********
	pthread_join(main_thread_ID, NULL);
// ----------  END  ----------------------------------------------------------------

	return 0;
}
// ----------  END MAIN ------------------------------------------------------------


// *********************************************************************************
// *****	Function Name:  main_thread										********
// *****	Reads all data output from all the other threads				********
// *********************************************************************************
void *main_thread(void *ptr)
{
// *********************************************************************************
// *****	Veriable Declaratins 											********	
	gps_struct strc_gps_data;
	alt_struct strc_alt_data;
	humidity_struct strc_humidity_data;
	temperature_struct strc_temperature_data;
	uv_struct strc_uv_data;
	solar_struct strc_solar_data;
	printall = 0;
	int printall_solar = 0;
	printall_gps = 0;
	FILE *pFile;
	SDL_Event Event;
	struct timeval start;
	struct timeval start2;
	struct timeval start3;
	struct timeval end;
	struct timeval end2;
	struct timeval end3;
	int timediff;
	int timediff2;
	int timediff3;
	int time_count = 0;
	unsigned char v_string[2000], v_temp_string[2000];
	int ret;
	v_record_data = 0;
	v_tx_gps = 0;
	v_landed = 0;
	v_tx_data = 0;
	int v_wifi_off = 1;
// ----------  END  ----------------------------------------------------------------

	SDL_Init(SDL_INIT_EVERYTHING);

	gettimeofday(&start,NULL);
	gettimeofday(&start2,NULL);
	gettimeofday(&start3,NULL);

// *********************************************************************************
// *****	MAIN LOOP														********
	for(;;)
	{
		usleep(100000);
		
		if(v_record_data == 1)
		{
		gettimeofday(&end,NULL);
		timediff = (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);
		if(timediff >= 500000) 
		{
			pFile = fopen("//home/root/Programs/results","a");
			fprintf(pFile,"$CHDAT,%d/%d/20%d,",strc_gps_data.day,strc_gps_data.month,strc_gps_data.year);
			fprintf(pFile,"%d:%d:%d,",strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds);
			fprintf(pFile,"%f,",strc_gps_data.v_latitude);
			fprintf(pFile,"%f,",strc_gps_data.v_longitude);
			fprintf(pFile,"%f,",strc_solar_data.v_Solar);
			fprintf(pFile,"%f,",strc_uv_data.v_UV);
			fprintf(pFile,"%f,",strc_humidity_data.v_Humidity);
			fprintf(pFile,"%f,",strc_alt_data.v_Pressure);
			fprintf(pFile,"%f\n",strc_temperature_data.v_Temperature);
			fclose(pFile);
			gettimeofday(&start,NULL);
		}
		}
		
		if(v_tx_gps == 1)
		{
		gettimeofday(&end2,NULL);
		timediff2 = end2.tv_sec - start2.tv_sec;
		if(timediff2 >= 5)
		{
			sprintf(v_string,"$CHGPS,%07f,%07f,%02d:%02d:%02d,%d,%d,%d\n",strc_gps_data.latitude, strc_gps_data.longitude, strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds, strc_gps_data.fix_type,strc_gps_data.num_sats,strc_gps_data.data_valid);
			ret = uart_write(v_uart_device,v_string);
			gettimeofday(&start2,NULL);
		}
		}
		
		if(v_wifi_off == 1)
		{
		gettimeofday(&end3,NULL);
		timediff3 = end3.tv_sec - start3.tv_sec;
		if(timediff3 >= 60)
		{
			system("echo 0 > /sys/class/gpio/gpio16/value");
			v_wifi_off = 0;	
		}
		}
		
		if(v_tx_data == 1)
		{
			pFile = fopen("//home/root/Programs/results","r");
			while((fscanf(pFile,"%s",v_temp_string)) != EOF)
			{
				sprintf(v_string,"%s\n",v_temp_string);
				ret = uart_write(v_uart_device,v_string);
				usleep(60000);
			}
			fclose(pFile);
			v_tx_data = 0;
		}
		
		if(new_gps_data == 1)
		{
			Get_GPS_Data(&strc_gps_data);
			if(printall_gps)
			{
			printf("\nLat = %.06f,  Long = %.06f\n",strc_gps_data.latitude,strc_gps_data.longitude);
			printf("Altitude = %.02f m, Speed = %.02f km/h\n",strc_gps_data.altitude,strc_gps_data.ground_speed);
			printf("Course over Ground = %.02f deg.\n",strc_gps_data.ground_course);
			printf("Number of Satellites = %d\n", strc_gps_data.num_sats);
			printf("Date: %d/%d/%d \n",strc_gps_data.day,strc_gps_data.month,strc_gps_data.year);
			printf("UTC Time: %d:%d:%d \n",strc_gps_data.hours,strc_gps_data.minutes,strc_gps_data.seconds);
			printf("Fix Type: %d\n\n", strc_gps_data.fix_type);
			}
		}
		
		if(new_alt_data == 1)
		{
			Get_Alt_Data(&strc_alt_data);
			if(printall)
			{
			printf("Barometric Pressure = %f mbars\n",strc_alt_data.v_Pressure);
			printf("Altitude = %f ft\n",strc_alt_data.v_Altitude);
			}
		}
		
		if(new_humidity_data == 1)
		{
			Get_Humidity_Data(&strc_humidity_data);
			if(printall)
			{
			printf("Relative Humidity = %f %% \n",strc_humidity_data.v_Humidity);
			}
		}
		
		if(new_temperature_data == 1)
		{
			Get_Temperature_Data(&strc_temperature_data);
			if(printall)
			{
			printf("Temperature = %f degrees C\n",strc_temperature_data.v_Temperature);
			}
			v_temperature_c = strc_temperature_data.v_Temperature;
		}
		
		if(new_uv_data == 1)
		{
			Get_UV_Data(&strc_uv_data);
			if(printall)
			{
			printf("UV = %f\n",strc_uv_data.v_UV);
			}
		}
		
		if(new_solar_data == 1)
		{
			Get_Solar_Data(&strc_solar_data);
			if(printall_solar)
			{
			printf("Solar = %f\n",strc_solar_data.v_Solar);
			}
		}

		if(SDL_PollEvent(&Event)!=0)
		{
			if(Event.type==SDL_QUIT)
			{
				break;
			}
			
		}
	
	}
// ----------  END  ----------------------------------------------------------------
	i2c_close(v_i2c_device);
	uart_close(v_uart_device);
	SDL_Quit();
	exit(0);

}
// ----------  END MAIN_THREAD  ----------------------------------------------------

#endif /* _GUM  */

