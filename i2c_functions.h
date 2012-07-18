// *********************************************************************************
// *****	i2c_functions.h													********
// *****	Christopher Crews	-	21 July 2010							********
// *********************************************************************************
#ifdef _GUM
#ifndef _I2CFCTN
#define _I2CFCTN

// *********************************************************************************
// *****	Include Statments	 											********
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Constants			 											********
#define SKIP_WRITE_REGISTER	0XFF
#define ON	1
#define OFF	0
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Function Prototypes 											********

// *****  i2c_print_messages  *****
// (1) int : 1 to print messages 0 to not print.
void i2c_print_messages(int);

// *****  i2c_write  *****
// return 1 if successful or 0 on error
// (1) device number that is returned from i2c_ipen
// (2) Unsigned char = Peripheral Address
// (3) Unsigned char = Register Address to write data to
// (4) char array holding the data to write to the peripheral
// (5) number of bytes of data to write to the peripheral
int i2c_write(int, unsigned char, unsigned char, unsigned char [], int);

// *****  i2c_read  *****
// return 1 if successful or 0 on error
// (1) device number that is returned from i2c_ipen
// (2) Unsigned char = Peripheral Address
// (3) Unsigned char = Register Address to read data from
// (4) char array holding the data to hold data read from the peripheral
// (5) number of bytes of data to read from the peripheral
int i2c_read(int, unsigned char, unsigned char, unsigned char [], int);

// *****  i2c_open  *****
// return device number or 0 if error
int i2c_open(void);

// *****  i2c_close  *****
// (1) device number that is returned from i2c_open
void i2c_close(int);
// --------  END  ------------------------------------------------------------------

#endif  /* _I2CFCTN  */
#endif  /* _GUM  */
