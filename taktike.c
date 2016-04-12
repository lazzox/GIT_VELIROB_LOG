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
		sendMsg("saljem", &USART_XM);
			SendChar('A',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar((ugao>>8),&USART_XDRIVE);
			SendChar(ugao,&USART_XDRIVE);
			SendChar('X',&USART_XDRIVE);
				SendChar('A',&USART_XM);
				SendChar((x>>8),&USART_XM);
				SendChar(x,&USART_XM);
				SendChar((y>>8),&USART_XM);
				SendChar(y,&USART_XM);
				SendChar((ugao>>8),&USART_XM);
				SendChar(ugao,&USART_XM);
				SendChar('X',&USART_XM);
			overflow_primanje = 0;
			korak2++;
		break;
		
		case 1:
			if(okay_flag){
				sendMsg("okey",&USART_XM);
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;
			}
			else if(overflow_primanje > 200)
			{
				korak2 = 0;
				sendMsg("OVERFLOW",&USART_XM);
			}
		
		break;
		
		case 2:
			if(stigao_flag){
				sendMsg("stigao",&USART_XM);
				stigao_flag = 0;
				korak2++;
			}	
		break;
		
		default:
			break;
	}
}

void idi_nazad(unsigned int x, unsigned int y, unsigned int ugao)
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
				korak2 = 0;
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
			idi_nazad(0,0,0);
			if(korak2==3)
			{
					korak++;
					korak2 = 0;
			}
		break;
		
		case 2:
			sendMsg("Zavrsio sam", &USART_XM);
			//korak++;
		break;
		
		default:
		break;
	}
}