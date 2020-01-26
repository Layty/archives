/*
 * board.c
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */
#include "board.h"
#include "serial.h"
#include "ds18b20.h"
#include "hlw8012.h"
#include "cli.h"
#include "swuart.h"



s16 temp1,temp2; //tempture on board
u16 light;       //light value
power_storage_t power_storage;
system_status_t system_status1,system_status2;
extern energy_factor_t energy_factor;
float hlw_p,hlw_v,hlw_c;
static void Timer2_init();
//static void Timer4_init();
u8 Board_Init()
{
	sys_clk_init();
	serial_init();
    light_init();   //light init
    hlw8012_init(); //power measure init
    Timer2_init();
    
#ifdef SWUART
    sys_disable_swim(); //swim pin used as gpio
    uart_init();    
#ifdef CLI
    cli_init();
    cli_cmd_init();
#endif
#else
    cli_init();
    cli_cmd_init();    
#endif

	return 0;
}
u16 Board_GetLight(u8 index)
{
    if(index==0)
        return light;
	return 0xfffful;
}

u8 Board_SetPower(u32 actual); //fix power
u8 Boadr_SetFactor(power_factor_t *factor);


static void w1_set_dir(u8 dir)
{
    if(dir>0)
    {
        PC_CR1|=SDA_PIN;
        PC_DDR|=SDA_PIN;
    }
    else
    {
        PC_CR1|=SDA_PIN;
        PC_DDR&=~SDA_PIN;
    }
}
static u8 w1_init(void)
{
    PC_CR1|=SDA_PIN;
    PC_DDR|=SDA_PIN;
    return 0;
}
static void w1_set(u8 value)
{
    value>0?(PC_ODR|=SDA_PIN):(PC_ODR&=~SDA_PIN);
}
static u8 w1_get(void)
{
    return PC_IDR&SDA_PIN;
}
static void w1_set_dir2(u8 dir)
{
    if(dir>0)
    {
        PC_CR1|=SDA_PIN2;
        PC_DDR|=SDA_PIN2;
    }
    else
    {
        PC_CR1|=SDA_PIN2;
        PC_DDR&=~SDA_PIN2;
    }
}
static u8 w1_init2(void)
{
    PC_CR1|=SDA_PIN2;
    PC_DDR|=SDA_PIN2;
    return 0;
}
static void w1_set2(u8 value)
{
    value>0?(PC_ODR|=SDA_PIN2):(PC_ODR&=~SDA_PIN2);
}
static u8 w1_get2(void)
{
    return PC_IDR&SDA_PIN2;
}

#pragma optimize=none
static void w1_delay(u16 t)
{
    while(t-->0);
}
static const w1_t w1[]=
{
	[0]={
		w1_init,
		w1_set_dir,
		w1_set,
		w1_get,
		w1_delay,
	},
	[1]={
		w1_init2,
		w1_set_dir2,
		w1_set2,
		w1_get2,
		w1_delay,
	},
};

void Board_UpdateTempreture(u8 index)
{
	s16 tempture = 0;
	
    switch(index)
    {
    case BOARD_TEMP_ON_MB:
    	tempture=ds18b20_getTM((w1_t *)&w1[0]);
		if(tempture != -255)
		{
			temp1 = tempture;
		}
        break;
    case BOARD_TEMP_EXTERN_MB:
    	tempture=ds18b20_getTM((w1_t *)&w1[1]);
		if(tempture != -255)
		{
			temp2 = tempture;
		}
        break;
    default:
        break;
    }
}
static void Timer2_init()
{
    TIM2_PSCR=10;   //pclk=2^10/f_master=64us
    u16 tm=(1024)/16;
    u16 load=(1000ul)/tm;     // 1ms
    TIM2_ARRH=(load>>8)&0xff;
    TIM2_ARRL=load&0xff;
    TIM2_IER_UIE=1;    
    TIM2_CR1_CEN=1;     //ENABLE
}
//static void Timer4_init()
//{
//    TIM4_PSCR=7;    //8us
//    TIM4_ARR=125;
//    TIM4_IER_UIE=1;
//    TIM4_CR1_CEN=1;
//}
