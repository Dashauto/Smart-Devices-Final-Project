/*
 * SPI_TEST_1.c
 *
 * Created: 2023/11/22 12:07:46
 * Author : Dashauto
 */ 


#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define MYUBRR F_CPU/16/BAUD-1
#define DDR_SPI DDRB
#define DD_MISO DDB4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char String[50];

void UART_init(void)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/* Set frame format: 2 stop bits, 8 data bits */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 data bits
	UCSR0C |= (1<<USBS0); // 2 stop bits
}

void UART_send( unsigned char data)
{
	// Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));
	// Put data into buffer and send data
	UDR0 = data;
}

void UART_putstr(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		UART_send(*StringPtr);
		StringPtr++;
	}
}



void SPI_SlaveInit(void) {
	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);  // Set MISO as output
	
	SPCR = (1<<SPE);         // Enable SPI
	
}

char SPI_SlaveReceive(void) {
	while(!(SPSR & (1<<SPIF)));  // Wait for reception complete
	return SPDR;                 // Return data register
}

int main(void) {
	SPI_SlaveInit();
	UART_init();
	while(1) {
		char received = SPI_SlaveReceive();
		// Process received data
		sprintf(String,"the data is %c\r\n", received);
		UART_putstr(String);
		_delay_ms(200);
	}
}


