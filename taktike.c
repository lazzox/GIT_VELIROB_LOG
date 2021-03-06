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

//#define senzor_prednji (PORTJ.IN & 0b00000101) //maske za oba prednja senzora
//#define senzor_zadnji  (PORTJ.IN & 0b00010000) // maska za zadnji senzor
//zadnji desni J7, zadnji levi J3, prednji desni J0, prednji J2

#define START 0xFF
#define INSTR_WRITE 0x03
#define LENGTH 0x07
#define ADDRESS 0x1E

unsigned char profiServoKomande[15];

#define senzor_maska_prednji 0b00000101
#define senzor_maska_zadnji  0b10001000
#define sensor_det (PORTJ.IN & sensor_dir)


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


void kraj_meca (void)
{
	switch(kur){
		
		case 0:
		SendChar('K',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('c',&USART_XDRIVE);
		SendChar('M',&USART_XDRIVE);
		kur++;
		overflow_funny=0;
		break;
		
		case 1:
		if(okay_flag){
			okay_flag = 0;
			overflow_funny = 0;
		}
		else if(overflow_funny > 200)
		{
			overflow_funny = 0;
			kur = 0;
		}
		
		break;
		
		
		
	}
}

void idi_pravo(signed int x, signed int y, signed int ugao)
{
	switch(korak2){
		case 0:
		if (!stigao_flag_pomocni)
		{
			sensor_dir = senzor_maska_prednji;
			//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
			//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
			//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
			// i ako su negativni)
			x += 4000;
			y += 4000;
			ugao += 720;
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
		}
		else if (stigao_flag_pomocni)
		{
			stigao_flag_pomocni=0;
		}
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
			korak2=4;
		}
		
		break;
		
		
		case 4:
		if(stigao_flag_pomocni)
		{
			korak2=0;
			overflow_primanje=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
		}
		else if (!stigao_flag_pomocni)
		{
			korak2=3;
			stigao_flag2=0;
			stigao_flag-0;
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
		if (!stigao_flag_pomocni)
		{
			sensor_dir = senzor_maska_zadnji;
			//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
			//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
			//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
			// i ako su negativni)
			x += 4000;
			y += 4000;
			ugao += 720;
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
		}
		else if (stigao_flag_pomocni)
		{
			stigao_flag_pomocni=0;
		}
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
			korak2=4;
		}
		
		break;
		
		case 4:
		if(stigao_flag_pomocni)
		{
			korak2=0;
			overflow_primanje=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
		}
		else if (!stigao_flag_pomocni)
		{
			korak2=3;
			stigao_flag2=0;
			stigao_flag-0;
		}
		
		
		break;
		
		
		
		default:
		break;
	}
	
}

void rotiraj(int ugao_rotacije)
{
	switch(korak2){
		case 0:
		//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
		//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
		//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
		// i ako su negativni)
		ugao_rotacije += 720;
		SendChar('U',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar((ugao_rotacije>>8),&USART_XDRIVE);
		SendChar(ugao_rotacije,&USART_XDRIVE);
		SendChar('X',&USART_XDRIVE);
		overflow_primanje = 0;
		korak2++;
		break;
		
		case 1:
		if(okay_flag){
			stigao_flag2 = 1;
			sendMsg("okey",&USART_XM);
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
		
		//case 2:
		//if (stigao_flag_pomocni)
		//{
		//overflow_primanje=0;
		//korak2=0;
		//stigao_flag_pomocni=0;
		//stigao_flag=0;
		//stigao_flag2=0;
		//
		//}
		//else if(stigao_flag){
		//stigao_flag2 = 0;
		//sendMsg("stigao",&USART_XM);
		//stigao_flag = 0;
		//korak2++;
		//}
		
		break;
		
		
		
		default:
		break;
	}
	
	
}


void ukljuci_senzore(){
	sensor_enable = 0xFF;
}

void iskljuci_senzore(){
	sensor_enable = 0x00;
}

void senzor_stop(void)
{
	//sendMsg("CLEAN", &USART_LCD);
	if( sensor_det & sensor_enable)
	{//Ako udje neki od senzora je detektovao
	
		for (int i=0;i<11;i++)
		{
			if(sensor_det)
			flag_senzor++;
		}
		
	}
	else{
		sendMsg("CLEAN", &USART_LCD);
	}
	if(flag_senzor>9)
	{
		sendMsg("DETEKCIJA", &USART_LCD);
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
			if (sys_time>8500)
			{
				//brzina(250);
				//ukljuci_senzore();
				ukljuci_senzore();
				idi_pravo(400,0,90);
				if (korak2 == 3)
				{
					korak=22;
					korak2 = 0;
					sys_time=0;
				}
			}
			break;

			case 22:
			//brzina(250);
			//ukljuci_senzore()
			//iskljuci_senzore();
			idi_nazad(400,-536,90);
			if (korak2 == 3)
			{
				korak=4;
				korak2 = 0;
				sys_time=0;
			}
		
		break;
				
			case 4:
			if (sys_time>1500)
			{
				//brzina(250);
				ukljuci_senzore();
				idi_pravo(400,300,90);
				if (korak2 == 3)
				{
					korak=9999;
					korak2 = 0;
					sys_time=0;
				}
			}
			break;
				
			
			
			
// 			case 1:
// 			//if (sys_time>1500)
// 			//{
// 				rotiraj(90);
// 				if (korak2 == 3)
// 				{
// 					korak++;
// 					korak2 = 0;
// 					sys_time-0;
// 					
// 				}
// 			//}
// 			break;
			
// 			case 2:
// 			//if (sys_time>1500)
// 			//{
// 				rotiraj(180);
// 				if (korak2 == 3)
// 				{
// 					korak++;
// 					korak2 = 0;
// 					sys_time-0;
// 					
// 				}
// 			//}
// 			break;
// 			
// 			case 3:
// 			if (sys_time>1500)
// 			{
// 				rotiraj(270);
// 				if (korak2 == 3)
// 				{
// 					korak++;
// 					korak2 = 0;
// 					sys_time-0;
// 					
// 				}
// 			}
// 			break;
// 			
// 				case 4:
// 				if (sys_time>1500)
// 				{
// 					rotiraj(0);
// 					if (korak2 == 3)
// 					{
// 						korak=1;
// 						korak2 = 0;
// 						sys_time-0;
// 						
// 					}
// 				}
// 				break;
// 			
			
			
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

void taktika_lazar(void){
	
	switch (korak)
	{
		case 0:
		//brzina(250);
		sendMsg("IDI PRAVO", &USART_LCD);
		idi_pravo(550,0,0);
		if (korak2 == 3)
		{
			sendMsg("Tacka 2", &USART_LCD);
			korak ++;
			korak2 = 0;
		}
		break;
		
		case 1:
			idi_pravo(0,0,0);
		if (korak2==3)
		{
			korak++;
			korak2=0;
		}
		break;
		
		case 2:
			inicijalizuj_servo_tajmer_20ms();
			pomeri_servo_1(300);
			korak++;
			korak2=0;
		break;
	default:
		break;
	}
	
}

void ProfiServo(unsigned char iddP, int ugaoP, int brzinaP)
{
	//F7 - Tx 
	//F6 - Rx
	//maksimalna brzina i maksilamli ugo je 1023, zato mnozim s ovim da rasirim nasptam pravog ugla
	//USART_Rx_Disable(USART_F1_data.usart);
	//
	//PORT_ConfigurePins( &PORTF,
	//PIN7_bm,
	//false,
	//false,
	//PORT_OPC_TOTEM_gc,
	//PORT_ISC_RISING_gc);
	//
	//USART_Tx_Enable(USART_F1_data.usart);
	if(ugaoP<0) ugaoP=0;
	if(ugaoP>300) ugaoP=300;
	if(brzinaP<0) brzinaP=0;
	if(brzinaP>600) brzinaP=600;

	ugaoP=(ugaoP)*3 +(ugaoP*4)/10;
	
	unsigned char idP=iddP,sum,checksum;
	unsigned char brzina_servo_HP=(unsigned char)((brzinaP&0xff00)>>8);
	unsigned char brzina_servo_LP=(unsigned char)((brzinaP&0x00ff));
	unsigned char ugao_servo_HP=(unsigned char)((ugaoP&0xff00)>>8);
	unsigned char ugao_servo_LP=(unsigned char)((ugaoP&0x00ff));
	
	sum=0x28 + idP + brzina_servo_HP + brzina_servo_LP + ugao_servo_HP + ugao_servo_LP;// + 0xFF + 0x03;          ///suma iinfo=2xStart+lenght+iw+add
	checksum=~sum;
	
	profiServoKomande[0] = 0xFF;
	profiServoKomande[1] = 0xFF;
	profiServoKomande[2] = 0xFF;
	profiServoKomande[3] = 0xFF;
	profiServoKomande[4] = 0xFF;//START;
	//USART_TXBuffer_PutByte(&USART_F0_data,START);
	profiServoKomande[5] = 0xFF;//START;
	//USART_TXBuffer_PutByte(&USART_F0_data,START);
	profiServoKomande[6] = idP;
	//USART_TXBuffer_PutByte(&USART_F0_data,idP);
	profiServoKomande[7] = 0x07;//LENGTH;
	//USART_TXBuffer_PutByte(&USART_F0_data,LENGTH);
	profiServoKomande[8] = 0x03;//INSTR_WRITE;
	//USART_TXBuffer_PutByte(&USART_F0_data,INSTR_WRITE);
	profiServoKomande[9] = 0x1E;//ADDRESS;
	//USART_TXBuffer_PutByte(&USART_F0_data,ADDRESS);
	
	//USART_TXBuffer_PutByte(&USART_E0_data,0xFF);		// torque.. maximalan je 0x03FF tj 1023 prvo saljem nize bite
	//putchar(brzina_servo_LP);
	//USART_TXBuffer_PutByte(&USART_E0_data,0x03);
	//putchar(brzina_servo_HP);
	
	
	profiServoKomande[10] = ugao_servo_LP;
	//USART_TXBuffer_PutByte(&USART_F0_data,ugao_servo_LP);
	profiServoKomande[11] = ugao_servo_HP;
	//USART_TXBuffer_PutByte(&USART_F0_data,ugao_servo_HP);
	profiServoKomande[12] = brzina_servo_LP;
	//USART_TXBuffer_PutByte(&USART_F0_data,brzina_servo_LP);
	profiServoKomande[13] = brzina_servo_HP;
	//USART_TXBuffer_PutByte(&USART_F0_data,brzina_servo_HP);
	
	profiServoKomande[14] = checksum;
	//USART_TXBuffer_PutByte(&USART_F0_data,checksum);
	int i = 0;
	
	while (i <= 14)
	{
		bool byteToBuffer;
		byteToBuffer = USART_TXBuffer_PutByte(&USART_E0_data, profiServoKomande[i]);
		if(byteToBuffer)
			i++;
	}
/*	USART_TXBuffer_PutByte(&USART_E0_data,START);
	USART_TXBuffer_PutByte(&USART_E0_data,START);
	USART_TXBuffer_PutByte(&USART_E0_data,idP);
	USART_TXBuffer_PutByte(&USART_E0_data,LENGTH);
	USART_TXBuffer_PutByte(&USART_E0_data,INSTR_WRITE);
	USART_TXBuffer_PutByte(&USART_E0_data,ADDRESS);
	
	//USART_TXBuffer_PutByte(&USART_E0_data,0xFF);		// torque.. maximalan je 0x03FF tj 1023 prvo saljem nize bite
	//putchar(brzina_servo_LP);
	//USART_TXBuffer_PutByte(&USART_E0_data,0x03);
	//putchar(brzina_servo_HP);
	
	USART_TXBuffer_PutByte(&USART_E0_data,ugao_servo_LP);
	USART_TXBuffer_PutByte(&USART_E0_data,ugao_servo_HP);
	USART_TXBuffer_PutByte(&USART_E0_data,brzina_servo_LP);
	USART_TXBuffer_PutByte(&USART_E0_data,brzina_servo_HP);
	
	USART_TXBuffer_PutByte(&USART_E0_data,checksum);*/
}

void ProfiServo_WheelMode(unsigned char iddP, int smer_rotacije)
{
	//F7 - Tx 
	//F6 - Rx
		
	unsigned char idP=iddP,sum,checksum,rotiraj;
	if(smer_rotacije)
		rotiraj = 0x07;
	if(smer_rotacije)
		rotiraj = 0x03;
		
	sum=0x127 + idP + rotiraj;
	checksum=~sum;
	
	profiServoKomande[0] = 0xFF;
	profiServoKomande[1] = 0xFF;
	profiServoKomande[2] = 0xFF;
	profiServoKomande[3] = 0xFF;
	profiServoKomande[4] = 0xFF;//START;
	profiServoKomande[5] = 0xFF;//START;
	profiServoKomande[6] = idP;
	profiServoKomande[7] = 0x05;//LENGTH;
	profiServoKomande[8] = 0x03;//INSTR_WRITE;
	profiServoKomande[9] = 0x20;//ADDRESS;	
	profiServoKomande[10] = 0xFF;
	profiServoKomande[11] = rotiraj;
	profiServoKomande[12] = checksum;
	int i = 0;
	
	while (i <= 12)
	{
		bool byteToBuffer;
		byteToBuffer = USART_TXBuffer_PutByte(&USART_E0_data, profiServoKomande[i]);
		if(byteToBuffer)
			i++;
	}
}

void ProfiServo_WheelMode_STOP(unsigned char iddP)
{
	//F7 - Tx 
	//F6 - Rx
		
	unsigned char idP=iddP,sum,checksum;
			
	sum=0x28 + idP;
	checksum=~sum;
	
	profiServoKomande[0] = 0xFF;
	profiServoKomande[1] = 0xFF;
	profiServoKomande[2] = 0xFF;
	profiServoKomande[3] = 0xFF;
	profiServoKomande[4] = 0xFF;//START;
	profiServoKomande[5] = 0xFF;//START;
	profiServoKomande[6] = idP;
	profiServoKomande[7] = 0x05;//LENGTH;
	profiServoKomande[8] = 0x03;//INSTR_WRITE;
	profiServoKomande[9] = 0x20;//ADDRESS;
	profiServoKomande[10] = 0x00;
	profiServoKomande[11] = 0x00;
	profiServoKomande[12] = checksum;
	int i = 0;
	
	while (i <= 12)
	{
		bool byteToBuffer;
		byteToBuffer = USART_TXBuffer_PutByte(&USART_E0_data, profiServoKomande[i]);
		if(byteToBuffer)
			i++;
	}
}


