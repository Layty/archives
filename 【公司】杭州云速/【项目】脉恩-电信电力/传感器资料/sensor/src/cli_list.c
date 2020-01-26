#include "cli.h"
#include "include.h"
#include "hlw8012.h"
static u8 cli_cmd_switchs=0; //bit 0 ir
                             //bit 1 zigbee

#define CLI_CMD_SWITCH_IR       0x01
#define CLI_CMD_SWITCH_ZIGBEE   0x02
#define CLI_CMD_SWITCH_ALL      0xff

static const char hex_tab[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
static const char cli_msg_ok[]="OK\n";
static const char cli_msg_fail[]="Fail\n";
static void cli_PrintfMsg(u8 index){
    switch(index){
    case 0:
        cli_printf(cli_msg_ok);
        break;
    case 1:
        cli_printf(cli_msg_fail);
        break;
    default:
        break;
    }
}
static int hex2string(char* pbuff,u8 hex)
{
    *pbuff++=hex_tab[hex>>4];
    *pbuff=hex_tab[hex&0x0f];
    return 2;
}
static int hex2string2(char* buff,u16 hex)
{
    (void)hex2string(buff,hex>>8);
    (void)hex2string(buff+2,hex&0xff);
    return 4;
}
static int hex2string4(char* buff,u32 hex)
{
    (void)hex2string2(buff,hex>>16);
    (void)hex2string2(buff+4,hex&(0xfffful));
    return 8;
}
static int string2dec(char *str)
{
    int ret=0;
    while(*str)
    {
        if(*str>='0'&&*str<='9')
        {
            ret*=10;
            ret+=(*str++-'0');
        }
        else
        {
            break;
        }
    }
    return ret;
}

static char* stringGetNextDec(char* str)
{
    while(*str)
    {
        if(*str++==' ')return str;
    }
    return NULL;
}

void cli_cmd_display_version(CLI_CMD_S * p,CLI_CMD_OP op)
{
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        cli_printf(__VER);
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:\n   v");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf("v - hardware version");
        break;
    default:
        break;
    }
}
void cli_cmd_display_status(CLI_CMD_S * p,CLI_CMD_OP op)
{
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        cli_printf("\nUnsupport");
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:\n   s");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf("s - system status");
        break;
    default:
        break;
    }
}
void cli_cmd_reset(CLI_CMD_S * p,CLI_CMD_OP op)
{
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        cli_printf("system reset\n");
            asm("LDW X,  SP ");
            asm("LD  A,  $FF");
            asm("LD  XL, A  ");
            asm("LDW SP, X  ");
            asm("JPF $8000");
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:\n   r");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf("r - reset mcu");
        break;
    default:
        break;
    }
}
void cli_cmd_switch(CLI_CMD_S * p,CLI_CMD_OP op)
{
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        if(p->para==0)
        {
            if(BIT_TEST(cli_cmd_switchs,CLI_CMD_SWITCH_IR))
                cli_printf("   ir = ON\n");
            else
                cli_printf("   ir = OFF\n");
            if(BIT_TEST(cli_cmd_switchs,CLI_CMD_SWITCH_ZIGBEE))
                cli_printf("   Zigbee = ON");
            else
                cli_printf("   Zigbee = OFF");  
        }else if(p->para=='i')
        {
            BIT_SET(cli_cmd_switchs,CLI_CMD_SWITCH_IR);
        }else if(p->para=='z')
        {
            BIT_SET(cli_cmd_switchs,CLI_CMD_SWITCH_ZIGBEE);
        }else if(p->para=='c')
        {
            BIT_CLR(cli_cmd_switchs,CLI_CMD_SWITCH_ALL);
        }
        else
        {
            cli_printf(strInvalueCmd);
        }
        break;
    case CLI_CMD_OP_HELP:
        cli_printf("Usage:\n   d : display swtich status\n   d -i : open ir switch\n   d -z : open zigbee switch\n   d -c : close all switch");
        break;
    case CLI_CMD_OP_DESC:
        cli_printf("display swtich status");
        break;
    default:
        break;
    }
}
static void cli_common_init()
{
    cli_add_cmd('v',cli_cmd_display_version);
    cli_add_cmd('s',cli_cmd_display_status);
    cli_add_cmd('r',cli_cmd_reset);
//    cli_add_cmd('d',cli_cmd_switch);
}
#ifdef CLI_CMD_STM8S
void cli_cmd_PowerManage(CLI_CMD_S * p,CLI_CMD_OP op)
{
    char _temp[10];
	u32 temp_value;
    int len;
    u32 e;
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        if(p!=NULL)
        {
            if(p->para=='s')
            {
                len=hex2string4(_temp,(u32)hlw_p);
                _temp[len]='\0';
                cli_printf("P=0x");
                cli_printf(_temp);

                len=hex2string4(_temp,(u32)hlw_v);
                _temp[len]='\0';
                cli_printf("\n\rV=0x");
                cli_printf(_temp);
                
                len=hex2string4(_temp,(u32)hlw_c);
                _temp[len]='\0';
                cli_printf("\n\rC=0x");
                cli_printf(_temp);
                e=data_get_energy(CONFIG_MAX_RETRY_TIME);
                len=hex2string4(_temp,e);
                _temp[len]='\0';
                cli_printf("\n\rE=0x");
                cli_printf(_temp);
            }
            else if(p->para=='z')
            {
                u32 v=0;
                extern void data_set_energy(u32 *value);
                data_set_energy(&v);
                cli_PrintfMsg(0);
            }
            else if(p->para=='c')
            {
                char *str=p->value;
                u32 vv=string2dec(str);
                energy_factor_t fac;
                fac.power=1000;
                fac.votage=1000;
                fac.current=1000;
                if(vv>0&&hlw_cnt_p>0)
                {
                    temp_value=(vv*1000)/hlw_cnt_p;
					fac.power = temp_value;
                }
                else
                {
                    cli_PrintfMsg(1);return;
                }
                str=stringGetNextDec(str);
                if(str!=NULL)
                {
                    vv=string2dec(str);
                    if(vv>0&&hlw_cnt_v>0)
                    {
                        temp_value=(vv*1000)/hlw_cnt_v;
						fac.votage = temp_value;
                    }
                    else
                    {
                        cli_PrintfMsg(1);return;
                    }
                }
				#if 0
                str=stringGetNextDec(str);
                if(str!=NULL)
                {
                    vv=string2dec(str);
                    if(vv>0&&hlw_cnt_c>0)
                    {
                        fac.current=((u32)(vv*1000))/hlw_cnt_c;
                    }
                    else
                    {
                        cli_PrintfMsg(1);return;
                    }
                }
				#endif

				sys_int_disable();
                data_set_energy_factor(&fac,3);
                sys_int_enable();
				hlw8012_init();
            }
            else
            {
                cli_printf(strInvalueCmd);
            }
        }
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:   r");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf(" - reset mcu");
        break;
    default:
        break;
    }
}
void cli_cmd_ReadLightValue(CLI_CMD_S * p,CLI_CMD_OP op)
{
    char _temp[16];
    int len;
    extern u16 light;
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        len=hex2string2(_temp,light);
        _temp[len]='\0';
        cli_printf("0x");
        cli_printf(_temp);
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:\n\r o");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf("o - Display light value");
        break;
    default:
        break;
    }
}
void cli_cmd_ReadRH_TEMP(CLI_CMD_S * p,CLI_CMD_OP op)
{
    char _temp[16];
    int len;
    cli_printf(strNewLine);
    switch(op)
    {
    case CLI_CMD_OP_EXEC:
        len=hex2string2(_temp,temp1);
        _temp[len]='\0';
        cli_printf("0x");
        cli_printf(_temp);
//        cli_printf(" (Vac=V*0.0625)");
 
        len=hex2string2(_temp,temp2);
        _temp[len]='\0';
        cli_printf("\n0x");
        cli_printf(_temp);
        break;
    case CLI_CMD_OP_HELP:
//        cli_printf("Usage:\n   r");
        break;
    case CLI_CMD_OP_DESC:
//        cli_printf("r - reset mcu");
        break;
    default:
        break;
    }
}



void cli_stm8s_init()
{
    cli_add_cmd('t',cli_cmd_ReadRH_TEMP);
    cli_add_cmd('p',cli_cmd_PowerManage);
    cli_add_cmd('o',cli_cmd_ReadLightValue);
}

#endif

#ifdef CLI_CMD_CC2530
void cli_cmd_update(CLI_CMD_S * p,CLI_CMD_OP op){}
void cli_cmd_ID(CLI_CMD_S * p,CLI_CMD_OP op){}
void cli_cmd_MAC(CLI_CMD_S * p,CLI_CMD_OP op){}
void cli_cmd_led(CLI_CMD_S * p,CLI_CMD_OP op){}
void cli_cmd_ir(CLI_CMD_S * p,CLI_CMD_OP op){}
void cli_cmd_Net(CLI_CMD_S * p,CLI_CMD_OP op){}

static void cli_cc2530_init()
{
    cli_add_cmd('u',cli_cmd_update);
    cli_add_cmd('n',cli_cmd_ID);
    cli_add_cmd('m',cli_cmd_MAC);
    cli_add_cmd('l',cli_cmd_led);
    cli_add_cmd('i',cli_cmd_ir);
    cli_add_cmd('z',cli_cmd_Net);
}

#endif

void cli_cmd_init()
{
    cli_common_init();
#ifdef CLI_CMD_STM8S
    cli_stm8s_init();
#endif
#ifdef CLI_CMD_CC2530
    cli_cc2530_init();
#endif
    
}

