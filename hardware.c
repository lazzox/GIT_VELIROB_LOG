/*
 * communication.c
 *
 * Created: 4/19/2011 2:38:26 PM
 *  Author: Milan Romic *  Author: robert kovacs */

#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/TC_driver.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/port_driver.h"


void Podesi_Interapt(void)
{
	// Enable high level interrupts in the PMIC. 
	PMIC.CTRL |= PMIC_HILVLEN_bm;
	// Enable medium level interrupts in the PMIC. 
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	// Enable low level interrupts in the PMIC. 
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	//round-robin algoritam EN
	PMIC.CTRL |= PMIC_RREN_bm;
	
	sei(); // enable all inerrupts
}

void Podesi_Oscilator(void)
{
	//Startovanje internog oscilatora od 32MHz
	OSC.CTRL = OSC_RC32MEN_bm;

	do {
		//Sacekaj dok se stabilizuje oscilator
	} while ( ( OSC.STATUS & OSC_RC32MRDY_bm ) == 0 );
	
	//Ukljuci preskalere B i C
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSBCDIV_2_2_gc;

	//Postavi 32MHz kao glavni oscilator
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;	
}

void Podesi_USART_Komunikaciju(void)
{
	//USART_D1 - LCD - 62500
	//PORTD.DIR &= PIN7_bm;		//PD7 (TXD1) - izlaz
	//PORTD.DIR  |= ~PIN6_bm;		//PD6 (RXD1) - ulaz
	//USART_InterruptDriver_Initialize(&USART_D1_data, &USARTD1, USART_DREINTLVL_LO_gc); //Koriscenje USARTD1 i inicijalizacija buffer-a
	//USART_Format_Set(USART_D1_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);//USARTE0, 8 Data bits, No Parity, 1 Stop bit.
	//USART_RxdInterruptLevel_Set(USART_D1_data.usart, USART_RXCINTLVL_LO_gc); //Aktiviranje RXC interrupt-a
	//USART_Baudrate_Set(&USARTD1, 1, 1 );  //62500
	//USART_Rx_Enable(USART_D1_data.usart); //Ukljucivanje RX i TX
	//USART_Tx_Enable(USART_D1_data.usart);
	
	//USART_E1 - SALJE NA XDRIVE - 56000
	PORTE.DIR |= PIN7_bm;//PE7 (TXE1) - izlaz  
	PORTE.DIR  &= ~PIN6_bm;	//PE6 (RXE1) - ulaz
	USART_InterruptDriver_Initialize(&USART_E1_data, &USARTE1, USART_DREINTLVL_LO_gc); 	//Koriscenje USARTE1 (definisano u globals.h) i inicijalizacija buffer-a
	USART_Format_Set(USART_E1_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);//USARTE1, 8 Data bits, No Parity, 1 Stop bit.
	USART_RxdInterruptLevel_Set(USART_E1_data.usart, USART_RXCINTLVL_LO_gc);//Aktiviranje RXC interrupt-a
	USART_Baudrate_Set(&USARTE1,127,-4); //Podesavanje Baud rate	//56000
	USART_Rx_Enable(USART_E1_data.usart);	//Ukljucivanje RX i TX
	USART_Tx_Enable(USART_E1_data.usart);
	
	////USART_E0 BT_RS232 - MCU - 19200
	////PE3 (TXE0) - izlaz
	//PORTE.DIR |= PIN3_bm;
	////PE2 (RXE0) - ulaz
	//PORTE.DIR  &= ~PIN2_bm;
	////Koriscenje USARTE0 i inicijalizacija buffer-a
	//USART_InterruptDriver_Initialize(&USART_E0_data, &USARTE0, USART_DREINTLVL_LO_gc);
	////USARTE0, 8 Data bits, No Parity, 1 Stop bit.
	//USART_Format_Set(USART_E0_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);
	////Aktiviranje RXC interrupt-a
	//USART_RxdInterruptLevel_Set(USART_E0_data.usart, USART_RXCINTLVL_LO_gc);
	////19200 @ 32Mhz as calculated from ProtoTalk Calc
	////USART_Baudrate_Set(&USARTE0, 107, -5 ); //115200
	////                            bsel, bscale
	//USART_Baudrate_Set(&USARTE0, 1, 1 ); //62500
	////Ukljucivanje RX i TX
	//USART_Rx_Enable(USART_E0_data.usart);
	//USART_Tx_Enable(USART_E0_data.usart);
	

	//USART_C0 - Xmega_USB - 115200
	PORTC.DIR &= PIN3_bm;	//PE3 (TXE0) - izlaz
	PORTC.DIR  |= ~PIN2_bm;//PE2 (RXE0) - ulaz
	USART_InterruptDriver_Initialize(&USART_C0_data, &USARTC0, USART_DREINTLVL_LO_gc); //Koriscenje USARTE0 i inicijalizacija buffer-a
	USART_Format_Set(USART_C0_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);//USARTE0, 8 Data bits, No Parity, 1 Stop bit.
	USART_RxdInterruptLevel_Set(USART_C0_data.usart, USART_RXCINTLVL_LO_gc);//Aktiviranje RXC interrupt-a
	USART_Baudrate_Set(&USARTC0, 107, -5 );//Podesavanje Baud rate	//115200
	USART_Rx_Enable(USART_C0_data.usart);//Ukljucivanje RX i TX
	USART_Tx_Enable(USART_C0_data.usart);

	//USART_D1 - LCD
	PORTD.DIR &= PIN7_bm;//PE3 (TXE0) - izlaz
	PORTD.DIR  |= ~PIN6_bm;//PE2 (RXE0) - ulaz
	USART_InterruptDriver_Initialize(&USART_D1_data, &USARTD1, USART_DREINTLVL_LO_gc);//Koriscenje USARTE0 i inicijalizacija buffer-a
	USART_Format_Set(USART_D1_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false); //USARTE0, 8 Data bits, No Parity, 1 Stop bit.
	USART_RxdInterruptLevel_Set(USART_D1_data.usart, USART_RXCINTLVL_LO_gc); //Aktiviranje RXC interrupt-a
	USART_Baudrate_Set(&USARTD1, 1, 1 );//Podesavanje Baud rate	//666500
	USART_Rx_Enable(USART_D1_data.usart);//Ukljucivanje RX i TX
	USART_Tx_Enable(USART_D1_data.usart);

	Resetuj_USART();
}

void Resetuj_USART(){
	
	RX_i_C0 = 0;
	RX_i_E0 = 0;
	RX_i_E1 = 0;	
}

void Podesi_Tajmere(void)
{
	//SPISAK TAJMERA:
	//TCC0 - SLOBODAN (FET izlazi)
	//TCC1 - SLOBODAN (FET izlazi)
	//TCD0 - QDEC enkoder (motor_levi)
	//TCD1 - QDEC enkoder (motor_desni)
	//TCE0 - QDEC enkoder (pasivni_levi)
	//TCE1 - system tajmer (3ms)
	//TCF0 - servo signal generator
	//TCF1 - motor PWM (levi, desni)
	
	/* System timer ( TOP value ). */
	TC_SetPeriod( &TCE1, 0x002F ); //0x00BF = 12ms //0x5F = 6ms //0x2F = 3ms <- Mirko // Stvarno je polovina vrednosti!
	/* Enable overflow interrupt at low level */
	TC1_SetOverflowIntLevel( &TCE1, TC_OVFINTLVL_MED_gc );
	/* Start Timer/Counter. */
	TC1_ConfigClockSource( &TCE1, TC_CLKSEL_DIV256_gc );
}

void Podesi_Pinove(void)
{
	//PORTA - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTA,0xFF);
	PORT_ConfigurePins(&PORTA,
						0xFF,
						0,
						0,
						PORT_OPC_TOTEM_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTA, 0xFF);


	
	//PORTB - digitalni izlazi 
	PORT_SetPinsAsOutput(&PORTB,0xFF);
	PORT_ConfigurePins(&PORTB,
						0xFF,
						0,
						0,
						PORT_OPC_TOTEM_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTB, 0xFF);
	
	//PORTC - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTC,0xFF);
	PORT_ConfigurePins(&PORTC,
						0xFF,
						0,
						0,
						PORT_OPC_PULLDOWN_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTC, 0xFF);

	//PORTD - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTD,0xFF);
	PORT_ConfigurePins(&PORTD,
						0xFF,
						0,
						0,
						PORT_OPC_TOTEM_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTD, 0xFF);

	//PORTE - digitalni izlazi
// 	PORT_SetPinsAsOutput(&PORTE,0xFF);
// 	PORT_ConfigurePins(&PORTE,
// 	0xFF,
// 	0,
// 	0,
// 	PORT_OPC_TOTEM_gc,
// 	PORT_ISC_INPUT_DISABLE_gc);
// 	PORT_ClearPins(&PORTE, 0xFF);

	
	//PORTF - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTF,0xFF);
	PORT_ConfigurePins(&PORTF,
						0xFF,
						0,
						0,
						PORT_OPC_TOTEM_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTF, 0xFF);
	
	//PORTK - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTK,0b11111111);
	//PORT_SetPinsAsOutput(&PORTK,0b00100000);
	//PORTK.PIN6CTRL=0b00111001;
 	PORT_ConfigurePins(&PORTK,
 						0xFF,
 						0,
 						0,
 						PORT_OPC_PULLUP_gc,
 						PORT_ISC_INPUT_DISABLE_gc);
 	//PORT_ClearPins(&PORTK, 0xFF);
	
	//PORTJ - digitalni izlazi
	PORT_SetPinsAsInput(&PORTJ,0xFF);
	PORT_ConfigurePins(&PORTJ,
						0xFF,
						0,
						0,
						PORT_OPC_PULLDOWN_gc,
						PORT_ISC_RISING_gc);
	
	//PORTH - digitalni izlazi
	PORT_SetPinsAsOutput(&PORTH,0xFF);
	PORT_ConfigurePins(&PORTH,
						0xFF,
						0,
						0,
						PORT_OPC_TOTEM_gc,
						PORT_ISC_INPUT_DISABLE_gc);
	PORT_ClearPins(&PORTH, 0xFF);
	//
	//PORT_SetPinsAsInput( &PORTB, 0xFF );
	//PORT_ConfigurePins( &PORTB,
						//0xFF,
						//false,
						//false,
						//PORT_OPC_PULLUP_gc,
						//PORT_ISC_FALLING_gc);
	
	//podesavanje interrupt0 za PORTB.0 - ISR(PORTB_INT0_vect)
	//PORT_ConfigureInterrupt0( &PORTB, PORT_INT0LVL_LO_gc, 0x01 );
	
	//PORTC - FET izlazi 
	//PORT_SetPinsAsOutput(&PORTC, 0xFF);
//  	PORT_MapVirtualPort0( PORTCFG_VP0MAP_PORTC_gc );	//mapiranje virtualnog porta 0 na PORTC
//  	PORT_SetDirection( &VPORT0, 0xFF );
//		VPORT0.OUT = 0x00;	//clear
	
	//pinovi za upravljanje moorima
	//PORT_SetPinsAsOutput(&PORTH, 0xFF); //PH0-IN_A1, PH1-EN_A1, PH2-EN_B1, PH3-IN_B1, PH4-IN_A2, PH5-EN_A2, PH6-EN_B2, PH7-IN_B2
	//PORT_ClearPins(&PORTH, 0xFF);
	//PORT_SetPins(&PORTH, 0b01100110); // EN ALL	

	//servo izlazi
	//PORT_SetPinsAsOutput(&PORTF, 0x0F);
	//PORT_ClearPins(&PORTF, 0x0F);
	
}

