/*******************************************************************************
*    Filename: hlw8012.c
*    Created on: 2015/10/30
*    Author: draus
*    Email: drausxia@myenergydomain.cn
*******************************************************************************/

#include "hlw8012.h"

energy_factor_t energy_factor;

extern void data_save_energy(u8 add,int retry);
extern void data_get_energy_factor(energy_factor_t *factor);


static u32 cnt_e=0,cnt_p=0,cnt_v=0,cnt_c=0;
static u32 last_cnt_p = 0;
u32 hlw_cnt_p=0,hlw_cnt_v=0,hlw_cnt_c=0;
u8 measure_p_flag = 0; //0:计数；1:单周期测量
extern u32 last_mscnt;
u32 last_period = 0;
extern u32 ms_counter;

static u32 cnt_e_limit1=0;
//static u32 cnt_e_limit2=0;
void hlw8012_rst()
{
    sys_int_disable();
    cnt_e=0;
    sys_int_enable();
}

#if 0
void hlw8012_measure_vc()
{
    static u8 flag = 0;
    if(flag==0) //init v
    {
        flag=1;        
        hlw8012_measure_v_init();
        return;
    }
    if(flag==1)    //wait
    {
        flag=2;
        return;
    }
    if(flag==2)     //start
    {
        cnt_v=0;
        hlw8012_measure_vc_start();
        flag=3;
        return;
    }
    if(flag==3)    //stop and init c
    {
        hlw8012_measure_vc_stop();
        hlw_cnt_v=cnt_v;
        cnt_v=0;
        cnt_c=0;
        flag=4;
        hlw8012_measure_c_init();
        return;
    }
    if(flag==4)    //wait
    {
        flag=5;
        return;
    }
    if(flag==5)    //start
    {
        hlw8012_measure_vc_start();
        flag=6;
        return;
    }
    if(flag==6)    //stop 
    {
        hlw8012_measure_vc_stop();
        hlw_cnt_c=cnt_c;
        cnt_c=0;
        cnt_v=0;
        flag=0;
        return;
    }
}
#endif

void hlw8012_measure_v()
{
    hlw_cnt_v=cnt_v/5;
    cnt_v = 0;
	
	if( measure_p_flag )
	{
		hlw_cnt_p = cnt_p/5;
		last_cnt_p = cnt_p = 0;
	}
}

void hlw8012_measure_p()
{	
	sys_int_disable();
	u32 tempcnt = cnt_p-last_cnt_p;
	
    if( tempcnt >= 2 )
    {
    	measure_p_flag = 1;	
    }
	else if( tempcnt == 1 )
	{
		measure_p_flag = 0;
		if(last_period > 100)
		{
			hlw_cnt_p = last_period;
		}
	}
	else
	{
		measure_p_flag = 0;
		if(ms_counter-last_mscnt>last_period)
		{
			hlw_cnt_p = ms_counter-last_mscnt;
		}
	}	
	
	last_cnt_p = cnt_p;
	sys_int_enable();
}

void hlw8012_update_p()
{
	float temp = 0;
	
	if(!measure_p_flag)
	{
		sys_int_disable();
		temp = energy_factor.power/hlw_cnt_p;
		
		if( temp < 3500 )
		{
			hlw_p = temp;

			if(hlw_v > 0)
			{
				hlw_c = hlw_p*100/hlw_v;
			}
		}
		sys_int_enable();
	}
}

#if 0
static float hlw8012_fix(float a,float b,u8 weight)
{
    float ret=a*weight;
    ret+=b*(100-weight);
    return ret/100;
}
#endif

void hlw8012_update_pvc()
{
    float temp_p = 0;
	
    sys_int_disable();
	if( measure_p_flag )
	{
    	temp_p =((float)((u32)hlw_cnt_p))*energy_factor.power/1000ul;
		if( temp_p < 3500 )
		{
			hlw_p = temp_p;
		}
	}
    hlw_v=((float)((u32)hlw_cnt_v))*energy_factor.votage/1000ul;

	if(hlw_v>0)
	{
		hlw_c= hlw_p*100/hlw_v;
	}

    sys_int_enable();

#if PULSE   //pulse update
    extern system_status_t system_status1;
    if(system_status1.com==SYSTEM_STATUS_COM_PULSE)
    {
        if(hlw_p>5)
        {
            extern void pulse_UpdateLimitCF(u16 value);pulse_UpdateLimitCF(hlw_p);
            extern void pulse_start();pulse_start();
        }
        else
        {
            extern void pulse_stop();pulse_stop();
        }
    }
    
#endif
}
void hlw8012_init()
{
	energy_factor.power = CONFIG_HLW8012_FACTOR_P_DEFAULT;
	energy_factor.votage = CONFIG_HLW8012_FACTOR_V_DEFAULT;
	energy_factor.current = CONFIG_HLW8012_FACTOR_C_DEFAULT;
	
    data_get_energy_factor(&energy_factor);
    cnt_e_limit1=((u32)3600ul)*10000ul/energy_factor.power;
    if(cnt_e_limit1<CONFIG_HLW8012_ENERGY_SAVE_MIN)
        cnt_e_limit1=CONFIG_HLW8012_ENERGY_SAVE_DEFAULT;
    hlw8012_lowlevel_init();
}

void hlw8012_update_e()
{
    if(cnt_e>=cnt_e_limit1)
    {
        sys_int_disable();
        cnt_e-=cnt_e_limit1;
        data_save_energy(1,3);
        sys_int_enable();
    }
}

#pragma vector=EXTI2_vector
__interrupt void EXIT2_Handler()
{
	last_period = ms_counter-last_mscnt;
	last_mscnt = ms_counter;
    cnt_p++;
    cnt_e++;
}

//电流电压测量
#pragma vector=EXTI1_vector
__interrupt void EXIT1_Handler()
{
    cnt_v++;
    cnt_c++;
}