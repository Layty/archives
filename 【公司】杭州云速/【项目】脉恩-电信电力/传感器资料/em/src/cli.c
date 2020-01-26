#include "cli.h"
#include "string.h"

/*
cli命令行界面，一共可以注册MAX_CLI_COMMANDS 个命令，
命令名字可以是[A-Z,a-z,0-9]的某个字母。
运行效果：
CMD> F - Flash utilities
  f - Write factory mac address
  n - Write the serial number
*/

#if CFG_CLI_ENABLED
#define MAX_CLI_COMMANDS        (26+26+10)  /* Lower + upper + digit */

/* Commands forward */
void cli_cmd_list_commands(CLI_CMD_S * para,CLI_CMD_OP op);
void cli_cmd_command_help(CLI_CMD_S * para,CLI_CMD_OP op);
bool cli_parse_cmd(u8 *cmdstr, CLI_CMD_S * para);


/* 
 * Command list 
 *
 * Note: We use STATIC initializer because commands could be added during
 *       system initialization phase.
 */
static CLI_CMD_FUNC cmds[MAX_CLI_COMMANDS]; //命令列表

/*查找所有已添加命令并执行命令描述*/
void cli_cmd_list_commands(CLI_CMD_S * para,CLI_CMD_OP op)
{	
    if (op == CLI_CMD_OP_HELP) {
        cli_printf("\n\rHelp - List all available commands\n\r");
    } else if (op == CLI_CMD_OP_DESC) {
        cli_printf("\n\rList all commands\n\r");
    } else {
        u8 i;
        CLI_CMD_FUNC *pcmd = &cmds[0];
        for(i=0; i<MAX_CLI_COMMANDS; i++, pcmd++) {
            if (*pcmd) {
                char c = i < 26 ?  ('a' + i) : (i < 52? ('A' + i - 26) : '0' + i - 52);
                cli_printf(&c);
                cli_printf(" - ");
                (*(*pcmd))(para, CLI_CMD_OP_DESC);
                cli_printf("\n\r");
            }
        }
    }
}

/*查找所有已添加命令并执行命令帮助*/
void cli_cmd_command_help (CLI_CMD_S * para,CLI_CMD_OP op)
{
    if (op == CLI_CMD_OP_HELP) {
        cli_printf("\n\rHelp - Show help information for a command\n");
    } else if (op == CLI_CMD_OP_DESC) {
        cli_printf(strInvalueCmd);
    } else {
        char cmd;
        CLI_CMD_FUNC *pcmd;
		
        cmd = para->para;
        cli_printf("\n");
        if (cmd >= 'a' && cmd <= 'z') {
            pcmd = &cmds[cmd - 'a'];
        } else if (cmd >= 'A' && cmd <= 'Z') {
            pcmd = &cmds[26 + cmd - 'A'];
        } else if (cmd >= '0' && cmd <= '9') {
            pcmd = &cmds[52 + cmd - '0'];
        } else if (cmd == '\r' || cmd == '\n') {
            return;
        } else {
            cli_printf(strInvalueCmd);
            return;
        }
        
        if (*pcmd == NULL) {
            cli_printf(strInvalueCmd);
            return;
        }

        cli_printf("\n\r");
        (*(*pcmd))(para, CLI_CMD_OP_HELP);
    }
}

/*当前仅支持不超过一个参数*/
bool cli_parse_cmd(u8 *cmdstr, CLI_CMD_S *para)
{
	u8 i = 0,j = 0;
	bool iRet = false;

	memset((u8*)para, 0, sizeof(CLI_CMD_S));

	/*get cmd*/
	for(i = 0; cmdstr[i]; i++){
		if(cmdstr[i] == ' '){
			continue;
		}else if((cmdstr[i]>='a'&&cmdstr[i]<='z') || \
			    (cmdstr[i]>='A'&&cmdstr[i]<='Z') || \
			    (cmdstr[i]>='0'&&cmdstr[i]<='9')){
			para->cmd = cmdstr[i];
			iRet = true;
			break;
		}else{
			return false;
		}		
	}

	/*get para*/
	for(i++; cmdstr[i]; i++){
		if(cmdstr[i] == ' '){
			continue;
		}else if(cmdstr[i] == '-'){
			i++;
			if((cmdstr[i]>='a'&&cmdstr[i]<='z') || \
				(cmdstr[i]>='A'&&cmdstr[i]<='Z') || \
				(cmdstr[i]>='0'&&cmdstr[i]<='9')){
				para->para = cmdstr[i];
				break;
			}else{
				return false;
			}
		}else{
			return false;
		}		
	}

	/*space*/
	for(i++; cmdstr[i]; i++){
		if(cmdstr[i] == ' '){
			continue;
		}else{
			break;	
		}
	}

	/*get value*/
	while(cmdstr[i]){
		para->value[j] = cmdstr[i];
		i++;
		j++;
	}

	return iRet;	
}

void cli_exec(u8 *cmdstr) 
{
    CLI_CMD_S  para;
    CLI_CMD_FUNC *pcmd = NULL;

    if(!cli_parse_cmd(cmdstr, &para)){
    	cli_printf(strInvalueCmd);
    }else{
	    if (para.cmd >= 'a' && para.cmd <= 'z') {
	        pcmd = &cmds[para.cmd - 'a'];
	    } else if (para.cmd >= 'A' && para.cmd <= 'Z') {
	        pcmd = &cmds[26 + para.cmd - 'A'];
	    } else if (para.cmd >= '0' && para.cmd <= '9') {
	        pcmd = &cmds[52 + para.cmd - '0'];
	    }
		/*else if (para.cmd == UI_KB_LF || para.cmd == UI_KB_ESC ||
	               para.cmd == UI_KB_BS || para.cmd == UI_KB_CR) {
	        sal_putchar('\n');
	    } 
	    */
	    else {
	        cli_printf(strInvalueCmd);
	    }
	    
	    if (*pcmd == NULL) {
	        cli_printf(strInvalueCmd);
	    }
		else{
			/*执行命令*/
		    (*(*pcmd))(&para, CLI_CMD_OP_EXEC);
		}
    }
	cli_printf(strMYE);
}


/*添加注册命令*/
bool cli_add_cmd(char cmd, CLI_CMD_FUNC func)
{
    CLI_CMD_FUNC *pcmd = NULL;
    
    SAL_ASSERT(func);
    if (func == NULL) {
        return false;
    }
    
    if (cmd >= 'a' && cmd <= 'z') {
        pcmd = &cmds[cmd - 'a'];
    } else if (cmd >= 'A' && cmd <= 'Z') {
        pcmd = &cmds[26 + cmd - 'A'];
    } else if (cmd >= '0' && cmd <= '9') {
        pcmd = &cmds[52 + cmd - '0'];
    } else {
        SAL_ASSERT(FALSE);
        return false;
    }
    
    SAL_ASSERT(*pcmd == NULL);
    if (*pcmd != NULL) {
        return false;
    }
    
    *pcmd = func;
    return true;
}

/*移除注册命令*/
void cli_remove_cmd(char cmd)
{
    CLI_CMD_FUNC *pcmd = NULL;
    
    if (cmd >= 'a' && cmd <= 'z') {
        pcmd = &cmds[cmd - 'a'];
    } else if (cmd >= 'A' && cmd <= 'Z') {
        pcmd = &cmds[26 + cmd - 'A'];
    } else if (cmd >= '0' && cmd <= '9') {
        pcmd = &cmds[52 + cmd - '0'];
    } else {
        SAL_ASSERT(false);
        return;
    }
    
    *pcmd = NULL;
}
/*CLI初始化*/
void cli_init(void)
{
    u8 i;
    for(i=0; i<MAX_CLI_COMMANDS; i++) {
        cmds[i] = NULL;
    }
   	/*注册2个默认命令*/
//	cli_add_cmd('h', cli_cmd_list_commands);
//    cli_add_cmd('H', cli_cmd_command_help);
//	cli_printf(strMYE);
}

#endif /* CFG_CLI_ENABLED */

