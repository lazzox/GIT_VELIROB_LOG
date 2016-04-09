/*
 * taktike.c
 *
 * Created: 08-Apr-16 18:24:23
 *  Author: Sirius-PC
 */ 

#include "Headers/avr_compiler.h"
#include "Headers/port_driver.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/taktike.h"
#include "Headers/globals.h"
#include "Headers/usart_driver.h"


volatile char poslata_komanda = 0;
volatile int korak = 0;
volatile int korak2 = 0;
volatile unsigned int TIMED_OUT_VREME = 200;

int idi_pravo(unsigned int x, unsigned int y, unsigned int ugao)
{
	switch(korak2){
		case 0:
			SendChar('A',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar((ugao>>8),&USART_XDRIVE);
			SendChar(ugao,&USART_XDRIVE);
			SendChar('X',&USART_XDRIVE);
									
			//TIMED_OUT_VREME += sys_time;
			//poslata_komanda = 1;
			//vreme_primanja = 0; //resetuj vreme primanja
			korak2++;
			break;
		case 1:
			SendChar('L', &USART_XM);
				if (okay_flag){
					SendChar('A', &USART_XM);
					korak2++;
					okay_flag = 0;
				}
			break;
		
		case 2:
				SendChar('J', &USART_XM);
				if(stigao_flag) //Uspesno poslao komandu robot ide po komandi
				{
					SendChar('O', &USART_XM);
					//poslata_komanda = 1; //resetuje flag
					stigao_flag = 0;
					korak2 = 0;
					return 1;
				}
			break;
		default: 
			return 0;
			break;
		}//switch
}

int idi_nazad(unsigned int x, unsigned int y, unsigned int ugao)
{
	switch(korak2){
		case 0:
		SendChar('B',&USART_XDRIVE);
		SendChar((x>>8),&USART_XDRIVE);
		SendChar(x,&USART_XDRIVE);
		SendChar((y>>8),&USART_XDRIVE);
		SendChar(y,&USART_XDRIVE);
		SendChar((ugao>>8),&USART_XDRIVE);
		SendChar(ugao,&USART_XDRIVE);
		SendChar('X',&USART_XDRIVE);
		
		//TIMED_OUT_VREME += sys_time;
		//poslata_komanda = 1;
		//vreme_primanja = 0; //resetuj vreme primanja
		korak2++;
		break;
		case 1:
		if (okay_flag){
			korak2++;
			okay_flag = 0;
		}
		break;
		
		case 2:
		if(stigao_flag) //Uspesno poslao komandu robot ide po komandi
		{
			//poslata_komanda = 1; //resetuje flag
			stigao_flag = 0;
			korak2 = 0;
			return 1;
		}
		break;
		default:
		return 0;
		break;
	}//switch
	
	return 0;
}

void taktika_1(void)
{
	switch (korak)
	{
		case 0:
			//SendChar('0', &USART_XM);
			//SendChar('0', &USART_XM);
			if(idi_pravo(500,0,0)) 
			{
				
				korak++;
				
			}
		break;
		
		case 1:
			//SendChar('1', &USART_XM);
			if(idi_nazad(0,0,0)) 
			{
				korak++;
				SendChar('Z', &USART_XM);
			}
		break;
		
		default:
			break;
	}
}