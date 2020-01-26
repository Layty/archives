/*
 * ds18b20.h
 *
 *  Created on: 2015年12月10日
 *      Author: Admin
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "include.h"

#define CONFIG_W1_ID_LEN       8

typedef struct
{
    u8 (*init)(void);
    void (*set_dir)(u8 dir);
    void (*set)(u8 value);
    u8 (*get)(void);
    void (*delay)(u16 t);
//    u8 id[CONFIG_W1_ID_LEN];   //序列号
}w1_t;

s16 ds18b20_getTM(w1_t *w1);
#endif /* INC_DS18B20_H_ */
