#ifndef RTC_H
#define RTC_H

#include <i2c.h>

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} rtc_data;

void rtc_set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint8_t year);
void rtc_set_data(rtc_data data);
rtc_data rtc_get_data(void);

#endif
