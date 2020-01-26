/*
 * ds18b20.c
 *
 *  Created on: 2015年12月10日
 *      Author: Admin
 */
#include "ds18b20.h"

#define DS18B20_CMD_SKIP_ROM            0xcc
#define DS18B20_CMD_START_CONVERT       0x44
#define DS18B20_CMD_READ_TM             0xbe
#define DS18B20_CMD_READ_SEARCH         0xF0

static u8 ds18b20_init(w1_t *w1)
{
    u8 ret=0;
    w1->init();         //初始化   
    
    w1->set_dir(1);
//    w1->set(1);
//    w1->delay(100);
    w1->set(0);
    w1->delay(1800);      //复位信号
    w1->set(1);
    w1->set_dir(0);
    w1->delay(100);  
//    while(w1->get());
//    {
//    }
    ret=w1->get();      //存在脉冲检测
    w1->set_dir(1);
    w1->set(1);
    w1->delay(240);             //
    return ret;
}
static void ds18b20_write_bit(w1_t *w1,u8 value)
{
    w1->set(0);
    w1->delay(2);
    if(value>0)
    {
        w1->set(1);
    }
    else
    {
        w1->set(0);
    }
    w1->delay(60);
    w1->set(1);
}
static u8 ds18b20_read_bit(w1_t *w1)
{
    u8 ret=0;
    w1->set_dir(1);
    w1->set(0);        
    w1->delay(2);
    w1->set_dir(0);
    w1->delay(10);
    if(w1->get()>0)
        ret|=0x80;   
    w1->delay(60);
    return ret;
}

static u8 ds18b20_read(w1_t *w1)
{
    sys_int_disable();
    u8 ret=0;
    for(u8 i=0;i<8;i++)
    {
        ret>>=1;
        if(ds18b20_read_bit(w1)>0)
        {
            ret|=0x80;
        }
    }
    sys_int_enable();
    return ret;
}
static void ds18b20_write(w1_t *w1,u8 dat)
{
    sys_int_disable();
    for(u8 i=0;i<8;i++)
    {
        ds18b20_write_bit(w1,dat&0x01);
        dat>>=1;
    }
    sys_int_enable();
}
#pragma optimize=none
s16 ds18b20_getTM(w1_t *w1)
{
    s16 ret=-255;
    if(ds18b20_init(w1))
        return ret;
    ds18b20_write(w1,DS18B20_CMD_SKIP_ROM);
    ds18b20_write(w1,DS18B20_CMD_START_CONVERT);    
//    for(int i=0;i<800;i++);
    if(ds18b20_init(w1))
        return ret;
    ds18b20_write(w1,DS18B20_CMD_SKIP_ROM);
    ds18b20_write(w1,DS18B20_CMD_READ_TM);
    ret=ds18b20_read(w1);
    ret|=((u16)ds18b20_read(w1))<<8;
    return ret;
}

