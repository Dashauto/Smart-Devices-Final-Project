/*
 * Time_counting.c
 *
 * Created: 2023/11/18 21:21:12
 * Author : Dashauto
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint32_t timer1_overflow_count = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

char String[50];

void UART_init()
{
	UBRR0H = (unsigned char)(BAUD_PRESCALER >> 8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 2 stop bits, 8 data bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits
}

void UART_putstr(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		while(!(UCSR0A & (1 << UDRE0)));
		// Put data into buffer and send data
		UDR0 = *StringPtr;
		// Wait for empty transmit buffer
		StringPtr++;
	}
}


void timer1_init()
{
	cli();
	// Set up timer with prescaler = 256 and CTC mode
	TCCR1B |= (1 << WGM12) | (1 << CS12);

	// Initialize counter
	TCNT1 = 0;

	// Set compare match register for 1 sec overflow
	// 16000000/256/1Hz = 62500 (for 16MHz AVR clock)
	OCR1A = 62500;

	// Enable Timer1 compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

ISR(TIMER1_COMPA_vect) {
	timer1_overflow_count++;
	
		seconds++;
		if (seconds >= 60) {
			minutes++;
			seconds = 0;
			if (minutes >= 60) {
				hours++;
				minutes = 0;
				if (hours >= 24) {
					hours = 0;
				}
			}
		timer1_overflow_count = 0;
	}
	sprintf(String, "Time counting now is: %d: %d: %d\r\n", hours, minutes, seconds);
	UART_putstr(String);
	
}

int main(void) {
	// Initialize Timer1 and UART
	
	timer1_init();
	UART_init();
	
	while (1) {
		
	}

	return 0;
}



