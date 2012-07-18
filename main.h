// *********************************************************************************
// *****	main.h															********
// *****	Christopher Crews	-	March 2012								********
// *********************************************************************************
#ifndef _MAINTHREAD
#define _MAINTHREAD

// *********************************************************************************
// *****	Include Statments	 											********
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_framerate.h>
#include <signal.h>
#include "program_constants.h"
#include "i2c_functions.h"
#include "uart_functions.h"
#include "adc_functions.h"
#include "gps.h"
#include "alt.h"
#include "humidity.h"
#include "temperature.h"
#include "uv.h"
#include "solar.h"
#include "hb.h"
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Variable Declarations											********
int printall, printall_gps;
int v_tx_gps, v_record_data, v_landed, v_tx_data;
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Function Prototypes 											********

// *****  < main_thread >  *****
// return: void
// (1) argument type: void pointer
void *main_thread(void *ptr);

// --------  END  ------------------------------------------------------------------

#endif  /* _MAINTHREAD  */
