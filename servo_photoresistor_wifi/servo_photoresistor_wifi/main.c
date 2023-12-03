/*
 * servo_photoresistor_wifi.c
 *
 * Created: 2023/11/24 18:00:11
 * Author : YINING
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define TRIG PD2
#define ECHO PD3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char rx_String[25];
char receivedData = 0;
int rx_finish;
int buffer_index = 0;
int rx_trigger = 0;

int user_open = 0;
int user_close = 0;
int user_air_open = 0;
int user_air_close = 0;
int curtain_decision = 0;
int AC_decision = 0;


volatile int duty_cycle = 0;
char String[50];
int ADC_interrupt  = 0;
int curtain_open = 0;
int sec_cnt = 0;

volatile uint16_t timer2_ticks = 0;
volatile uint16_t timer1_ticks = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

void timer1_init()
{
	// Set up timer with prescaler = 256 and CTC mode
	TCCR1B |= (1 << WGM12) | (1 << CS12);

	// Initialize counter
	TCNT1 = 0;

	// Set compare match register for 1 sec overflow
	// 16000000/256/1Hz = 62500 (for 16MHz AVR clock)
	OCR1A = 62500;

	// Enable Timer1 compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	timer1_ticks++;
	
	seconds++;
	if (seconds >= 60) 
	{
		minutes++;
		seconds = 0;
		if (minutes >= 60) 
		{
			hours++;
			minutes = 0;
			if (hours >= 24) 
			{
				hours = 0;
			}
		}
	    //timer1_ticks = 0;
	}
	// sprintf(String, "Time counting now is: %d: %d: %d\r\n", hours, minutes, seconds);
	// UART_putstr(String);
	
}


//void initialize_timer2() {
    //TCCR2A = 0; // Set to normal mode
    //TCCR2B = (1 << CS22); // Prescaler set, e.g., 64
    //TIMSK2 = (1 << TOIE2); // Enable overflow interrupt
    //TCNT2 = 0; // Initialize counter
//}
//
//ISR(TIMER2_OVF_vect) {
    //timer2_ticks++; // Increment the timer ticks
	//if(timer2_ticks >= 100){
		//timer2_ticks = 0;
		//sec_cnt ++;
	//}
//}

void UART_init()
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	/* Set frame format: 2 stop bits, 8 data bits */
	UCSR0C |= (1<<USBS0);
	UCSR0C |= (3<<UCSZ00);
	
	//UCSR0B |= RXCIE0;
	UCSR0B |= (1 << RXCIE0);
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

ISR(USART_RX_vect) {
	receivedData = UDR0; // Read the received data
	rx_trigger = 1;
	
	//sprintf(String,"data: %c\r\n", receivedData);
	//UART_putstr(String);
	
}

void rx_data_handling()
{
	if (rx_String[0] == 'A')
	{
		user_open = 1;
	}
	else if (rx_String[0] == 'B')
	{
		user_close = 1;
	}
	else if (rx_String[0] == 'C')
	{
		user_air_open = 1;
	}
	else if (rx_String[0] == 'D')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'E')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'F')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'G')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'H')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'I')
	{
		user_air_close = 1;
	}
	else if (rx_String[0] == 'J')
	{
		curtain_decision = 1;
	}
	else if (rx_String[0] == 'K')
	{
		AC_decision = 1;
	}
	
	
	
}

void Initialization()
{
	
	DDRD |= (1<<DDD6);
	DDRB &= ~(1<<DDB0);
	DDRB &= ~(1<<DDB1);
	
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

}

void Initialize_ADC()
{
	PRR &= ~(1<<PRADC);
	
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
	
	ADMUX &= ~(1<<MUX0);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX3);
	
	ADCSRA |= (1<<ADATE);
	
	ADCSRB &= ~(1<<ADTS0);
	ADCSRB &= ~(1<<ADTS1);
	ADCSRB &= ~(1<<ADTS2);
	
	DIDR0 |= (1<<ADC0D);
	
	ADCSRA |= (1<<ADEN);
	
	ADCSRA |= (1<<ADIE);
	
	ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect)
{
	ADC_interrupt = 1;
}

int main(void)
{
	cli();
	Initialization();
	Initialize_ADC();
	//initialize_timer2();
	timer1_init();
	UART_init();
	sei();
	while (1)
	{
		if (ADC_interrupt)
		{
			sprintf(String, "ADC: %u  sec_cnt: %u  OCR0A: %u\r\n",ADC, sec_cnt,OCR0A);
			UART_putstr(String);
			ADC_interrupt = 0;
			if ((ADC >= 900 || (PINB & (1 << PINB1)) ) && !curtain_open)
			{
				OCR0A = 80 * 2;

                // Reset the timer ticks if needed
				//sec_cnt = 0;
				//timer2_ticks = 0;
				timer1_ticks = 0;

				// toggle curtain_open
				curtain_open = !curtain_open;
			}
			else if ((ADC <= 300 || (PINB & (1 << PINB0)) ) && curtain_open)
			{
				OCR0A = 106 * 2;

				// Reset the timer ticks if needed
				//sec_cnt = 0;
				//timer2_ticks = 0;
				timer1_ticks = 0;

				// toggle curtain_open
				curtain_open = !curtain_open;
			}

			// if reaches 5 seconds;
			if (timer1_ticks >= 5) 
			{
				// set OCR0A tp 93*2
				OCR0A = 93*2;	 
			}
			

			
		}
	}
}

