/*
 * include.h
 *
 *  Created on: 2015年12月10日
 *      Author: Admin
 */

#ifndef INC_INCLUDE_H_
#define INC_INCLUDE_H_

#include "typedef.h"
#include "power.h"
#include "clock.h"
#include "version.h"

#include <iostm8s003f3.h>

//mallocs
#ifndef DEF_YES
#define DEF_NO 		(0)
#define DEF_YES 	(!DEF_NO)
#endif

#define BIT(_n) ((0x01u)<<(_n))
#define BIT_TEST(v,mask)    ((v)&(mask))
#define BIT_SET(v,mask)     {v|=mask;}
#define BIT_CLR(v,mask)     {v&=~mask;}

#define sys_int_enable()        do{asm("rim");}while(0)   //enable interrupt
#define sys_int_disable()       do{asm("sim");}while(0)   //disable interrupt
#define sys_clk_init() do{\
                        while(!CLK_ICKR_HSIRDY);    /*wait for HSI ready*/\
                        CLK_CKDIVR=0;   /*set Fmaster = 16MHz*/\
                    }while(0)
#define sys_iwdg_start() do{\
                            IWDG_KR=0xcc;   /*start iwdg*/\
                            IWDG_KR=0x55;   /*enable iwdg config*/\
                            IWDG_RLR=0xff;  /*set iwdg counter*/\
                            IWDG_PR=0x6;    /*256分频 F=128kHz/256=500Hz*/\
                            IWDG_KR=0xaa;   /*refresh*/\
                        }while(0)
#define sys_iwdg_update()    	do{IWDG_KR=0xaa;}while(0)          /*refresh watchdog*/
#define mem_flash_lock()        do{FLASH_IAPSR_PUL=0;}while(0)
#define mem_flash_unlock()      do{FLASH_PUKR=0x56;FLASH_PUKR=0xae;}while(!(FLASH_IAPSR&0x02))
#define mem_eeprom_lock()       do{FLASH_IAPSR_DUL=0;}while(0)
#define mem_eeprom_unlock()     do{FLASH_DUKR=0xae;FLASH_DUKR=0x56;}while(!(FLASH_IAPSR&0x08))

#define sys_disable_swim()      do{CPU_CFG_GCR_SWO=1;}while(0)  //disable iap
#define sys_set_AL_main()       do{CPU_CFG_GCR_AL=0;}while(0)
#define sys_set_AL_isr()        do{CPU_CFG_GCR_AL=1;}while(0)
//gloable vars
extern s16 temp1,temp2; //tempture on board
extern u16 light;       //light value                            
extern power_storage_t power_storage;
extern system_status_t system_status1,system_status2;
extern u8 password[];
extern float hlw_p,hlw_v,hlw_c;
extern u32 hlw_cnt_p,hlw_cnt_v,hlw_cnt_c;

void light_init();
void light_start();


void command_resove(u8 *package,u8 len);
u8 command_main(u8 *data);

#endif /* INC_INCLUDE_H_ */
