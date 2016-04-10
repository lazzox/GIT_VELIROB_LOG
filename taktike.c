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


void idi_pravo(unsigned int x, unsigned int y, unsigned int ugao)
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
			overflow_primanje = 0;
			korak2++;
		break;
		
		case 1:
			if(okay_flag){
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;
			}
			else if(overflow_primanje > 200)
			{
				sendMsg("OVERFLOW",&USART_XM);
				
			}
		break;
		
		case 2:
			if(stigao_flag){
				stigao_flag = 0;
				korak2++;
			}	
		break;
		
		default:
			break;
	}
	
	
	//switch(korak2){
		//case 0:
			//SendChar('A',&USART_XDRIVE);
			//SendChar((x>>8),&USART_XDRIVE);
			//SendChar(x,&USART_XDRIVE);
			//SendChar((y>>8),&USART_XDRIVE);
			//SendChar(y,&USART_XDRIVE);
			//SendChar((ugao>>8),&USART_XDRIVE);
			//SendChar(ugao,&USART_XDRIVE);
			//SendChar('X',&USART_XDRIVE);		
			//TIMED_OUT_VREME += sys_time;
			////vreme_primanja = 0; //resetuj vreme primanja
			//korak2 = 1;
			//break;
		//case 1:
				//if (okay_flag){
					////SendChar('k', &USART_XM);
					////sendMsg("OKAY ", &USART_XM);
					//korak2 = 2;
					//okay_flag = 0;
					//TIMED_OUT_VREME  = 200;
				//}
				//else if(TIMED_OUT_VREME < sys_time){
				////	sendMsg("TIMED_OUT", &USART_XM);
					//korak2 = 0;
					//TIMED_OUT_VREME = 200;
				//}
			//break;
		//
		//case 2:
				//if(stigao_flag) //Uspesno poslao komandu robot ide po komandi
				//{
					////SendChar('s', &USART_XM);
					////sendMsg("STIGAO SAM", &USART_XM);
					////poslata_komanda = 1; //resetuje flag
					//stigao_flag = 0;
					//korak2 = 0;
					//TIMED_OUT_VREME = 200;
					//return 1;
				//}
			//break;
		//default: 
			//return 0;
		//break;
		//}//switch
	//return 0;
}

void idi_nazad(unsigned int x, unsigned int y, unsigned int ugao)
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
		overflow_primanje = 0;
		korak2++;
		break;
		
		case 1:
		if(okay_flag){
			korak2 = 2;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			sendMsg("OVERFLOW",&USART_XM);
			
		}
		break;
		
		case 2:
		if(stigao_flag){
			stigao_flag = 0;
			korak2++;
		}
		break;
		
		default:
		break;
	}
}

void taktika_1(void)
{
	switch (korak)
	{
		case 0:
			idi_pravo(500,0,0);
			if (korak2 == 3)
			{
				korak++;
				korak2 = 0;
			}
			
		
		case 1:
			SendChar('1', &USART_XM);
			idi_nazad(0,0,0);
			if(korak2==3)
			{
					korak++;
					korak2 = 0;
			}
		break;
		
		case 2:
			sendMsg("Zavrsio sam", &USART_XM);
		break;
		
		default:
		break;
	}
}