/*
 * Smart Curtain Opener Atmega328P Code
 *
 * Created: 2023/11/24 18:00:11
 * Author : YINING XIA, WEIHENG ZHUANG, TIANYU GAO
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define DHT11_PIN PORTD2 // Connect DHT11 data line to PD2
#define AC_PIN PORTD3


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

volatile uint8_t user_open = 0;
volatile uint8_t user_close = 0;
volatile uint8_t user_air_open = 0;
volatile uint8_t user_air_close = 0;

volatile uint8_t time_open = 0;
volatile uint8_t time_open_hour = 0;
volatile uint8_t time_open_minute = 0;
volatile uint8_t time_close = 0;
volatile uint8_t time_close_hour = 0;
volatile uint8_t time_close_minute = 0;

volatile uint8_t last_temp = 0;
volatile uint8_t temp_on = 0;
volatile uint8_t temp_close = 0;
volatile uint8_t temp_on_val = 0;
volatile uint8_t temp_close_val = 0;
volatile uint8_t current_time = 0;


int set_current_time = 0;


volatile uint8_t curtain_decision = 0;
volatile uint8_t AC_decision = 0;


volatile int duty_cycle = 0;
char String[50];
int ADC_interrupt  = 0;
int curtain_open = 0;
int sec_cnt = 0;

//volatile uint16_t timer2_ticks = 0;
volatile uint16_t timer1_ticks = 0;
volatile uint8_t HT_start = 0;
volatile uint8_t HT_record = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;
volatile uint8_t humidity, temperature;


void rx_data_handling();

void flag_init() {
	user_open = 0;
	user_close = 0;
	user_air_open = 0;
	user_air_close = 0;

	time_open = 0;
	time_open_hour = 0;
	time_open_minute = 0;
	time_close = 0;
	time_close_hour = 0;
	time_close_minute = 0;

	temp_on = 0;
	temp_close = 0;
	current_time = 0;

	set_current_time = 0;
}


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

ISR(USART_RX_vect) 
{
	receivedData = UDR0; // Read the received data
	rx_trigger = 1;
	
}

void DHT11_start(void) 
{
	DDRD |= (1 << DHT11_PIN); // Set pin as output
	PORTD &= ~(1 << DHT11_PIN); // Pull pin low
	_delay_ms(20); // Wait for 20ms
	PORTD |= (1 << DHT11_PIN); // Pull pin high
	_delay_us(40); // Wait for 40us
	DDRD &= ~(1 << DHT11_PIN); // Set pin as input
}

uint8_t DHT11_check_response(void) 
{
	uint8_t response = 0;
	_delay_us(40);
	if (!(PIND & (1 << DHT11_PIN))) {
		_delay_us(80);
		if ((PIND & (1 << DHT11_PIN))) response = 1;
	}
	_delay_us(40);
	return response;
}

uint8_t DHT11_read_data(void) 
{
	uint8_t i, j;
	uint8_t data = 0;
	for (j = 0; j < 8; j++) {
		while (!(PIND & (1 << DHT11_PIN))); // Wait for pin to go high
		_delay_us(30);
		if (!(PIND & (1 << DHT11_PIN))) // If pin is low
		i = 0;
		else
		i = 1;
		_delay_us(40);
		if (i)
		data |= (1 << (7 - j)); // Set bit (7 - j)
	}
	return data;
}

void readHandt()
{
	DHT11_start();

	// Read and process the data from DHT11
	if (DHT11_check_response()) 
	{
		humidity = DHT11_read_data();
		DHT11_read_data();
		temperature = DHT11_read_data();
		// Process or display the temperature and humidity
		sprintf(String, "Y%c%c\n", (char) (humidity + 32), (char) (temperature + 32));
		UART_putstr(String);

		sprintf(String, "X%c%c\n", (char) (hours + 32), (char) (minutes + 32));
		UART_putstr(String);
	}
}

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
	HT_record++;
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
	}
	
}

// Air-conditioner a.k.a DC motor
void AC_control(int on)
{
	if (on) 
	{
		PORTD |= (1 << AC_PIN);
	}
	else
	{
		PORTD &= ~(1 << AC_PIN); // Pull pin low
	}
	
}


void rx_data_handling()
{

    // open curtain button
	if (rx_String[0] == 'A')
	{
		user_open = 1;
	}
	// close curtain button
	else if (rx_String[0] == 'B')
	{
		user_close = 1;
	}
	// open A/C button
	else if (rx_String[0] == 'C')
	{
		user_air_open = !user_air_open;
	}
	// close A/C button
	else if (rx_String[0] == 'D')
	{
		user_air_close = 1;
	}
	// set time to open curtain
	else if (rx_String[0] == 'E')
	{
		time_open = 1;
		time_open_hour = (int) (rx_String[1] - 32);
		time_open_minute = (int) (rx_String[2] - 32);
	}
	// set time to close curtain
	else if (rx_String[0] == 'F')
	{
		time_close = 1;
		time_close_hour = (int) (rx_String[1] - 32);
		time_close_minute = (int) (rx_String[2] - 32);
	}
	// set temperature to open A/C
	else if (rx_String[0] == 'G')
	{
		temp_on = 1;
		temp_on_val = ((int) rx_String[1] -48) * 10 + (int) rx_String[2] -48;
	}
	// set temperature to close A/C
	else if (rx_String[0] == 'H')
	{
		temp_close = 1;
		temp_close_val += ((int) rx_String[1] -48) * 10 + (int) rx_String[2] - 48;	
	}
	// set current time
	else if (rx_String[0] == 'I')
	{
		set_current_time = 1;
	}
	// make smart decision (curtain)
	else if (rx_String[0] == 'J')
	{
		curtain_decision = !curtain_decision;
	}
	// make smart decision (A/C)
	else if (rx_String[0] == 'K')
	{
		AC_decision = !AC_decision;
	}
}

void Initialization()
{
	// AC pin
	DDRD |= (1 << AC_PIN); // Set pin as output
	
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

void open_close_time()
{
	if (time_open && !curtain_open)
	{

		if(hours == time_open_hour && minutes == time_open_minute)
		{
			OCR0A = 80 * 2;
			time_open = 0;

			// Reset the timer ticks
			timer1_ticks = 0;
			
			// toggle curtain_open
			curtain_open = !curtain_open;
		}
	}
	else if (time_close && curtain_open)
	{

		if(hours == time_close_hour && minutes == time_close_minute)
		{
			OCR0A = 106 * 2;
			time_close = 0;

			// Reset the timer ticks
			timer1_ticks = 0;
			
			// toggle curtain_open
			curtain_open = !curtain_open;
		}
	}
	
}

int main(void)
{
	cli();
	Initialization();
	Initialize_ADC();
	timer1_init();
	UART_init();
	sei();
	while (1)
	{
		// If reaches 5s
		if (timer1_ticks >= 5)
		{
			// set OCR0A to 93*2
			OCR0A = 93*2;
			//flag_init();
		}
		if (HT_record >= 5)
		{
			readHandt();
			HT_record = 0;
		}
		
		// control air-conditioner based on different user inputs	
		if (AC_decision && temperature >= 25)
		{
			AC_control(1);
		} 
		else if ((!AC_decision) && user_air_open)
		{
			AC_control(1);
		}
		else if((!AC_decision) && temp_on && (temperature >= temp_on_val))
		{
			AC_control(1);
		}
		else
		{
			AC_control(0);
			temp_on = 0;
		}
			
		if (rx_trigger)
		{
			if((char) receivedData != '\n' && buffer_index < 24)
			{
				rx_String[buffer_index] = receivedData;
				buffer_index++;
			}
			else
			{
				rx_String[buffer_index] = '\0'; // Null-terminate the string
				buffer_index = 0; // Reset buffer index for the next message
				rx_finish = 1;
				_delay_us(1);
			}
			receivedData = 0;
			rx_trigger = 0;
		}
		if (rx_finish)
		{
			rx_finish = 0;
			rx_data_handling();
			
			sprintf(String, "Temp: %d temp_on: %d RT: %d\r\n", temp_on_val, temp_on, temperature);
			UART_putstr(String);

			//if (user_open && !curtain_decision && !curtain_open)
			if (user_open && !curtain_open)
			{
				OCR0A = 80 * 2;
				user_open = 0;

				// Reset the timer ticks
				timer1_ticks = 0;
				
				// toggle curtain_open
				curtain_open = !curtain_open;
			}
			//else if (user_close && !curtain_decision && curtain_open)
			else if (user_close && curtain_open)
			{
				OCR0A = 106 * 2;
				user_close = 0;

				// Reset the timer ticks
				timer1_ticks = 0;

				// toggle curtain_open
				curtain_open = !curtain_open;
			}
			if (set_current_time) {
				hours = (int) rx_String[1] - 32;
				minutes = (int) rx_String[2] - 32;
				seconds = 0;
				_delay_us(1);
				sprintf(String, "X%c%c\n", (char) (hours + 32), (char) (minutes + 32));
				UART_putstr(String);
				set_current_time = 0;
			}	
		}
		
		open_close_time();
		
		if (curtain_decision && ADC_interrupt)
		{		
			ADC_interrupt = 0;
			if (ADC >= 900 && !curtain_open)
			{
				OCR0A = 80 * 2;

                // Reset the timer ticks
				timer1_ticks = 0;
				
				// toggle curtain_open
				curtain_open = !curtain_open;
			}
			else if (ADC <= 300 && curtain_open)
			{
				OCR0A = 106 * 2;

				// Reset the timer ticks
				timer1_ticks = 0;

				// toggle curtain_open
				curtain_open = !curtain_open;
			}					
		}
	}
}




