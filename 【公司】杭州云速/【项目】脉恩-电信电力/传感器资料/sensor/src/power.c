/*
 * power.c
 *
 *  Created on: 2015��12��10��
 *      Author: Admin
 */
#include "power.h"
#include "include.h"
#include <string.h>

void power_Init()
{
    
}

u8 power_SetEnergy(u32 e)
{
    sys_int_disable();
    power_storage.energy=e;
    sys_int_enable();
    return 0;
}
#pragma vector=EXTI2_vector
__interrupt void EXIT2_Handler()
{
    cnt_p++;
    cnt_e++;    
}

//������ѹ����
#pragma vector=EXTI1_vector
__interrupt void EXIT1_Handler()
{
    cnt_v++;
    cnt_c++;
}