/*
 * main.c
 *
 * Poslednje_izmene: 30/03/2016 16:07:00
 * Autor: AXIS team 
 
 Izmene:

 
 
 Potrebne izmene:
Sve
 */ 

#include <avr/io.h>
#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/port_driver.h"
#include "Headers/adc_driver.h"
#include "math.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/funkcije.h"


int main(void)
{
	Podesi_Oscilator();					//podesavanje oscilatora
	Podesi_Pinove();					//podesavanje I/O pinova
	//PodesiADC();						//podesavanje AD konvertora
	Podesi_Tajmere();					//podesavanje tajmera
	Podesi_Interapt();					//podesavanje interapt prioriteta
	Podesi_USART_Komunikaciju();		//podesavanje komunikacije
	_delay_ms(2000);
// 	sendChar('k');
	//PORT_SetPins(&PORTK,0xFF);
	
	while(1){
		//sendChar_USB('L');
		//sendChar_USB('a');
		//sendChar_USB('z');
		//sendChar_USB(0xFF);
		//sendChar_USB(0xEF);
		//_delay_ms(3000);	
	}
	
	
	while(1){
		//sendChar_USB(0xFF);		
		//sendChar('k');
		//sendChar('k');
		//sendChar('k');
   		if (sima==1)
   		{
	   		_delay_ms(5000);
	   		sendChar(0xEF);
			   sima=0;
	   		
   		}
		//PORT_SetPins(&PORTC, 0xFF);
		
		char s=1;
		
		_delay_ms(500);
		if (s==1)
		{
			PORT_SetPins(&PORTK,0xFF);
			s=0;
		}
		
		_delay_ms(500);
		
		if (s==0)
		{
			PORT_ClearPins(&PORTK,0xFF);
			s=1;
		}
		
		
	}
}//main
