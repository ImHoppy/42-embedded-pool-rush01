#include <util/delay.h>
#include <util/twi.h>
#include <avr/io.h>
#include <rtc.h>
#include <uart.h>
#include <stdlib.h>

#define RTC_ADDR 0b1010001

// RTC commands

#define Control_status_1_CMD 0x00 // Data order: TEST1 N STOP N TESTC N N N
#define Control_status_2_CMD 0x01 // Data order: N N N TI_TP AF TF AIE TIE

//  Time and date registers
#define VL_seconds_CMD 0x02		// Data order: VL SECONDS(0 to 59)
#define Minutes_CMD 0x03		// Data order: x MINUTES(0 to 59)
#define Hours_CMD 0x04			// Data order: x x HOURS(0 to 23)
#define Days_CMD 0x05			// Data order: x x DAYS(1 to 31)
#define Weekdays_CMD 0x06		// Data order: x x x x x WEEKDAYS(0 to 6)
#define Century_months_CMD 0x07 // Data order: C x x MONTHS(1 to 12)
#define Years_CMD 0x08			// Data order: YEARS(0 to 99)

void rtc_set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint8_t year)
{
	i2c_start(RTC_ADDR, I2C_WRITE);
	i2c_write(Control_status_1_CMD);
	// Control_status_1
	i2c_write(0x00);
	// Control_status_2
	i2c_write(0x00);
	// Time and date registers
	i2c_write(sec);
	i2c_write(min);
	i2c_write(hour);
	i2c_write(day);
	// Weekdays
	i2c_write(0x00);
	i2c_write(month);
	i2c_write(year);
	i2c_stop();
}

void rtc_set_data(rtc_data data)
{
	rtc_set_time(data.sec, data.min, data.hour, data.day, data.month, data.year);
}

rtc_data rtc_get_data(void)
{
	rtc_data data = {0};
	i2c_start(RTC_ADDR, I2C_WRITE);
	i2c_write(VL_seconds_CMD);
	i2c_stop();
	i2c_start(RTC_ADDR, I2C_READ);
	i2c_read(ACK);
	data.sec = TWDR;
	i2c_read(ACK);
	data.min = TWDR;
	i2c_read(ACK);
	data.hour = TWDR;
	i2c_read(ACK);
	data.day = TWDR;
	// Skipping weekdays
	i2c_read(ACK);
	i2c_read(ACK);
	data.month = TWDR;
	i2c_read(NACK);
	data.year = TWDR;
	i2c_stop();
	return data;
}
