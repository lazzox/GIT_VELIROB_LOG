/*
 * interrupt.c
 *
 * Created: 4/19/2011 2:15:28 PM
 *  Author: robert kovacs
 */ 
#include <avr/io.h>
#include "Headers/avr_compiler.h"
#include "Headers/adc_driver.h"
#include "Headers/usart_driver.h"
#include "Headers/TC_driver.h"
#include "Headers/globals.h"
#include "Headers/port_driver.h"
#include "Headers/mechanism.h"
#include "math.h"





//interrupt tajmera
ISR(TCE1_OVF_vect)	//1.5ms 
{	

}

ISR(TCF0_CCA_vect)
{
}

ISR(TCF0_OVF_vect)
{
	
}

ISR(USARTE1_RXC_vect)
{
	USART_RXComplete(&USART_E1_data);
	receiveArray[RX_i_E1] = USART_RXBuffer_GetByte(&USART_E1_data);
	//USART_TXBuffer_PutByte(&USART_C0_data, receiveArray[RX_i_E1]);	//echo
	sendChar_USB(receiveArray[RX_i_E1]);
	RX_i_E1++;///
	
}