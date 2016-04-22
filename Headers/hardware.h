/*
 * hardware.h
 *
 * Created: 4/19/2011 6:30:23 PM
 *  Author: robert kovacs
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

void Podesi_Interapt(void);
void Podesi_Oscilator(void);
void Podesi_USART_Komunikaciju(void);
void Podesi_Tajmere(void);
void Podesi_Pinove(void);
void Resetuj_USART();
void inicijalizuj_servo_tajmer_20ms();
void pomeri_servo_1(uint16_t deg);

#endif /* HARDWARE_H_ */