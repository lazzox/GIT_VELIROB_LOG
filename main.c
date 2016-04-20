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
	PORTE.OUT = 0x01; //Za profi servo
	tajmer=0;
	senzor_tajmer=0;
	vreme_primanja = 0;
	stigao_flag_pomocni=0;
	sys_time=0;
	RX_i_E1 = 0;
	TIMED_OUT_VREME = 200;
	korak = 0;
	korak2 = 0;
	stigao_flag2 =0;
	Podesi_Oscilator();					//podesavanje oscilatora
	Podesi_Pinove();					//podesavanje I/O pinova
	Podesi_Tajmere();					//podesavanje tajmera
	Podesi_Interapt();					//podesavanje interapt prioriteta
	Podesi_USART_Komunikaciju();		//podesavanje komunikacije
	
	_delay_ms(2000);
	PORTE.OUT = 0x01; //Za service
	sendMsg("LOGIKA", &USARTD1);

	//ProfiServo(br. servoa, ugao, brzina) - ugao: 100 - 300; brzina: 100-600
	while(1){ //ZAKOMENTARISI OVAJ WHILE - KORISTIMO SAMO ZA AX12 TESTIRANJE
		//Za 5:
		//148
		//otvoreno 150 zatvoreno 60
		//hvataljka na velikom robotu
		//ispod 50 i iznad 250 ne sme
		//za 7:
		//150 i 240;
		//AX_12(7,150);
		///AX_12(5,70);

		//za 4:
		//230 odvaja
		//300
		
		ProfiServo(6,190,300);
		sendMsg("LOGIKA",&USARTD1);
		_delay_ms(2000);
		ProfiServo(6,20,300);
		_delay_ms(2000);
	}
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
		//TAKTIKE
		taktika_kocka();
	}
}//main

