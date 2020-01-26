/*******************************************************************************
*    Filename: hlw8012.h
*    Created on: 2015/10/30
*    Author: draus
*    Email: drausxia@myenergydomain.cn
*******************************************************************************/

#ifndef __HLW8012_H__
#define __HLW8012_H__
#include "include.h"

#define PULSE   DEF_NO


typedef struct
{
    u16 power;		//功率补偿参数
    u16 votage;		//电压补偿参数
    u16 current;	//电流补偿参数
    u8 reserved;
    u8 checksum;
}energy_factor_t;

#define HLW8012_CF          BIT(5)
#define HLW8012_CF1         BIT(4) 
#define HLW8012_VC_SEL      BIT(5)  //PB

#define hlw8012_lowlevel_init()     do{\
                                        PC_DDR&=~HLW8012_CF;\
                                        PB_DDR&=~HLW8012_CF1;\
                                        PB_DDR|=HLW8012_VC_SEL;\
                                        PB_CR1&=~HLW8012_VC_SEL;/*open-drain mode*/\
                                        PB_ODR|=HLW8012_VC_SEL;\
                                        PB_CR2|=HLW8012_CF1;\
                                        PC_CR2|=(HLW8012_CF);\
                                        EXTI_CR1=0;\
                                        EXTI_CR1|=0x3c;\
                                    }while(0)

#define hlw8012_measure_c_init()   do{PB_ODR&=~HLW8012_VC_SEL;}while(0)
#define hlw8012_measure_v_init()   do{PB_ODR|=HLW8012_VC_SEL;}while(0)

#define hlw8012_measure_vc_start() do{PB_CR2|=HLW8012_CF1;}while(0)
#define hlw8012_measure_vc_stop()  do{PB_CR2&=~HLW8012_CF1;}while(0)
                                        
#define hlw8012_measure_vc_get_status() ((PB_ODR&HLW8012_VC_SEL)>0)

#define CONFIG_HLW8012_FACTOR_P_DEFAULT 3172      //
#define CONFIG_HLW8012_FACTOR_V_DEFAULT 285       // 
#define CONFIG_HLW8012_FACTOR_C_DEFAULT 666       //
#define CONFIG_HLW8012_WEIGHT   30

#define CONFIG_HLW8012_P_MAX            5000
#define CONFIG_HLW8012_V_MAX            500
#define CONFIG_HLW8012_C_MAX            1600

#define CONFIG_HLW8012_P_MIN            2
#define CONFIG_HLW8012_V_MIN            10
#define CONFIG_HLW8012_C_MIN            5

#define CONFIG_EEPROM_DEFAULT_VALUE     0xff
#define CONFIG_HLW8012_FACOTR_MIN       50
#define CONFIG_HLW8012_FACOTR_DEFAULT   500
#define CONFIG_HLW8012_ENERGY_SAVE_MIN  2500
#define CONFIG_HLW8012_ENERGY_SAVE_DEFAULT  3600
#define CONFIG_HLW8012_CURRENT_PULSE_MIN    7
#define CONFIG_HLW8012_CURRENT_PULSE_DEFAULT    0
#define CONFIG_HLW8012_MIN_PULSE_WIDTH  400   //50ms
                   
extern energy_factor_t data_energy_factor1;
extern energy_factor_t data_energy_factor2;
                   
void hlw8012_init();
void hlw8012_measure_vc();
void hlw8012_measure_v();
void hlw8012_measure_p();
void hlw8012_update_pvc();
void hlw8012_update_p();
void hlw8012_update_e();
void hlw8012_rst();

bool data_set_energy_factor(energy_factor_t *factor,u8 retry);
void data_get_system_info(system_info_t *info);
u32 data_get_energy(int retry);
void data_set_energy(u32 *value);
void data_save_energy(u8 add,int retry);
static bool data_check_energy();
void data_get_energy_factor(energy_factor_t *factor);
u8 data_get_checksum(u8 *src,u8 len);                                 
bool data_set_system_info(system_info_t *info,u8 retry);                
                                        
#endif