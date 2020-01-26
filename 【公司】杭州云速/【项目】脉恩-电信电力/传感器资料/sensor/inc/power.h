/*
 * power.h
 *
 *  Created on: 2015��12��10��
 *      Author: Admin
 */

#ifndef INC_POWER_H_
#define INC_POWER_H_

#include "typedef.h"

typedef struct
{
	u32 power;
	u32 votage;
	u32 current;
	u32 energy;
}power_storage_t;
typedef struct
{
	u16 votage;
	u16 power;
	u16 current;
}power_factor_t;

#endif /* INC_POWER_H_ */
