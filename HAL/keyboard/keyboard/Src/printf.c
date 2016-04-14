/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the µVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>
#include "stm32f1xx_hal.h"
#include "usart.h"

#pragma import(__use_no_semihosting_swi)


extern int sendchar (int c);
extern int getkey   (void);


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {
	uint8_t b[1];
	b[0] = c;
	HAL_UART_Transmit(&huart1, b, 1, 10);
  return (c);
}


int fgetc(FILE *f) {
  //return (getkey());
	return 0;
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  uint8_t b[1];
	b[0] = c;
	HAL_UART_Transmit(&huart1, b, 1, 10);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
