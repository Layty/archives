/*
 * clock.c
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */
#include "clock.h"
#define CONFIIG_MSECOND_OF_MINUTE (60)
#define CONFIIG_MSECOND_OF_HOUR   (3600ul)  
#define CONFIIG_MSECOND_OF_DAY    (86400ul)

//static const u8 day_of_month[]={31,28,31,30,31,30,31,31,30,31,30,31};

#define	IsLeapYear(yr)	(!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
 
#include "clock.h"

static u32 seconds;

datetime_t clock_get2()
{
    datetime_t dt;
    dt.second=seconds%CONFIIG_MSECOND_OF_MINUTE;
    dt.minute=(seconds%CONFIIG_MSECOND_OF_HOUR)/CONFIIG_MSECOND_OF_MINUTE;
    dt.hour=(seconds%CONFIIG_MSECOND_OF_DAY)/CONFIIG_MSECOND_OF_HOUR;
    dt.day=0;
    dt.month=0;
    dt.year=0;
    return dt;
}

u32 clock_get()
{
	return seconds;
}
void clock_set(datetime_t *dt)
{
    (void)dt;
}
void clock_update()
{
	seconds++;
}
