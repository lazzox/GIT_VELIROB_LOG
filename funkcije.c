/*
 * funkcije.c
 *
 * Created: 26/03/16 00:43:50
 * Author: Team Axis
 */ 

#include <avr/io.h>
#include "Headers/funkcije.h"
#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/port_driver.h"
#include "Headers/adc_driver.h"
#include "math.h"
#include "Headers/globals.h"
#include "Headers/mechanism.h"
#include "Headers/hardware.h"



void sendMsg(char *poruka)
{
	while(*poruka != '\0'){
		sendChar(*poruka);
		poruka++;
	}
}

void sendChar(char c)
{
	USARTE0.DATA = c;
	while(!(USARTE0.STATUS & (1 << 5)));
}