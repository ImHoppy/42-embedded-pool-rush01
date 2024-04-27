#include "led_spi.h"

void	spi_init()//initializing SPI as master
{
	PRR &= ~(1 << PRSPI);//disabling power reduction

	//configuring SPI pins
	DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);//setting SS/MOSI/SCK to output
	DDRB &= ~(1 << PB4);//setting MISO to input

	SPCR |= (1 << SPE) | (1 << MSTR);//enabling SPI as master

	SPCR &= ~(1 << DORD);//sets data order as MSB first

	SPCR &= ~(1 << CPOL) & ~(1 << CPHA);//sets clock as mode 0

	//sets clock frequency as /16
	SPCR |= (1 << SPR0);
	SPCR &= ~(1 << SPR1);
}	

void	spi_send(uint8_t data)
{
	SPDR = data;//load data
	while (!(SPSR & (1 << SPIF)));//wait for data to be sent
}

void	start_frame()
{
	for (uint8_t i = 0; i < 4; i++)
		spi_send(0);
}

void	led_frame(uint8_t r, uint8_t g, uint8_t b)
{
	spi_send(0b11100100);
	spi_send(b);
	spi_send(g);
	spi_send(r);
}

void	end_frame()
{
	for (uint8_t i = 0; i < 4; i++)
		spi_send(255);
}

void	set_leds_spi(uint8_t RGB[3][3])
{
	start_frame();
	
	for (uint8_t i = 0; i < 3; i++)
		led_frame(RGB[i][0], RGB[i][1], RGB[i][2]);
		
	end_frame();
}