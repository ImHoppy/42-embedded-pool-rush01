#ifndef LED_SPI_h
#define LED_SPI_h

#include <avr/io.h>

void	spi_init();//initializing SPI as master
void	spi_send(uint8_t data);
void	start_frame();
void	led_frame(uint8_t r, uint8_t g, uint8_t b);
void	end_frame();
void	set_leds(uint8_t RGB[3][3]);

#endif