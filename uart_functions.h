// *********************************************************************************
// *****	uart_functions.h												********
// *****	Christopher Crews	-	12 November 2011						********
// *********************************************************************************
#ifdef _GUM
#ifndef _UARTFCTN
#define _UARTFCTN

// *********************************************************************************
// *****	Include Statments	 											********
// *********************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Constants			 											********
// *********************************************************************************
#define SKIP_WRITE_REGISTER	0XFF
#define ON	1
#define OFF	0
// ----------  END  ----------------------------------------------------------------

// *********************************************************************************
// *****	Function Prototypes 											********
// *********************************************************************************

// *****  uart_print_messages  *****
// (1) int : 1 to print messages 0 to not print.
void uart_print_messages(int);

// *****  uart_write  *****
// return (0) if successful or (-1) on error
// (1) int: device number that is returned from uart_open
// (2) Unsigned char array: holds the data to write
// (3) int: number of bytes to write
int uart_write(int, unsigned char []);

// *****  uart_read  *****
// return (0) if successful, (-1) on error, or (num_bytes_read) if it differs
// (1) int: device number that is returned from uart_open
// (2) Unsigned char array: will hold data that is read
// (3) int: number of bytes of data to read
int uart_read(int, unsigned char []);

// *****  uart_open  *****
// return device number or 0 if error
int uart_open(void);

// *****  uart_close  *****
// (1) device number that is returned from uart_open
void uart_close(int);
// --------  END  ------------------------------------------------------------------

#endif  /* _UARTFCTN  */
#endif  /* _GUM  */
