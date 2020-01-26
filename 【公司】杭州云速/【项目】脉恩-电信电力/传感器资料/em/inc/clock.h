/*
 * clock.h
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */

#ifndef SRC_CLOCK_H_
#define SRC_CLOCK_H_

#include "include.h"

#define CLOCK_UTCTIME_MAX (0xfffffffful)
typedef struct
{
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
}datetime_t;
u32 clock_get();
void clock_set(datetime_t *dt);
datetime_t clock_get2();
void clock_update();
#endif /* SRC_CLOCK_H_ */
