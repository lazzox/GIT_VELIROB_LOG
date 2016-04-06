/*
 * mechanism.c
 *
 * Created: 4/19/2011 3:24:54 PM
 *  Author: robert kovacs
 */
#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/port_driver.h"
#include "Headers/globals.h"
#include "Headers/mechanism.h"

volatile unsigned char

//komunikacija
sendArray[128],
receiveArray[128],
CHC,
RX_i_E0,
RX_i_E1,
RX_i_C0,
proveri_vreme_primanja,
sima=0;





