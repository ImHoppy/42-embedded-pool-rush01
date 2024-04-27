#include <avr/io.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include <adc.h>
#include <7segment.h>
#include <timer.h>

// D9 P15
// D10 P16
// D11 P17

#define D9 (1 << 3)
#define D10 (1 << 2)
#define D11 (1 << 1)

volatile uint16_t display_value = 0;
volatile bool display_point = false;

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

ISR(TIMER0_COMPA_vect)
{
	seg7_display_number(display_value, display_point);
}

#define SW1 (1 << 2)
#define SW2 (1 << 4)

int main()
{
	adc_init(ADC_NORMAL);
	i2c_init();
	uart_init(UART_ALL);

	seg7_init();
	firmware_bootup();

	timer0_init(3);
	timer0_COMP();

	uint8_t current_mode = 0;
	// const uint8_t max_mode = 3;
	while (1)
	{
		// if ((PIND & SW1))
		// {
		// 	current_mode = (current_mode - 1) % max_mode;
		// }
		// if ((PIND & SW2))
		// {
		// 	current_mode = (current_mode + 1) % max_mode;
		// }
		// if (current_mode > max_mode)
		// {
		// 	current_mode = 2;
		// }
		rv1();
	}
}
