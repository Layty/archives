/*******************************************************************************
*    Filename: data.c
*    Created on: 2015/10/30
*    Author: draus
*    Email: drausxia@myenergydomain.cn
*******************************************************************************/

#include "include.h"
#include "hlw8012.h"
#include <string.h>
u8 data_get_checksum(u8 *src,u8 len);

u32 data_hlw_energy_partA1 @CONFIG_ADDR_HLW_ENERGY_PART_A1;
u32 data_hlw_energy_partA2 @CONFIG_ADDR_HLW_ENERGY_PART_A2;
u32 data_hlw_energy_partA3;

energy_factor_t data_energy_factor1 @CONFIG_ADDR_POWER_FACTOR1;
energy_factor_t data_energy_factor2 @CONFIG_ADDR_POWER_FACTOR2;


u8 data_hlw_energy_partB[CONFIG_MEM_EEPROM_PAGE_SIZE] @CONFIG_ADDR_HLW_ENERGY_PART_B;
u8 password[CONFIG_MEM_EEPROM_PAGE_SIZE] @CONFIG_ADDR_PASSWORD;   //√‹¬Î

system_info_t data_sys_info1 @CONFIG_ADDR_SYSTEM_INFO1;  //SYSTEM INFO 1
system_info_t data_sys_info2 @CONFIG_ADDR_SYSTEM_INFO2;  //SYSTEM INFO 2

u32 data_reset_count @CONFIG_ADDR_DEBUG;


static bool data_check_energy()
{
    //data check repair
    if(data_hlw_energy_partA1!=data_hlw_energy_partA2)
    {
        if(data_hlw_energy_partA1==data_hlw_energy_partA3)
        {
            mem_eeprom_unlock();
            data_hlw_energy_partA2=data_hlw_energy_partA3;
            mem_eeprom_lock();
        }else if(data_hlw_energy_partA2==data_hlw_energy_partA3)
        {
            mem_eeprom_unlock();
            data_hlw_energy_partA1=data_hlw_energy_partA3;
            mem_eeprom_lock();
        }else
        {
            mem_eeprom_unlock();
            data_hlw_energy_partA2=data_hlw_energy_partA1;
            mem_eeprom_lock();
            data_hlw_energy_partA3=data_hlw_energy_partA1;
        }
    }else
    {
        if(data_hlw_energy_partA1!=data_hlw_energy_partA3)
        {
            data_hlw_energy_partA3=data_hlw_energy_partA1;
        }
    }
    if(data_hlw_energy_partA1==data_hlw_energy_partA3&&data_hlw_energy_partA2==data_hlw_energy_partA3)
    {
        return true;
    }
    return false;
}
void data_save_energy(u8 add,int retry)
{
    bool flag=false;
    int i=0;
    volatile u8 partB=0;
    volatile u8 value;
    for(;i<CONFIG_MEM_EEPROM_PAGE_SIZE;i++)
    {
        partB=data_hlw_energy_partB[i];
        value=partB+add;
        if(partB!=0xFF)
        {
            u8 index=(value>>4)&0x0F;
            mem_eeprom_unlock();
            if(value==0xff)
            {
                flag=true;
                data_hlw_energy_partB[index]=0;
            }
            else
            {
                data_hlw_energy_partB[index]=value;
            }
            if(index!=i)
            {
                data_hlw_energy_partB[i]=0xFF;
            }
            mem_eeprom_lock();
            break;
        }
    }
    if(i>=CONFIG_MEM_EEPROM_PAGE_SIZE)
    {
        data_hlw_energy_partB[0]=add;
        if(add==0xFF)
        {
            flag=true;
        }
    }
    if(flag)
    {
        for(;retry>0;retry--)
        {
            if(data_check_energy())
            {
                break;
            }
        }    
        sys_int_disable();
        data_hlw_energy_partA3+=1;
        mem_eeprom_unlock();
        data_hlw_energy_partA1=data_hlw_energy_partA3;
        data_hlw_energy_partA2=data_hlw_energy_partA3;
        mem_eeprom_lock();
        sys_int_enable();
    }
}
void data_set_energy(u32 *value)
{
    mem_eeprom_unlock();
    data_hlw_energy_partA1=(*value)>>8;
    data_hlw_energy_partA2=(*value)>>8;
    mem_eeprom_lock();
    for(int i=0;i<CONFIG_MEM_EEPROM_PAGE_SIZE;i++)
    {
        if(data_hlw_energy_partB[i]!=0xFF)
        {
           mem_eeprom_unlock();
           data_hlw_energy_partB[i]=(*value)&0xff;
           mem_eeprom_lock();
           break;
        }
    }    
}
u32 data_get_energy(int retry)
{
    u32 ret=0;
    for(int i=0;i<CONFIG_MEM_EEPROM_PAGE_SIZE;i++)
    {
        if(data_hlw_energy_partB[i]!=0xFF)
        {
           ret|=data_hlw_energy_partB[i];
           break;
        }
    }
    for(;retry>0;retry--)
    {
        if(data_check_energy())
        {
            break;
        }
    }
    ret|=data_hlw_energy_partA3<<8;
    return ret;    
}
void data_get_system_info(system_info_t *info)
{
    int len=sizeof(data_sys_info1)-1;
    if(data_get_checksum((u8 *)&data_sys_info1,len)!=data_sys_info1.checksum)
    {
        if(data_get_checksum((u8 *)&data_sys_info2,len)==data_sys_info2.checksum)
        {
            mem_eeprom_unlock();
            memcpy((void *)&data_sys_info1,(const void *)&data_sys_info2,len+1);
            mem_eeprom_lock();
        }
        else
        {
            info->version[0]='V';
            info->id[0]='0';
            info->checksum=data_get_checksum((u8 *)info,len-3);
            mem_eeprom_unlock();
            memcpy((void *)&data_sys_info1,(const void *)info,len+1);
            memcpy((void *)&data_sys_info2,(const void *)info,len+1);
            mem_eeprom_lock();
        }
    }
    else
    {
        if(data_get_checksum((u8 *)&data_sys_info2,len)!=data_sys_info2.checksum)
        {
            mem_eeprom_unlock();
            memcpy((void *)&data_sys_info2,(const void *)&data_sys_info1,len+1);
            mem_eeprom_lock();
        }
    }
}
bool data_set_energy_factor(energy_factor_t *factor,u8 retry)
{
    int len=sizeof(energy_factor_t);
    factor->checksum=data_get_checksum((u8 *)factor,len-1);
    for(;retry>0;retry--)
    {
        mem_eeprom_unlock();
        memcpy((void *)&data_energy_factor1,(const void *)factor,len);
        memcpy((void *)&data_energy_factor2,(const void *)factor,len);
        mem_eeprom_lock();
        if(!memcmp(&data_energy_factor2,&data_energy_factor1,len))
        {
            return true;
        }
    }
    hlw8012_init(); //update power limits
    return false;
}
void data_get_energy_factor(energy_factor_t *factor)
{
	if(data_energy_factor1.power >= CONFIG_HLW8012_FACOTR_MIN)
	{
    	factor->power=data_energy_factor1.power;
	}
	
	if(data_energy_factor1.votage >= CONFIG_HLW8012_FACOTR_MIN)
	{
		factor->votage=data_energy_factor1.votage;
	}

	if(data_energy_factor1.current >= CONFIG_HLW8012_FACOTR_MIN)
	{
    	factor->current=data_energy_factor1.current;
	}

	return;
}
#if defined(COMMMAND_FUNCTION_ID)
bool data_set_system_info(system_info_t *info,u8 retry)
{
    int len=sizeof(system_info_t);
    info->checksum=data_get_checksum((u8 *)info,len-1);
    for(;retry>0;retry--)
    {
        mem_eeprom_unlock();
        memcpy((void *)&data_sys_info1,(const void *)info,len);
        memcpy((void *)&data_sys_info2,(const void *)info,len);
        mem_eeprom_lock();
        if(memcmp(&data_sys_info1,&data_sys_info1,len))
        {
            return true;
        }
    }
    return false;
}
#endif
void data_add_rst_count()
{
    mem_eeprom_unlock();
    data_reset_count++;
    mem_eeprom_lock();
}
u8 data_get_checksum(u8 *src,u8 len)
{
    u8 ret=0;
    while(len-->0)
    {
        ret+=*src++;
    }
    return ret;
}


