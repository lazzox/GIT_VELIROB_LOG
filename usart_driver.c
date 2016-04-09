#include "Headers/usart_driver.h"



/*  Initializes buffer and selects what USART module to use.
 *
 *  Initializes receive and transmit buffer and selects what USART module to use,
 *  and stores the data register empty interrupt level.
 *
 *  \param usart_data           The USART_data_t struct instance.
 *  \param usart                The USART module.
 *  \param dreIntLevel          Data register empty interrupt level.
 */
void USART_InterruptDriver_Initialize(USART_data_t * usart_data,
                                      USART_t * usart,
                                      USART_DREINTLVL_t dreIntLevel)
{
	usart_data->usart = usart;
	usart_data->dreIntLevel = dreIntLevel;

	usart_data->buffer.RX_Tail = 0;
	usart_data->buffer.RX_Head = 0;
	usart_data->buffer.TX_Tail = 0;
	usart_data->buffer.TX_Head = 0;
}


/*  Set USART DRE interrupt level.
 *
 *  Set the interrupt level on Data Register interrupt.
 *
 *  \note Changing the DRE interrupt level in the interrupt driver while it is
 *        running will not change the DRE interrupt level in the USART before the
 *        DRE interrupt have been disabled and enabled again.
 *
 *  \param usart_data         The USART_data_t struct instance
 *  \param dreIntLevel        Interrupt level of the DRE interrupt.
 */
void USART_InterruptDriver_DreInterruptLevel_Set(USART_data_t * usart_data,
                                                 USART_DREINTLVL_t dreIntLevel)
{
	usart_data->dreIntLevel = dreIntLevel;
}


/*  Test if there is data in the transmitter software buffer.
 *
 *  This function can be used to test if there is free space in the transmitter
 *  software buffer.
 *
 *  \param usart_data The USART_data_t struct instance.
 *
 *  \retval true      There is data in the transmit buffer.
 *  \retval false     The transmit buffer is empty.
 */
bool USART_TXBuffer_FreeSpace(USART_data_t * usart_data)
{
	// Make copies to make sure that volatile access is specified. 
	uint8_t tempHead = (usart_data->buffer.TX_Head + 1) & USART_TX_BUFFER_MASK;
	uint8_t tempTail = usart_data->buffer.TX_Tail;

	// There are data left in the buffer unless Head and Tail are equal.
	return (tempHead != tempTail);
}



/*  Put data (5-8 bit character).
 *
 *  Stores data byte in TX software buffer and enables DRE interrupt if there
 *  is free space in the TX software buffer.
 *
 *  \param usart_data The USART_data_t struct instance.
 *  \param data       The data to send.
 */
bool USART_TXBuffer_PutByte(USART_data_t * usart_data, uint8_t data)
{
	uint8_t tempCTRLA;
	uint8_t tempTX_Head;
	bool TXBuffer_FreeSpace;
	USART_Buffer_t * TXbufPtr;

	TXbufPtr = &usart_data->buffer;
	TXBuffer_FreeSpace = USART_TXBuffer_FreeSpace(usart_data);


	if(TXBuffer_FreeSpace)
	{
	  	tempTX_Head = TXbufPtr->TX_Head;
	  	TXbufPtr->TX[tempTX_Head]= data;
		// Advance buffer head. 
		TXbufPtr->TX_Head = (tempTX_Head + 1) & USART_TX_BUFFER_MASK;

		// Enable DRE interrupt. 
		tempCTRLA = usart_data->usart->CTRLA;
		tempCTRLA = (tempCTRLA & ~USART_DREINTLVL_gm) | usart_data->dreIntLevel;
		usart_data->usart->CTRLA = tempCTRLA;
	}
	return TXBuffer_FreeSpace;
}



/*  Test if there is data in the receive software buffer.
 *
 *  This function can be used to test if there is data in the receive software
 *  buffer.
 *
 *  \param usart_data         The USART_data_t struct instance
 *
 *  \retval true      There is data in the receive buffer.
 *  \retval false     The receive buffer is empty.
 */
bool USART_RXBufferData_Available(USART_data_t * usart_data)
{
	// Make copies to make sure that volatile access is specified.  
	uint8_t tempHead = usart_data->buffer.RX_Head;
	uint8_t tempTail = usart_data->buffer.RX_Tail;

	// There are data left in the buffer unless Head and Tail are equal.  
	return (tempHead != tempTail);
}



/*  Get received data (5-8 bit character).
 *
 *  The function USART_RXBufferData_Available should be used before this
 *  function is used to ensure that data is available.
 *
 *  Returns data from RX software buffer.
 *
 *  \param usart_data       The USART_data_t struct instance.
 *
 *  \return         Received data.
 */
uint8_t USART_RXBuffer_GetByte(USART_data_t * usart_data)
{
	USART_Buffer_t * bufPtr;
	uint8_t ans;

	bufPtr = &usart_data->buffer;
	ans = (bufPtr->RX[bufPtr->RX_Tail]);

	// Advance buffer tail.  
	bufPtr->RX_Tail = (bufPtr->RX_Tail + 1) & USART_RX_BUFFER_MASK;

	return ans;
}



/*  RX Complete Interrupt Service Routine.
 *
 *  RX Complete Interrupt Service Routine.
 *  Stores received data in RX software buffer.
 *
 *  \param usart_data      The USART_data_t struct instance.
 */
bool USART_RXComplete(USART_data_t * usart_data)
{
	USART_Buffer_t * bufPtr;
	bool ans;

	bufPtr = &usart_data->buffer;
	// Advance buffer head.  
	uint8_t tempRX_Head = (bufPtr->RX_Head + 1) & USART_RX_BUFFER_MASK;

	// Check for overflow.  
	uint8_t tempRX_Tail = bufPtr->RX_Tail;
	uint8_t data = usart_data->usart->DATA;

	if (tempRX_Head == tempRX_Tail) {
	  	ans = false;		//stigli su svi podaci
	}else{
		ans = true;
		usart_data->buffer.RX[usart_data->buffer.RX_Head] = data;
		usart_data->buffer.RX_Head = tempRX_Head;
	}
	return ans;
}



/*  Data Register Empty Interrupt Service Routine.
 *
 *  Data Register Empty Interrupt Service Routine.
 *  Transmits one byte from TX software buffer. Disables DRE interrupt if buffer
 *  is empty. Argument is pointer to USART (USART_data_t).
 *
 *  \param usart_data      The USART_data_t struct instance.
 */
void USART_DataRegEmpty(USART_data_t * usart_data)
{
	USART_Buffer_t * bufPtr;
	bufPtr = &usart_data->buffer;

	// Check if all data is transmitted. 
	uint8_t tempTX_Tail = usart_data->buffer.TX_Tail;
	if (bufPtr->TX_Head == tempTX_Tail){
	    // Disable DRE interrupts.
		uint8_t tempCTRLA = usart_data->usart->CTRLA;
		tempCTRLA = (tempCTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc;
		usart_data->usart->CTRLA = tempCTRLA;

	}else{
		// Start transmitting. 
		uint8_t data = bufPtr->TX[usart_data->buffer.TX_Tail];
		usart_data->usart->DATA = data;

		// Advance buffer tail. 
		bufPtr->TX_Tail = (bufPtr->TX_Tail + 1) & USART_TX_BUFFER_MASK;
	}
}


/*  Put data (9 bit character).
 *
 *  Use the function USART_IsTXDataRegisterEmpty before using this function to
 *  put 9 bit character to the TX register.
 *
 *  \param usart      The USART module.
 *  \param data       The data to send.
 */
void USART_NineBits_PutChar(USART_t * usart, uint16_t data)
{
	if(data & 0x0100) {
		usart->CTRLB |= USART_TXB8_bm;
	}else {
		usart->CTRLB &= ~USART_TXB8_bm;
	}

	usart->DATA = (data & 0x00FF);
}


/*  Get received data (9 bit character).
 *
 *  This function reads out the received 9 bit character (uint16_t).
 *  Use the function USART_IsRXComplete to check if anything is received.
 *
 *  \param usart      The USART module.
 *
 *  \retval           Received data.
 */
uint16_t USART_NineBits_GetChar(USART_t * usart)
{
	if(usart->CTRLB & USART_RXB8_bm) {
		return(0x0100 | usart->DATA);
	}else {
		return(usart->DATA);
	}
}

/*  Send Char over USART example:
*	SendChar('k',&USART_LCD);
*	SendChar('k',&USART_XDRIVE)
*/
void SendChar(char c, USART_t * USART){
	USART->DATA = c;
	while(!(USART->STATUS & (1 << 5)));
}

/*	Send array of chars, over E1 USART when
*	sending type it like example: 
*	sendMsg("text of your message");
 */
void sendMsg(char *poruka, USART_t * usartic)
{
	while(*poruka != '\0'){
		SendChar(*poruka,usartic);
		//sendChar(*poruka);
		poruka++;
	}
}

void idi_pravo_HC(unsigned int x, unsigned int y, unsigned int ugao){
	
	
	SendChar('A',&USART_XDRIVE);
	SendChar((x>>8),&USART_XDRIVE);
	SendChar(x,&USART_XDRIVE);
	SendChar((y>>8),&USART_XDRIVE);
	SendChar(y,&USART_XDRIVE);
	SendChar((ugao>>8),&USART_XDRIVE);
	SendChar(ugao,&USART_XDRIVE);
	SendChar('X',&USART_XDRIVE);
	//
	//
	//SendChar((char)(x>>8),&USART_XM);
	//SendChar((char)x,&USART_XM);
	//SendChar((char)(y>>8),&USART_XM);
	//SendChar((char)y,&USART_XM);
	//SendChar((char)(ugao>>8),&USART_XM);
	//SendChar((char)ugao,&USART_XM);
	
	//SendChar('A',&USART_XDRIVE); //0
	//SendChar('1',&USART_XDRIVE);
	//SendChar('2',&USART_XDRIVE);
	//SendChar('3',&USART_XDRIVE);
	//SendChar('4',&USART_XDRIVE);
	//SendChar('5',&USART_XDRIVE);
	//SendChar('6',&USART_XDRIVE);
	//SendChar('X',&USART_XDRIVE);
	
	SendChar('A',&USART_XM); //0
	SendChar(x>>8,&USART_XM);
	SendChar(x,&USART_XM);
	SendChar(y>>8,&USART_XM);
	SendChar(y,&USART_XM);
	SendChar(ugao>>8,&USART_XM);
	SendChar(ugao,&USART_XM);
	SendChar('X',&USART_XM);

}