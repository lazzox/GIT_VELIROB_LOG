/*
 * taktike.h
 *
 * Created: 08-Apr-16 18:25:12
 *  Author: Sirius-PC
 */ 

#ifndef TAKTIKE_H_
#define TAKTIKE_H_

void inicijalizuj_servo_tajmer_20ms();
void pomeri_servo_1(uint16_t deg);

void idi_pravo(signed int x, signed int y, signed int ugao);
void idi_nazad(signed int x, signed int y, signed int ugao);
void senzor_stop(void);
void rotiraj(int ugao_rotacije);

void kraj_meca(void);
void brzina(unsigned int brzinaa);
void postavi_sistem(long x, long y, long ugao);

void ukljuci_senzore();
void iskljuci_senzore();

void ProfiServo(unsigned char iddP, int ugaoP, int brzinaP);
void ProfiServo_WheelMode(unsigned char iddP, int smer_rotacije);
void ProfiServo_WheelMode_STOP(unsigned char iddP);

void taktika_lazar(void);
void taktika_kocka(void);
void taktika_1(void);


volatile unsigned char
flag_senzor,
senzor_enable_prednji,
sensor_dir,
sensor_enable,
senzor_enable_zadnji;

volatile unsigned int
senzor_tajmer,
korak,
funny_action_timer,
korak2;


#endif /* TAKTIKE_H_ */