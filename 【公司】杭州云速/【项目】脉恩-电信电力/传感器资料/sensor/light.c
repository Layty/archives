/*
 * light.c
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */

#include "include.h"
#include <string.h>
#define CONFIG_LIGHT_BUFF_SIZE 20
static u16 ltmp[CONFIG_LIGHT_BUFF_SIZE];

void light_init()
{
    memset(ltmp,0,30);
    ADC_CR1=0x70;
    ADC_CSR=0x20;
    ADC_CSR|=(0x04&0x07);       //PD3,AIN4
//    ADC_AWCRL|=0x10;
    ADC_CR2_ALIGN=1;
//    ADC_TDRL = 0x01;
    ADC_CR1_ADON=1;
}
void light_start()
{
    ADC_CR1_ADON=1;
}
#pragma vector=ADC1_AWS0_vector
__interrupt void ADC1_AWS0_Handler()
{
    //for this interrupt is triggered every 2000ms,so the value of simbol light will be updated every 60s.
    static u8 dh,dl;
    static u8 ltmp_cnt;
    static u16 tmp;
    static u16 sum;
    sys_int_disable();
    if(ADC_CSR&0x80)
    {
        ADC_CSR&=~0x80;
        dl=ADC_DRL;
        dh=ADC_DRH;
        tmp=((unsigned int)dl)|(((unsigned int)dh)<<8);
        sum+=tmp;   //add curent value to sum
        ltmp[ltmp_cnt]=tmp; //save value 
        if(++ltmp_cnt>=CONFIG_LIGHT_BUFF_SIZE){
            ltmp_cnt=0;
            sum-=ltmp[0];
        }else{
            sum-=ltmp[ltmp_cnt];    //remove the oldest value from sum;
        }
        light=sum/CONFIG_LIGHT_BUFF_SIZE;
    }
    sys_int_enable();
}
