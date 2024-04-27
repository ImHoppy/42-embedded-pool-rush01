#include <avr/io.h>
#include <adc.h>

extern volatile uint16_t displed_value;

void mode_rv1(void)
{
	displed_value = adc_read(0);
}
void mode_ldr(void)
{
	displed_value = adc_read(1);
}
void mode_ntc(void)
{
	displed_value = adc_read(2);
}
