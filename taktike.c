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

#define senzor_prednji (PORTJ.IN & 0b00000001)
#define senzor_zadnji (PORTJ.IN & 0b00000010)

void postavi_sistem(long x, long y, long ugao)
{
	switch(korak2){
		case 0:
		SendChar('G',&USART_XDRIVE);
		SendChar((x>>8),&USART_XDRIVE);
		SendChar(x,&USART_XDRIVE);
		SendChar((y>>8),&USART_XDRIVE);
		SendChar(y,&USART_XDRIVE);
		SendChar((ugao>>8),&USART_XDRIVE);
		SendChar(ugao,&USART_XDRIVE);
		SendChar('S',&USART_XDRIVE);
		korak2++;
		overflow_primanje = 0;
		break;
		case 1:
		if(okay_flag){
			
			//sendMsg("okey",&USART_XM);
			korak2 = 3;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			overflow_primanje = 0;
			korak2 = 0;
			sendMsg("OVERFLOW",&USART_XM);
			
		}
		
		break;
		
		default:
		break;
	}
}

void idi_pravo(signed int x, signed int y, signed int ugao)
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
				stigao_flag2 = 1;
				sendMsg("okey",&USART_XM);
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;
			}
			else if(overflow_primanje > 200)
			{
				overflow_primanje = 0;
				korak2 = 0;
				sendMsg("OVERFLOW",&USART_XM);
				
			}  
		
		break;
		
		case 2:
		if (stigao_flag_pomocni)
		{
			overflow_primanje=0;
			korak2=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
			
		}
		else if(stigao_flag){
				stigao_flag2 = 0;
				sendMsg("stigao",&USART_XM);
				stigao_flag = 0;
				korak2++;
			}
				
		break;
		
		
		
		default:
			break;
	}
	
}

void idi_nazad(signed int x, signed int y, signed int ugao)
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
			stigao_flag2 = 1;
			sendMsg("okey",&USART_XM);
			korak2 = 2;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			overflow_primanje = 0;
			korak2 = 0;
			sendMsg("OVERFLOW",&USART_XM);
			
		}
		
		break;
		
		case 2:
		if (stigao_flag_pomocni)
		{
			overflow_primanje=0;
			korak2=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
			
		}
		else if(stigao_flag){
			stigao_flag2 = 0;
			sendMsg("stigao",&USART_XM);
			stigao_flag = 0;
			korak2++;
		}
		
		break;
		
		
		
		default:
		break;
	}
	
}

void senzor_stop (void)
{
	
	if((senzor_prednji&&senzor_enable_prednji)||(senzor_zadnji&&senzor_enable_zadnji))
	{
		for (int i=0;i<10;i++)
		{
			if((senzor_prednji&&senzor_enable_prednji)||(senzor_zadnji&&senzor_enable_zadnji))
			flag_senzor++;
		}
		
	}
	if(flag_senzor>=5)
	{
		stigao_flag_pomocni=1;
		SendChar('S',&USART_XDRIVE);
		SendChar('A',&USART_XDRIVE);
		SendChar('B',&USART_XDRIVE);
		SendChar('C',&USART_XDRIVE);
		SendChar('D',&USART_XDRIVE);
		SendChar('E',&USART_XDRIVE);
		SendChar('F',&USART_XDRIVE);
		SendChar('P',&USART_XDRIVE);
		//stigao_flag_pomocni=1;
		//stigao_flag=0;
		//korak2=0;
		flag_senzor=0;
		//korak_detek=korak;
		
	}
			

}


void brzina (unsigned int brzinaa)
{
	switch(korak2){
		case 0:
		SendChar('D',&USART_XDRIVE);
		SendChar((brzinaa>>8),&USART_XDRIVE);
		SendChar(brzinaa,&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('X',&USART_XDRIVE);
		overflow_primanje = 0;
		korak2++;
		break;
		
		case 1:
		if(okay_flag){
			korak2 = 3;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			overflow_primanje = 0;
			korak2 = 0;
		}
		break;
		
		default:
		break;
	}
	
}



void taktika_kocka(void){
		
		switch (korak)
		{
			case 0:
				//brzina(250);
				idi_pravo(1100,0,0);
				if (korak2 == 3)
				{
					korak++;
					korak2 = 0;
				}
			break;
			
			case 1:
				idi_nazad(0,0,0);
				if (korak2 == 3)
				{
					korak++;
					korak2 = 0;
					
				}
			break;
			
			//case 2:
				//idi_pravo(0,500,0);
				//if (korak2 == 3)
				//{
					//sendMsg('korak++', &USART_XM);
					////if (!stigao_flag_pomocni)
					//korak=3;
					//korak2 = 0;
				//}
			//break;
			//
			//case 3:
				//idi_pravo(0,0,0);
				//if (korak2 == 3)
				//{
					//sendMsg('korak++', &USART_XM);
					////if (!stigao_flag_pomocni)
					//korak=0;
					//korak2 = 0;
				//}
			//
			//break;
			
			default:
			break;
		}
}

void taktika_1(void)
{
	
	switch (korak)
	{
		case 0:
			brzina(250);
			idi_pravo(550,0,0);			
			if (korak2 == 3)
			{
				sendMsg("Tacka 2", &USART_XM);
				korak ++;
				korak2 = 0;
			}
		break;
			
		case 1:
			idi_pravo(550,550,0);
			if (korak2==3)
			{
				korak++;
				korak2=0;
			}
		break;
			
 		case 2:
 			idi_pravo(0,550,0);
 			if (korak2==3)
 			{
 				korak++;
 				korak2=0;
 			}
 		break;
			
			
			//case 3:
			//rotiraj(30);
			//if(korak2==3)
			//{
				//sendMsg("Tacka 4", &USART_XM);
				//korak++;
				//korak2 = 0;
			//}
			//break;
			
			//case 4:
			//idi_pravo(0,0,0);
			//if(korak2==3)
			//{
				//sendMsg("Tacka 1 ", &USART_XM);
				//korak++;
				//korak2 = 0;
			//}
			//break;
		default:
		break;
	}
	
}