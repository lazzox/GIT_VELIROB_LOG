/*
 * globals.h
 *
 * Created: 4/19/2011 2:16:36 PM
 *  Author: robert kovacs
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "avr_compiler.h"
#include "usart_driver.h"

#define mmio32(x)   (*(volatile unsigned long *)(x))

// USART data struct  
USART_data_t USART_E0_data;
USART_data_t USART_E1_data;
USART_data_t USART_C0_data; 
USART_data_t USART_D1_data;

///////////////////////////Globalne promenljive///////////////////////
extern volatile unsigned char
//komunikacija
sendArray[128],
receiveArray[128],
okay_flag,
RX_i_E0,
RX_i_E1,
RX_i_C0,
stigao_flag,
stigao_flag2,
stigao_flag_pomocni,
flag_senzor,

//KOMUNIKACIJA
adresa_long,
vrednost_prom,
adresa_niz_1,
adresa_niz_2,
adresa_niz_3;

extern volatile unsigned int
vreme_primanja,
overflow_primanje,
sys_time,
tajmer,
senzor_tajmer,
TIMED_OUT_VREME,
korak,
korak_detek,
korak2,
senzor_enable_prednji,
senzor_enable_zadnji;
							
#endif /* GLOBALS_H_ */