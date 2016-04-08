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
#include "math.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/funkcije.h"


int main(void)
{
	Podesi_Oscilator();					//podesavanje oscilatora
	Podesi_Pinove();					//podesavanje I/O pinova
	Podesi_Tajmere();					//podesavanje tajmera
	Podesi_Interapt();					//podesavanje interapt prioriteta
	Podesi_USART_Komunikaciju();		//podesavanje komunikacije
	
	_delay_ms(1500);
	vreme_primanja = 0;
	
	//SendChar('A', &USART_XDRIVE); //0
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('1', &USART_XDRIVE);
	//SendChar('X', &USART_XDRIVE);
	//SendChar('a', &USART_LCD);
	//sendMsg("READY TO REC", &USART_LCD);
	
	//sendMsg("A1234567X", &USART_XDRIVE);
	//sendMsg("A1234567X", &USART_XDRIVE);
	idi_pravo(500,0,0);
	
	while(1){
		if (vreme_primanja > 200){
			vreme_primanja = 0;
			RX_i_E1 = 0;
		}
	
		if(okay_flag == 1){
			sendMsg("OKAY", &USART_LCD);
			okay_flag = 2;
		}
	}
}//main
