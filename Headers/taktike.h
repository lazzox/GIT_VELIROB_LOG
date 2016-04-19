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
void senzor_stop (void);
void rotiraj (unsigned int ugao);

void brzina(unsigned int brzinaa);

void taktika_kocka(void);
void taktika_1(void);


#endif /* TAKTIKE_H_ */