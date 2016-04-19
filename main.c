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
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/taktike.h"
 

int main(void)
{
	Podesi_Oscilator();					//podesavanje oscilatora
	Podesi_Pinove();					//podesavanje I/O pinova
	Podesi_Tajmere();					//podesavanje tajmera
	Podesi_Interapt();					//podesavanje interapt prioriteta
	Podesi_USART_Komunikaciju();		//podesavanje komunikacije
	
	_delay_ms(1000);
	
	tajmer=0;
	senzor_tajmer=0;
	vreme_primanja = 0;
	stigao_flag_pomocni=0;
	sys_time=0;

	//idi_pravo(300,0,90);
	
	RX_i_E1 = 0;
	
	TIMED_OUT_VREME = 200;
	korak = 0;
	korak2 = 0;
	okay_flag=0;
	stigao_flag=0;
	stigao_flag2 =0;
	sys_time=0;
	sendMsg("LOGIKA", &USARTD1);
	
	_delay_ms(500);
	
	
	//while (1)
	//{
		//if(PORTJ.IN & 0b00000001)
		//{
			//sendMsg("KEFA",&USARTD1);
		//}
		//else{
			//sendMsg("CILO",&USARTD1);
		//}
		//_delay_ms(1000);
	//}
	
	while(1){
		if (vreme_primanja > 100){
			vreme_primanja = 0;
			RX_i_E1 = 0;
		}

 	if (senzor_tajmer>150)
 	{
	 	senzor_stop();
	 	senzor_tajmer=0;
 	}
 		
		//taktika_kocka();
		taktika_1();
		
		
	
	}
}//main

