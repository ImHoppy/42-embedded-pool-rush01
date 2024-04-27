#include <avr/io.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include <adc.h>
#include <7segment.h>
#include <timer.h>

#include "led_spi.h"
#include "modes.h"

// D9 P15
// D10 P16
// D11 P17

#define D9 (1 << 3)
#define D10 (1 << 2)
#define D11 (1 << 1)
#define MAX_MODE 12

volatile uint16_t displed_value = 0;
volatile bool display_point = false;

volatile uint8_t current_mode = 0;

void firmware_bootup()
{
	// Clear RGB LEDs
	clear_leds_spi();
	SPCR =  ~(1 << SPE);//disabling SPI as master
	// Turn on D1, D2, D3, D4 for 3 sec
	DDRB |= (1 << PB4); 
	PORTB |= (_BV(PIN0) | _BV(PIN1) | _BV(PIN2) | _BV(PIN4));
	seg7_turnall(true);
	_delay_ms(3000);
	PORTB = 0;
	seg7_turnall(false);
	_delay_ms(1000);
}

ISR(TIMER1_OVF_vect)
{
	if (current_mode == 4)
	{
		static uint8_t color = 0;

		if (color == 0)
			set_leds_spi((uint8_t[3][3]){{100, 0, 0}, {100, 0, 0}, {100, 0, 0}}); // set all spi leds as RED
		else if (color == 1)
			set_leds_spi((uint8_t[3][3]){{0, 100, 0}, {0, 100, 0}, {0, 100, 0}}); // set all spi leds as GREEN
		else
			set_leds_spi((uint8_t[3][3]){{0, 0, 100}, {0, 0, 100}, {0, 0, 100}}); // set all spi leds as BLUE
		color = (color + 1) % 3;
	}
}

ISR(TIMER0_COMPA_vect)
{
	switch (current_mode)
	{
	case 0:
	{
		mode_0();
		break;
	}

	case 1:
	{
		mode_1();
		break;
	}

	case 2:
	{
		mode_2();
		break;
	}

	case 3:
	{
		mode_3();
		break;
	}

	case 4:
	{
		mode_4();
		break;
	}

	case 5:
	{
		break;
	}

	case 6:
	{
		break;
	}

	case 7:
	{
		break;
	}

	case 8:
	{
		break;
	}

	case 9:
	{
		break;
	}

	case 10:
	{
		break;
	}

	case 11:
	{
		break;
	}

	default:
	{
		break;
	}
	}

	if (current_mode != 4)
	{
		seg7_display_number(displed_value, display_point);
	}
}

#define SW1 (1 << 2)
#define SW2 (1 << 4)

void test_button(uint8_t button1, uint8_t button2, uint8_t button3)
{
	if (button1)
	{
		// turn on D9
	}
	else
	{
		// turn off D9
	}
	if (button2)
	{
		// turn on D10
	}
	else
	{
		// turn off D10
	}
	if (button3)
	{
		// turn on D11
	}
	else
	{
		// turn off D11
	}
}

void current_mode_display() 
{ 
	SPCR =  ~(1 << SPE);//disabling SPI as master
	DDRB |= (1 << PB4); 
	PORTB = (current_mode & 0b111) | ((current_mode & 0b1000) << 1);
}

int main()
{
	uint8_t button_state1 = 0;
	uint8_t button_state2 = 0;
	uint8_t button_state3 = 0;

	// Set D1, D2, D3, D4 as output
	DDRB = _BV(PIN0) | _BV(PIN1) | _BV(PIN2) | _BV(PIN4);

	adc_init(ADC_NORMAL);
	i2c_init();
	uart_init(UART_ALL);
	// spi_init();

	seg7_init();
	firmware_bootup();

	timer0_init(3);
	timer0_COMP();

	timer1_init(1000);
	timer1_OVF();

	current_mode = 0;
	current_mode_display();
	while (1)
	{
		if (button_state1 == 0)
		{
			if (!(PIND & SW1)) // checking if button 1 is pressed
			{
				button_state1 = 1;
				cli();
				current_mode = (current_mode + 1) % MAX_MODE;
				current_mode_display();
				clear_leds_spi();
				sei();
			}
		}

		if (button_state2 == 0)
		{
			if (!(PIND & SW2)) // checking if button 2 is pressed
			{
				button_state2 = 1;
				if (current_mode == 0)
					current_mode = MAX_MODE;
				else
					current_mode--;
				current_mode_display();
				cli();
				clear_leds_spi();
				sei();
			}
		}
		// do the same for SW3

		if (PIND & SW1) // checking if button 1 is not pressed
			button_state1 = 0;
		if (PIND & SW2) // checking if button 2 is not pressed
			button_state2 = 0;
		// same for SW3

		test_button(button_state1, button_state2, button_state3);
		// turn on leds if buttons are pressed
		// if (current_mode > MAX_MODE)
		// {
		// 	current_mode = 2;
		// }
		// rv1();
	}
}
