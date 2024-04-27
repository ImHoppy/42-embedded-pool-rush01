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
#define MAX_MODE 4

volatile uint16_t display_value = 0;
volatile bool display_point = false;

volatile	uint8_t current_mode = 0;

void rv1(void)
{
	display_value = adc_read(0);
}
void ldr(void)
{
	display_value = 1;
}
void ntc(void)
{
	display_value = 2;
}

void firmware_bootup()
{
	// Set D1, D2, D3, D4 as output
	DDRB |= _BV(PIN0) | _BV(PIN1) | _BV(PIN2) | _BV(PIN4);
	// Turn on D1, D2, D3, D4 for 3 sec
	PORTB ^= (_BV(PIN0) | _BV(PIN1) | _BV(PIN2) | _BV(PIN4));
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
		static uint8_t	color = 1;

		if (color == 0)
			set_leds_spi((uint8_t [3][3]){{100, 0, 0}, {100, 0, 0}, {100, 0, 0}});//set all spi leds as RED
		else if (color == 1)
			set_leds_spi((uint8_t [3][3]){{0, 100, 0}, {0, 100, 0}, {0, 100, 0}});//set all spi leds as GREEN
		else
			set_leds_spi((uint8_t [3][3]){{0, 0, 100}, {0, 0, 100}, {0, 0, 100}});//set all spi leds as BLUE
		color = (color + 1) % 2;
	}
}

ISR(TIMER0_COMPA_vect)
{
	switch (current_mode)
	{
		case 0:
		{
			break;
		}

		case 1:
		{
			break;
		}

		case 2:
		{
			break;
		}

		case 3:
		{
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

		default:
		{
			break;
		}
	}

	if (current_mode != 4)
	{
		seg7_display_number(display_value, display_point);
	}
}

#define SW1 (1 << 2)
#define SW2 (1 << 4)

int main()
{
	adc_init(ADC_NORMAL);
	i2c_init();
	uart_init(UART_ALL);
	spi_init();

	seg7_init();
	firmware_bootup();

	timer0_init(3);
	timer0_COMP();

	mode_4_setup();
	current_mode = 4;
	while (1)
	{
		current_mode = 4;
		// if ((PIND & SW1))
		// {
		// 	current_mode = (current_mode - 1) % MAX_MODE;
		// }
		// if ((PIND & SW2))
		// {
		// 	current_mode = (current_mode + 1) % MAX_MODE;
		// }
		// if (current_mode > MAX_MODE)
		// {
		// 	current_mode = 2;
		// }
		// rv1();
	}
}
