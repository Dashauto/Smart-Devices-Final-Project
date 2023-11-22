/*
 * SPI_test1_master.c
 *
 * Created: 2023/11/22 12:43:17
 * Author : YINING
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define TRIG PD2
#define ECHO PD3

#define set(reg,bit) (reg) |= (1<<(bit))
#define clear(reg,bit) (reg) &= ~(1<<(bit))
#define toggle(reg,bit) (reg) ^= (1<<(bit))

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void lcd_init(void);

// Function Declarations
void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void SPI_ControllerTx(uint8_t data);
void SPI_ControllerTx_stream(uint8_t stream);




void SPI_MasterInit(void) {
	// Set MOSI and SCK as Output
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);  //Set up output pins
	
	set(PORTB, PORTB2);

	// Enable SPI, Master, set clock rate
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

/**************************************************************************//**
* @fn			void SPI_ControllerTx(uint8_t data)
* @brief		Send 8-bit SPI data to peripheral
* @note
*****************************************************************************/
void SPI_ControllerTx(uint8_t data)
{
	clear(PORTB, PORTB2);	//CS pulled low to start communication

	SPI_ControllerTx_stream(data);

	set(PORTB, PORTB2);	//set CS to high
}

/**************************************************************************//**
* @fn			void SPI_ControllerTx_stream(uint8_t stream)
* @brief		Send a command to LCD through SPI without setting CS or DC
* @note
*****************************************************************************/
void SPI_ControllerTx_stream(uint8_t stream)
{
	SPDR = stream;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
}


int main(void) {
	SPI_MasterInit();
	uint8_t A = 65;
	while (1) {
		SPI_ControllerTx(A);  // Transmit 'A'
		//_delay_ms(1000);
		
	}
}
