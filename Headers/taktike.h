/*
 * taktike.h
 *
 * Created: 08-Apr-16 18:25:12
 *  Author: Sirius-PC
 */ 

#ifndef TAKTIKE_H_
#define TAKTIKE_H_



void idi_pravo(signed int x, signed int y, signed int ugao);
void idi_nazad(signed int x, signed int y, signed int ugao);
void senzor_stop(void);
void rotiraj(unsigned int ugao);
void postavi_sistem(long x, long y, long ugao);
void brzina(unsigned int brzinaa);
void ukljuci_senzore();
void iskljuci_senzore();
void ProfiServo(unsigned char iddP, int ugaoP, int brzinaP);


void taktika_kocka(void);
void taktika_1(void);


volatile unsigned char 
senzor_enable_prednji,
sensor_dir,
sensor_enable,
senzor_enable_zadnji;


#endif /* TAKTIKE_H_ */