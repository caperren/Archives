#define SENDSPEED 4	

unsigned char InitializeUART (){
		unsigned char baud = 12; //((F_CPU/(9600*8UL))-1);
		
		/* Set baud rate */
		UBRR1H = (baud >> 8);
		UBRR1L = baud;
		/* Set the U2X1 bit */
		UCSR1A = (1 << U2X1);
		/* Enable transmitter */
		UCSR1B = (1 << TXEN1 | 1 << RXEN1);
		/* Set frame format: 8data, 1stop bit */
		UCSR1C = (3 << UCSZ10);

		return 0;
	}

/** This function needs to write a single byte to the UART. It must check that the UART is ready for a new byte 
and return a 1 if the byte was not sent. 
@param [in] data This is the data byte to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/

unsigned char SendByteUART (unsigned char data){
		
	/* 
	** Checks the buffer empty bit so that data can either be placed in the buffer
	** or else reuturn a 1
	*/
	if((UCSR1A & (1 << UDRE1)) == (1 << UDRE1)){
		UDR1 = data;
		_delay_ms(SENDSPEED);			//Places data into the UDR1 8-bit data buffer
	}else {
		return 1;
	}

	return 0;
}

/** This function needs to writes a string to the UART. It must check that the UART is ready for a new byte and 
return a 1 if the string was not sent. 
@param [in] data This is a pointer to the data to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/

unsigned char SendStringUART (unsigned char *data){
	
	for (int loop = 0 ; data[loop] != '\0'; loop++){
		unsigned char bytereturn;	//Storage for return value of SendByteUART
				
		/* Sends the current byte based on the loop umber to SendByte */
		bytereturn = SendByteUART(data[loop]);
		
		/* Stops sending if SendByteUART had an error */
		if(bytereturn == 1){
			return 1;
		}	
	}
	return 0;
}

unsigned char GetByteUART(){
	while(!(UCSR1A & (1 << RXC1)));
		return(UDR1);	
}

unsigned char NewScreen(unsigned char STRING1[], unsigned char STRING2[]){
	unsigned char UARTREAD[17];
	unsigned char loop = 0;
	
	for(; loop < 17 ; loop++){
		while(!(UARTREAD[loop] = GetByteUART()));
	}
	for(loop = 0 ; loop < 8 ; loop ++){
		STRING1[loop] = UARTREAD[loop];
	}	
	STRING1[8] = '\0';
	
	for(; loop < 16 ; loop++){
		STRING2[(loop-8)] = UARTREAD[loop];
	}
	
	SendStringUART(STRING1);
	SendStringUART(STRING2);
	SendStringUART("\n");
	return 0;
}