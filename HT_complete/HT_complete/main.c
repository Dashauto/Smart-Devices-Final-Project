/*
 * HandT_TEST.c
 *
 * Created: 2023/11/13 11:58:06
 * Author : zedempire
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define DHT11_PIN PORTD2 // Connect DHT11 data line to PD2


/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include "uart.h"

/*--------------------Variables---------------------------*/
char String[50];
void UART_init()
{
	UBRR0H = (unsigned char)(BAUD_PRESCALER >> 8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 2 stop bits, 8 data bits */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits
	UCSR0C |= (1 << USBS0); // 2 stop bits
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


void DHT11_start(void) {
	DDRD |= (1 << DHT11_PIN); // Set pin as output
	PORTD &= ~(1 << DHT11_PIN); // Pull pin low
	_delay_ms(20); // Wait for 20ms
	PORTD |= (1 << DHT11_PIN); // Pull pin high
	_delay_us(40); // Wait for 40us
	DDRD &= ~(1 << DHT11_PIN); // Set pin as input
}

uint8_t DHT11_check_response(void) {
	uint8_t response = 0;
	_delay_us(40);
	if (!(PIND & (1 << DHT11_PIN))) {
		_delay_us(80);
		if ((PIND & (1 << DHT11_PIN))) response = 1;
	}
	_delay_us(40);
	return response;
}


uint8_t DHT11_read_data(void) {
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

int main(void) {
	uint8_t humidity, temperature;
	UART_init();
	

	while (1) {
		
		//read_dht11_dat();
		///*
		DHT11_start();

		// Read and process the data from DHT11
		 if (DHT11_check_response()) {
			 humidity = DHT11_read_data();
			 DHT11_read_data();
			 temperature = DHT11_read_data();
			 // Process or display the temperature and humidity
			 sprintf(String, "Humidity: %d%%, Temperature: %dC\r\n", humidity, temperature);
			 UART_putstr(String);
		 }
		//*/
		// Process the read values of humidity and temperature

		_delay_ms(1000); // DHT11 should be read at intervals of at least 1 second
	}
}

