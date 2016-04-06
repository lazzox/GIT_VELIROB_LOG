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

//dodao Njegos
USART_data_t USART_D1_data;



///////////////////////////Globalne promenljive///////////////////////

extern volatile unsigned char
//komunikacija
sendArray[128],
receiveArray[128],
CHC,
sima,
RX_i_E0,
RX_i_E1,
RX_i_C0,
proveri_vreme_primanja,
velicina_niz_1,
velicina_niz_2,
velicina_niz_3,
sample_time_niz_1,
sample_time_niz_2,
sample_time_niz_3,
sample_counter_niz_1,
sample_counter_niz_2,
sample_counter_niz_3,
niz_counter_niz_1,
niz_counter_niz_2,
niz_counter_niz_3,


//KOMUNIKACIJA
adresa_long,
vrednost_prom,
adresa_niz_1,
adresa_niz_2,
adresa_niz_3;
							
#endif /* GLOBALS_H_ */