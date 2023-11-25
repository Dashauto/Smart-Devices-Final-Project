/*
 * LAB3_UART_TEST.c
 *
 * Created: 2023/10/12 16:05:09
 * Author : Dashauto
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>



char String[25];
unsigned char receivedData = 0;

void UART_init(void)
{
	cli();
	/*Set baud rate */
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	/* Set frame format: 2 stop bits, 8 data bits */
	UCSR0C |= (1<<USBS0);
	UCSR0C |= (3<<UCSZ00);
	
	UCSR0B |= RXCIE0;
	sei();
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

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
	;
	/* Get and return received data from buffer */
	return UDR0;
}
/*
ISR(USART_RX_vect) {
	receivedData = UDR0; // Read the received data
	// Add code here to handle the received data
	PORTB &= ~(1 << PORTB5);
}
*/
int main(void)
{
	UART_init();
	DDRB |= (1 << DDB5);
	PORTB |= (1 << PORTB5);
	

	while(1)
	{
		// Print to terminal
		//sprintf(String,"Hello again world! \n");
		//UART_putstr(String);
		char h = 0;
		h = USART_Receive();
		if (h == 'H')
		{
			PORTB ^= (1 << PORTB5);
		}
		//int hi = 0;
		
		//_delay_ms(1000);
	}
}



