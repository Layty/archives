/*******************************************************************************
*    Filename: cmd.c
*    Created on: 2015/10/30
*    Author: draus
*    Email: drausxia@myenergydomain.cn
*******************************************************************************/

#include "cmd.h"
#include "include.h"
#include <string.h>
#include "hlw8012.h"

static u8 cmd_id0,cmd_id1,cmd_type;
static u8 COMMAND_FLAGS;

const char* strInvalueCmd="Fault cmd";
const char* strMYE="<MYE>";
const char* strNewLine="\n";

static void command_cmd_set(u8 id1,u8 id0,u8 type)
{
    cmd_id1=id1;
    cmd_id0=id0;
    cmd_type=type;
}
static u8 command_package_gen(u8 *buff,u8 len,u8 *dest)
{
    dest[0]=COMMAND_PACKAGE_PREFIX1;
    dest[1]=cmd_type;
    dest[2]=len+2;
    dest[3]=cmd_id1;
    dest[4]=cmd_id0;
    memcpy(&dest[5],buff,len);
    u8 ret=dest[2]+3;
    dest[ret]=data_get_checksum(dest,ret);
    dest[ret+1]=COMMAND_PACKAGE_SUFIX1;
    dest[ret+2]=COMMAND_PACKAGE_SUFIX2;
    dest[ret+3]=COMMAND_PACKAGE_SUFIX3;
    dest[ret+4]=COMMAND_PACKAGE_SUFIX4;
    return ret+5;
}
void command_resove(u8 *package,u8 len)
{
    energy_factor_t fac;
	u32 temp_value;

//    u8 *pac=memchr(package,COMMAND_PACKAGE_PREFIX1,len);       //查找数据包起始标志
    u8 *pac=package;
    if(pac==NULL)       //未查找到数据包起始标志
    {
        return;
    }
    len=pac[2]+3;
    if(data_get_checksum(pac,len)!=pac[len])
    {
        return;
    }
#if PULSE
    if(system_status1.step==SYSTEM_STATUS_STEP_INIT)
    {
        system_status1.step=SYSTEM_STATUS_STEP_MAIN;
        system_status2.step=SYSTEM_STATUS_STEP_MAIN;
    }
    if(system_status1.com==SYSTEM_STATUS_COM_PULSE)
    {
        return;
    }
#endif
    u16 func=*((u16 *)&pac[3]);
    if(pac[1]==COMMAND_PACKAGE_RW) //读写模式
    {   
        command_cmd_set(pac[3],pac[4],COMMAND_PACKAGE_RW);
        
        switch(func)
        {
        case COMMAND_PACKAGE_FUNCTION_EPUI:
            COMMAND_FLAGS=COMMAND_FLAGS_EPUI;
            break;
        case COMMAND_PACKAGE_FUNCTION_TM:
            COMMAND_FLAGS=COMMAND_FLAGS_TM;
            break;
#if defined(COMMAND_FUNCTION_EPUI_PULSE)
        case COMMAND_PACKAGE_FUNCTION_EPUI_PULSE:
            COMMAND_FLAGS=COMMAND_FLAGS_EPUI_PULSE;
            break;
#endif
#if defined(COMMAND_FUNCTION_PEYMDHM)           
        case COMMAND_PACKAGE_FUNCTION_PEYMDHM:
            COMMAND_FLAGS=COMMAND_FLAGS_PEYMDHM;
            break;
#endif
        case COMMAND_PACKAGE_FUNCTION_VERSION:
            COMMAND_FLAGS=COMMAND_FLAGS_VERSION;
            break;
#if defined(COMMAND_FUNCTION_ID)
        case COMMAND_PACKAGE_FUNCTION_ID:
            COMMAND_FLAGS=COMMAND_FLAGS_ID;
            break;
#endif
#if defined(COMMAND_FUNCTION_YDHMS)
        case COMMAND_PACKAGE_FUNCTION_YDHMS:
            COMMAND_FLAGS=COMMAND_FLAGS_YDHMS;
            break;
#endif
        case COMMAND_PACKAGE_FUNCTION_XYZ:
//            if(system_status1.unlock==1&&system_status2.unlock==1)
            {
                COMMAND_FLAGS=COMMAND_FLAGS_XYZ;
            }
//            else
//            {
//                COMMAND_FLAGS|=COMMAND_FLAGS_ERR;
//                return;
//            }
            break;
        default:
            break;
        }
    }else if(pac[1]==COMMAND_PACKAGE_SET)     //设置模式
    {
        command_cmd_set(pac[3],pac[4],COMMAND_PACKAGE_SET);
        COMMAND_FLAGS=0;
        switch(func)
        {
        case COMMAND_PACKAGE_FUNCTION_EPUI:
            
            sys_int_disable();
            data_set_energy((u32 *)(&pac[5]));
            sys_int_enable();
            hlw8012_rst();
            COMMAND_FLAGS=COMMAND_FLAGS_SET;
            break;
#if defined(COMMAND_FUNCTION_YDHMS)
        case COMMMAND_PACKAGE_FUNCTION_YDHMS:
            clock_set((datetime_t *)(&pac[5]));
            COMMAND_FLAGS=COMMAND_FLAGS_SET;
            break;
#endif
        case COMMAND_PACKAGE_FUNCTION_XYZ:
#if 0           //for test
            hlw_cnt_p=200;hlw_cnt_v=200;hlw_cnt_c=200;
#endif
//            if(system_status1.unlock==1&&system_status2.unlock==1&&system_status1.mode==SYSTEM_STATUS_MODE_FACTORY)
            {
                if(hlw_cnt_p>0){
					
                    temp_value=(((u32)(*((u16 *)(&pac[5]))))*1000)/hlw_cnt_p;
					fac.power=temp_value;
                }
                else
                {
                    COMMAND_FLAGS=COMMAND_FLAGS_ERR;
                    return;
                }
                if(hlw_cnt_v>0){
                    temp_value=(((u32)(*((u16 *)(&pac[7]))))*1000)/hlw_cnt_v;
					fac.votage=temp_value;
                }
                else{
                    COMMAND_FLAGS=COMMAND_FLAGS_ERR;
                    return;
                }
				#if 0
                if(hlw_cnt_c>0){
                    fac.current=(((u32)(*((u16 *)(&pac[9]))))*1000)/hlw_cnt_c;
                }
                else{
                    COMMAND_FLAGS=COMMAND_FLAGS_ERR;
                    return;
                }
				#endif
                sys_int_disable();
                data_set_energy_factor((energy_factor_t *)(&fac),CONFIG_MAX_RETRY_TIME);
                sys_int_enable();
				hlw8012_init();
                COMMAND_FLAGS=COMMAND_FLAGS_SET;
            }
            break;
#if defined(COMMAND_FUNCTION_SECURITY)
        case COMMAND_PACKAGE_FUNCTION_MODE:
            system_status1.mode=pac[5];
            system_status2.mode=pac[5];
            if(system_status1.mode==SYSTEM_STATUS_MODE_FACTORY&&system_status2.mode==SYSTEM_STATUS_MODE_FACTORY)
            {
                COMMAND_FLAGS=COMMAND_FLAGS_SET;
            }
            else
            {
                system_status1.mode=SYSTEM_STATUS_MODE_NORMAL;
                system_status2.mode=SYSTEM_STATUS_MODE_NORMAL;
                COMMAND_FLAGS=COMMAND_FLAGS_ERR;
                return;
            }
            break;
        case COMMAND_PACKAGE_FUNCTION_SECURITY:
            if(system_status1.mode==SYSTEM_STATUS_MODE_FACTORY&&system_status2.mode==SYSTEM_STATUS_MODE_FACTORY&&
               (!memcmp(&password[0],&pac[5],CONFIG_MEM_EEPROM_PAGE_SIZE)))    //密码验证正确            
            {                
                system_status1.unlock=1;
                system_status2.unlock=1;
                COMMAND_FLAGS=COMMAND_FLAGS_SET;
            }
            else
            {
                COMMAND_FLAGS=COMMAND_FLAGS_ERR;
                return;
            }
            break;
#endif
#if defined(COMMAND_FUNCTION_ID)
        case COMMAND_PACKAGE_FUNCTION_ID:
            data_set_system_info((system_info_t *)(&pac[5]),CONFIG_MAX_RETRY_TIME);
            break;
#endif
#if defined(COMMAND_FUNCTION_BROADCAST)
        case COMMAND_PACKAGE_FUNCTION_BROADCAST:
            COMMAND_FLAGS=COMMAND_FLAGS_SEND_BROADCAST;
            break;
#endif
        default:
            COMMAND_FLAGS=COMMAND_FLAGS_ERR;
            break;
        }
        if(COMMAND_FLAGS&COMMAND_FLAGS_ERR!=COMMAND_FLAGS_ERR)
        {
            COMMAND_FLAGS=COMMAND_FLAGS_SET;
        }
    }else
    {
        COMMAND_FLAGS=COMMAND_FLAGS_ERR;
    }
}
u8 command_main(u8 *data)
{
    u8 msg[16];
//    datetime_t dt;
    s16 tm_=temp1;
    u8 len=0;
    if(COMMAND_FLAGS==0)return 0;
//    else if(system_status1.step==SYSTEM_STATUS_STEP_SYNC&&system_status2.step==SYSTEM_STATUS_STEP_SYNC)
//    {
//        sys_int_disable();
//        system_status1.com=SYSTEM_STATUS_COM_UART;
//        system_status2.com=SYSTEM_STATUS_COM_UART;
//        system_status1.step=SYSTEM_STATUS_STEP_MAIN;
//        system_status2.step=SYSTEM_STATUS_STEP_MAIN;
//        sys_int_enable();
//    }
    switch(COMMAND_FLAGS)
    {
    case COMMAND_FLAGS_EPUI:
        sys_int_disable();
        *(u32 *)(&msg[0])=data_get_energy(CONFIG_MAX_RETRY_TIME);       
        *(u32 *)(&msg[4])=(u32)hlw_p;        
        *(u16 *)(&msg[8])=(u32)hlw_v;
        *(u16 *)(&msg[10])=(u32)hlw_c;
        len=command_package_gen(msg,12,data);
        sys_int_enable();
        break;
#ifdef COMMAND_EPUI_PULSE
    case COMMAND_FLAGS_EPUI_PULSE:
        sys_int_disable();
        *(u16 *)(&msg[0])=hlw_cnt_p;        
        *(u16 *)(&msg[2])=hlw_cnt_v;
        *(u16 *)(&msg[4])=hlw_cnt_c;
        len=command_package_gen(msg,6,data);
        sys_int_enable();
        break;
#endif
    case COMMAND_FLAGS_TM:
        sys_int_disable();
#if 1 //convert temp to real value ,and store in two bytes
        
        tm_=(s16)(tm_*6.25);
        if(tm_<0)
        {
            msg[2]=0x80;
            tm_=-tm_;
        }
        else
        {
            msg[2]=0;
        }
        msg[2]|=(tm_/100)&0x7f;
        msg[3]=(tm_%100)/10;//保留一位精度
#else
        *(u16 *)(&msg[2])=temp1;
#endif
        *(u16 *)(&msg[0])=light;        
#if 1 //convert temp to real value ,and store in two bytes
        tm_=temp2;
        tm_=(s16)(tm_*6.25);
        if(tm_<0)
        {
            msg[4]=0x80;
            tm_=-tm_;
        }
        else
        {
            msg[4]=0;
        }
        msg[4]|=(tm_/100)&0x7f;
        msg[5]=(tm_%100)/10;    //保留一位精度
#else
        *(u16 *)(&msg[4])=temp2;
#endif
        len=command_package_gen(msg,6,data); 
        break;
#if 0
    case COMMAND_FLAGS_PEYMDHM:
        dt=clock_get2();
        sys_int_disable();        
        *(u32 *)(&msg[0])=data_get_energy(CONFIG_MAX_RETRY_TIME);
        *(u32 *)(&msg[4])=hlw_p;       
        memcpy(&msg[8],(u8 *)(&dt),5);      
        len=command_package_gen(msg,13,data);
        sys_int_disable();
        break;
    case COMMAND_FLAGS_YDHMS:
        dt=clock_get2();
        sys_int_disable();
        memcpy(&msg[0],&dt,6);        
        len=command_package_gen(msg,6,data); 
        COMMAND_FLAGS|=COMMAND_FLAGS_YDHMS;
        break;
#endif
    case COMMAND_FLAGS_ID:
        sys_int_disable();
        extern system_info_t data_sys_info1;
        memcpy(&msg[0],&data_sys_info1.id[0],CONFIG_SYSTEM_INFO_ID_SIZE);        
        len=command_package_gen(msg,CONFIG_SYSTEM_INFO_ID_SIZE,data);
        break;
    case COMMAND_FLAGS_VERSION:
        sys_int_disable();
        extern system_info_t data_sys_info1;
        memcpy(&msg[0],&data_sys_info1.version[0],CONFIG_SYSTEM_INFO_VERSION_SIZE);        
        len=command_package_gen(msg,CONFIG_SYSTEM_INFO_VERSION_SIZE,data);
        break;
    case COMMAND_FLAGS_XYZ:
        sys_int_disable();
        data_get_energy_factor((energy_factor_t *)(&msg[0]));        
        len=command_package_gen(msg,6,data); 
        break;
    case COMMAND_FLAGS_SEND_BROADCAST:
    case COMMAND_FLAGS_SET:
        len=command_package_gen(0,0,data);
        break;
    case COMMAND_FLAGS_ERR:
        msg[0]=0xAA;
        msg[1]=0x55;
        msg[2]=0xAA;
        msg[3]=0x55;
        len=command_package_gen(msg,4,data);
        break;
    default:
        break;
    }
    sys_int_enable();
    COMMAND_FLAGS=0;
    return len;
}