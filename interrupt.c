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
	vreme_primanja++;
}

ISR(TCF0_CCA_vect)
{
}

ISR(TCF0_OVF_vect)
{
}


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
	SendChar('x', &USART_XM);
	USART_RXComplete(&USART_E1_data);
	receiveArray[RX_i_E1] = USART_RXBuffer_GetByte(&USART_E1_data);
	RX_i_E1++;
	vreme_primanja = 0;
	
	if(RX_i_E1 > 0){ //Primljeni podaci i spremni za obradu
		switch(receiveArray[0]){
			case 'A':
				if(receiveArray[8] == 'X'){ //idi u tacku primljeno!
					//parsiraj ovde sve
					SendChar(receiveArray[1],&USART_LCD);
					SendChar(receiveArray[2],&USART_LCD);
					SendChar(receiveArray[3],&USART_LCD);
					SendChar(receiveArray[4],&USART_LCD);
					SendChar(receiveArray[5],&USART_LCD);
					SendChar(receiveArray[6],&USART_LCD);
					SendChar(receiveArray[7],&USART_LCD);
					//parsiraj ovde sve
					RX_i_E1 = 0;
				}
			break;
			
			
			case 'I':
				if(receiveArray[5] == 'D'){
					//parsiraj ovde sve
					RX_i_E1 = 0;
				}
				break;
				
			case 'O':
			SendChar('o', &USART_XM);
				okay_flag = 1;
				RX_i_E1 =0;	
				break;
			
			
			default:
				break;
			
		}		
	}

}