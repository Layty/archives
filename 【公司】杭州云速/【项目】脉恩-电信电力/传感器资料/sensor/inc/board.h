/*
 * board.h
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "include.h"
#include "config.h"
#include "../inc/power.h"
#include "clock.h"

#define SDA_PIN BIT(7)
#define SDA_PIN2 BIT(6)

#define BOARD_TEMP_ON_MB        1
#define BOARD_TEMP_EXTERN_MB    2

u8 Board_Init();
u16 Board_GetLight(u8 index);
#define Board_GetPowe() power_Get()
u8 Board_SetEnergy(u32 e);
u8 Board_SetPower(u32 actual); //fix power
u8 Boadr_SetFactor(power_factor_t *factor);
void Board_UpdateTempreture(u8 index);

#endif /* INC_BOARD_H_ */
