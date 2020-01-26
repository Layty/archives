/*
 * typedef.h
 *
 *  Created on: 2015��12��10��
 *      Author: Admin
 */

#ifndef INC_TYPEDEF_H_
#define INC_TYPEDEF_H_
#include "config.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;
typedef signed int s16;
typedef unsigned long u32;
typedef signed long s32;

#ifdef __IAR__
typedef _Bool bool;
#endif

#define SYSTEM_STATUS_STEP_INIT 0x11
#define SYSTEM_STATUS_STEP_CFG  0x22
#define SYSTEM_STATUS_STEP_SYNC 0x33
#define SYSTEM_STATUS_STEP_MAIN 0x44

#define SYSTEM_STATUS_COM_UART  0x2
#define SYSTEM_STATUS_COM_PULSE 0x5

#define SYSTEM_STATUS_MODE_FACTORY 0x3
#define SYSTEM_STATUS_MODE_NORMAL  0xF
#define SYSTEM_STATUS_MODE_DEBUG   0xc 
typedef struct
{
    u8 step; //״̬
    u8 com:3;  //�Ƿ��ڴ���ģʽ
    u8 unlock:1;
    u8 mode:4; //����ģʽ
}system_status_t;
typedef struct
{
    u8 version[CONFIG_SYSTEM_INFO_VERSION_SIZE];
    u8 id[CONFIG_SYSTEM_INFO_ID_SIZE];
    u8 checksum;   
}system_info_t;

#ifndef true
#define false 0
#define true (!false)
#endif

#ifndef NULL
#define NULL (0)
#endif

#endif /* INC_TYPEDEF_H_ */
