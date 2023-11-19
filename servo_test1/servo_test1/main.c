/*
 * servo_test1.c
 *
 * Created: 2023/11/18 17:28:53
 * Author : YINING
 */ 

#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define F_CPU 16000000UL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void Initialization()
{
	cli();
	DDRD |= (1<<DDD6);
	
	// prescale to 16M/64 = 250000Hz
	TCCR0B |= (1<<CS00);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);
	
	// Phase Correct PWM mode
	TCCR0A |= (1<<WGM00);
	TCCR0A &= ~(1<<WGM01);
	TCCR0B |= (1<<WGM02);
	
	// Set the compare match value to create an overflow interrupt
	OCR0A = 93*2; // 250000/440/4 = 142
	TCCR0A |= (1 << COM0A0);

	
	sei();
}

int main(void)
{
	Initialization();
	while (1)
	{
		_delay_ms(100);
		OCR0A = 80*2;
		_delay_ms(500);
		OCR0A = 93*2;
		_delay_ms(100);
		OCR0A = 110*2;
		_delay_ms(500);
		OCR0A = 93*2;
	}
}
