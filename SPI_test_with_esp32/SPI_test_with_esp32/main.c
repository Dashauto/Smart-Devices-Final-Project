/*
 * SPI_test_with_esp32.c
 *
 * Created: 2023/11/22 17:26:44
 * Author : YINING
 */ 

// ATmega328P SPI code without Arduino library

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#define set(reg,bit) (reg) |= (1<<(bit))
#define clear(reg,bit) (reg) &= ~(1<<(bit))
#define toggle(reg,bit) (reg) ^= (1<<(bit))

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define MOSI 3
#define MISO 4
#define SCK 5
#define SS 2

void UART_init(void);
void UART_transmit(unsigned char data);
void UART_putstring(char* StringPtr);

char String[25];

void UART_init(void);
void UART_transmit(unsigned char data);
void UART_putstring(char* StringPtr);

void UART_init(void)
{
	
	/*Set baud rate */
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* 1 stop bits & 8 data bits */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 data bits
}

void UART_transmit(unsigned char data)
{
	// Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));
	// Put data into buffer and send data
	UDR0 = data;
	
}

void UART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		UART_transmit(*StringPtr);
		StringPtr++;
	}
}


void SPI_MasterInit(void) {
	// Set MOSI and SCK as Output
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);  //Set up output pins
	
	set(PORTB, PORTB2);

	// Enable SPI, Master, set clock rate as default
	//SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	SPCR = (1 << SPE) | (1 << MSTR);
}

void SPI_MasterTransmit_stream(char cData) {
	// Start transmission
	SPDR = cData;
	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF)));
}

void SPI_MasterTransmit(char cData)
{
	clear(PORTB, PORTB2);	//CS pulled low to start communication

	SPI_MasterTransmit_stream(cData);

	set(PORTB, PORTB2);	//set CS to high
}

char SPI_MasterReceive_stream(void) {
	// Transmit dummy byte to receive from slave
	SPDR = 0xFF;

	// Wait for reception complete
	while(!(SPSR & (1<<SPIF)));

	// Return Data Register
	return SPDR;
}

char SPI_MasterReceive(void) {
	clear(PORTB, PORTB2);	//CS pulled low to start communication

	char receivedData = SPI_MasterReceive_stream();

	set(PORTB, PORTB2);	//set CS to high
	return receivedData;
}

int main(void) {
	SPI_MasterInit();
	UART_init();
	
	while(1) {
		// Transmit data to ESP32
		SPI_MasterTransmit('A'); // Example data
		//sprintf(String, "try to receive\r\n");
		//UART_putstring(String);
		// Receive data from ESP32
		char receivedData = SPI_MasterReceive();
		
		sprintf(String, "data received: %c \r\n", receivedData);
		UART_putstring(String);
		
		// Add a delay
		_delay_ms(1000);
	}
}
