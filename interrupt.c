/*
 * interrupt.c
 *
 * Created: 4/19/2011 2:15:28 PM
 *  Author: robert kovacs
 */ 
#include <avr/io.h>
#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/TC_driver.h"
#include "Headers/globals.h"
#include "Headers/port_driver.h"





//interrupt tajmera
ISR(TCE1_OVF_vect)	//1.5ms 
{	
	overflow_primanje++;
	vreme_primanja++;
	sys_time++;
	tajmer++;
	senzor_tajmer++;
}

//ISR(USARTE1_DRE_vect)
//{
	//USART_DataRegEmpty(&USART_E1_data);
//}

ISR(USARTE0_DRE_vect)
{
	USART_DataRegEmpty(&USART_E0_data);
}
ISR(USARTD1_DRE_vect)
{
	USART_DataRegEmpty(&USART_D1_data);
}
ISR(USARTC0_DRE_vect)
{
	USART_DataRegEmpty(&USART_C0_data);
}

ISR(USARTE1_RXC_vect)
{
	USART_RXComplete(&USART_E1_data);
	receiveArray[RX_i_E1] = USART_RXBuffer_GetByte(&USART_E1_data);
	//SendChar((char)RX_i_E1,&USART_XM);
	//SendChar(receiveArray[RX_i_E1], &USART_XM);
	RX_i_E1++;
	vreme_primanja = 0;
	
	
	if(RX_i_E1 > 7){ //Primljeni podaci i spremni za obradu
		switch(receiveArray[0]){
			case 'I':
				if(receiveArray[7] == 'D'){
					//parsiraj ovde sve
					RX_i_E1 = 0;
				}
				break;
				
			case 'O': //O - OKAY FLAG (PRIMIO SAM PORUKU) 
				if(receiveArray[7] == 'K'){
					okay_flag = 1;
					RX_i_E1 =0;	
				}
				break;
				
			case 'S': //TU - Kao TU SAM (STIGAO SAM)
				if(receiveArray[7] == 'T'){
					if (stigao_flag2) stigao_flag = 1;
					RX_i_E1 = 0;
				}
				break;
				
			default:
				break;
			
		}		
	}

}