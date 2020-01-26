/*
 * main.c
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */
#include "board.h"
#include "serial.h"
#include "hlw8012.h"
#include "cli.h"
#include "swuart.h"

static u8 task_flags=0;
#define TF_UART_RX 0x01
#define TF_POWER_UPDATE 0x02
#define TF_LIGHT_UPDATE 0x04
#define TF_TEMP_UPDATE1 0x08
#define TF_TEMP_UPDATE2 0x10
#define TF_POWER_UPDATE_E 0x20
#define TF_POWER_UPDATE_TIME 0x40

u32 ms_counter = 0;
u32 last_mscnt = 0;

static void counter_update();

#ifdef PULSE_GEN
#define PULSE_INIT()    {PA_DDR|=0x02;PA_CR1|=0x02;}
#define PULSE_TOGGLE()  {(PA_ODR&0x02)?(PA_ODR&=~0x02):(PA_ODR|=0x02);}

#endif

void main()
{
    u8 buff[CONFIG_APP_BUFF_SIZE];
    int len;
	//int disable
	sys_int_disable();

	//app init
	(void)Board_Init();

#ifdef PULSE_GEN
    PULSE_INIT();
#endif
    
	//interrupt &watchdog
#if CONFIG_WATCHDOG
	sys_iwdg_start();
#endif
	sys_int_enable();
 
	while(DEF_YES)
	{       
        if(BIT_TEST(task_flags,TF_UART_RX))
        {
            if((len=serial_receive(buff,CONFIG_APP_BUFF_SIZE))>0)
            {
                command_resove(buff,len);
                if((len=command_main(buff))>0)
                {
                    serial_send(buff,len);
                }
            }

#ifdef SWUART
            if((len=uart_recive(buff))>0)
            {
#ifdef CLI
                buff[len]='\0';
                cli_exec(buff);
#else
                command_resove(buff,len);
                if((len=command_main(buff))>0)
                {
                    uart_sendstr(buff,len);
                }
#endif
            }
#endif
            BIT_CLR(task_flags,TF_UART_RX);
        }
        if(BIT_TEST(task_flags,TF_POWER_UPDATE))
        {   
        	hlw8012_update_p();
            BIT_CLR(task_flags,TF_POWER_UPDATE);
        }
        if(BIT_TEST(task_flags,TF_LIGHT_UPDATE))    //light
        {
            light_start();
            BIT_CLR(task_flags,TF_LIGHT_UPDATE);
        }
        if(BIT_TEST(task_flags,TF_TEMP_UPDATE1))
        {
            Board_UpdateTempreture(BOARD_TEMP_ON_MB);
            BIT_CLR(task_flags,TF_TEMP_UPDATE1);
        }
        if(BIT_TEST(task_flags,TF_TEMP_UPDATE2))
        {
            Board_UpdateTempreture(BOARD_TEMP_EXTERN_MB);
            BIT_CLR(task_flags,TF_TEMP_UPDATE2);
        }
        if(BIT_TEST(task_flags,TF_POWER_UPDATE_E))
        {
            hlw8012_update_e();
            BIT_CLR(task_flags,TF_POWER_UPDATE_E);
        }
        if(BIT_TEST(task_flags,TF_POWER_UPDATE_TIME))
        {
            hlw8012_update_pvc();
            BIT_CLR(task_flags,TF_POWER_UPDATE_TIME);
        }
        
#if CONFIG_WATCHDOG
		sys_iwdg_update();
#endif
	}

}
void counter_update()
{
	ms_counter++;

	if(ms_counter%10 == 0){

		if(ms_counter == 0xFFFFE380 )
		{
			ms_counter = 0;
			last_mscnt = 0xFFFFFFFF;
		}
		
	    if((ms_counter%CONFIG_TIME_UPDATE_POWER)==0)
	    {
	        hlw8012_measure_p();	        
	        BIT_SET(task_flags,TF_POWER_UPDATE);
	    }
	    if((ms_counter%CONFIG_TIME_UPDATE_POWER_E)==0)
	    {
	        BIT_SET(task_flags,TF_POWER_UPDATE_E);
	    }
	    if((ms_counter%CONFIG_TIME_UPDATE)==0)
	    {        
	        hlw8012_measure_v();
	        BIT_SET(task_flags,TF_POWER_UPDATE_TIME);
	    }
	    if((ms_counter%CONFIG_TIME_UPDATE_LIGHT)==0)
	    {
	        BIT_SET(task_flags,TF_LIGHT_UPDATE);
	    }  
	    if((ms_counter%CONFIG_TIME_UPDATE_UART_RX)==0)
	    {
	        BIT_SET(task_flags,TF_UART_RX);
	    }
	    if((ms_counter%CONFIG_TIME_UPDATE_TEMP1)==0)
	    {
	        BIT_SET(task_flags,TF_TEMP_UPDATE1);
	    }
	    if((ms_counter%CONFIG_TIME_UPDATE_TEMP2)==0)
	    {
	        BIT_SET(task_flags,TF_TEMP_UPDATE2);
	    }
	    serial_callback();
	}
}

#pragma vector=TIM2_OVR_UIF_vector
__interrupt void TIM2_OVR_UIF_Handler()
{
    if(TIM2_SR1_UIF)    // 1ms update period
    {
    	TIM2_SR1_UIF=0;
        counter_update();
    }
}

#pragma vector=TIM1_OVR_UIF_vector
__interrupt void TIM1_OVR_UIF_Handler()
{
#ifdef PULSE_GEN
    static u16 cnt = 0;
    cnt++;
#endif
    if(TIM1_SR1&0x01)
    {
#ifdef PULSE_GEN
        if((cnt%2)==0){
            PULSE_TOGGLE();
            cnt=0;
        }
#endif
        TIM1_SR1_UIF=0;      
        uart_timer();
#ifdef SWUART_TRANSMIT_USED
        uart_Tx_timing();
#endif

    }
}

#pragma vector=TIM1_CAPCOM_CC4IF_vector
__interrupt void TIM1_CAPCOM_CC4IF_Handler()
{
    if(TIM1_SR1&0x10)
    {
        TIM1_SR1&=~0x10;
#ifdef SWUART_RECEIVE_USED
        uart_Rx_timing2();
#endif
    }
}
