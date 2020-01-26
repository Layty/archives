/*
 * config.h
 *
 *  Created on: 2015年12月10日
 *      Author: Admin
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "include.h"

#define CONFIG_DEBUG DEF_YES		//enable suart debug mode
#define CONFIG_PLATFORM_STM8S		//platform config

#define CONFIG_WATCHDOG DEF_YES

#define CONFIG_APP_BUFF_SIZE 32

#if 0
#define CONFIG_TIME_UPDATE_UART_RX 5    //100ms
#define CONFIG_TIME_UPDATE_POWER 25     //500ms
#define CONFIG_TIME_UPDATE_TEMP1 100    //2000ms
#define CONFIG_TIME_UPDATE_TEMP2 100    //2000ms
#define CONFIG_TIME_UPDATE_LIGHT 50     //1000ms    
#define CONFIG_TIME_UPDATE_POWER_E 500  //10s
#define CONFIG_TIME_UPDATE         50   //1s
#else
#define CONFIG_TIME_UPDATE_UART_RX 100    //100ms
#define CONFIG_TIME_UPDATE_POWER 100     //100ms
#define CONFIG_TIME_UPDATE_TEMP1 2000    //2000ms
#define CONFIG_TIME_UPDATE_TEMP2 2000    //2000ms
#define CONFIG_TIME_UPDATE_LIGHT 1000     //1000ms    
#define CONFIG_TIME_UPDATE_POWER_E 10000  //10s
#define CONFIG_TIME_UPDATE         5000  //5s
#endif

//for eeprom 
#define CONFIG_MAX_RETRY_TIME               3
#define CONFIG_ADDR_HLW_ENERGY_PART_A1  0x4000
#define CONFIG_ADDR_SYSTEM_INFO1        0x4010
#define CONFIG_ADDR_PASSWORD            0x4020
#define CONFIG_ADDR_POWER_FACTOR1        0x4030
#define CONFIG_ADDR_POWER_FACTOR2        0x4038
#define CONFIG_ADDR_HLW_ENERGY_PART_A2  0x4040
#define CONFIG_ADDR_SYSTEM_INFO2        0x4050
#define CONFIG_ADDR_HLW_ENERGY_PART_B   0x4060
#define CONFIG_ADDR_DEBUG               0x4070
#define CONFIG_SYSTEM_INFO_SIZE         0x0F
#define CONFIG_MEM_EEPROM_PAGE_SIZE     0x10
#define CONFIG_SYSTEM_INFO_ID_SIZE      0x0b    /*11字节ID*/
#define CONFIG_SYSTEM_INFO_VERSION_SIZE 0x04    /*5字节版本号*/

#endif /* INC_CONFIG_H_ */
