// *********************************************************************************
// *****	hb.h															********
// *****	Christopher Crews	-	10 February 2012						********
// *********************************************************************************
#ifdef _GUM
#ifndef _HBFCTN
#define _HBFCTN


// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
// ----------  END  ----------------------------------------------------------------


// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************


// ----------  END  Constants  -----------------------------------------------------



// *********************************************************************************
// *****	Primary Function Prototypes 									********
// *********************************************************************************

// *****  Hb_Thread  *****
// return: void
// (1) argument type: void pointer
void *Hb_Thread(void *ptr);
// --------  END  ------------------------------------------------------------------

// ----------  END  Primary Function Prototypes  -----------------------------------


// *********************************************************************************
// *****	Supporting Functions Prototypes									********
// *********************************************************************************


// --------  END  Supporting Functions Prototypes-----------------------------------

#endif  /* _UFCTN  */
#endif  /* _GUM  */
