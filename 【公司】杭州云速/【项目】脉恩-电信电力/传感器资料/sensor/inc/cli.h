#ifndef _CLI_H_
#define _CLI_H_

#include "include.h"
#include "swuart.h"
#include "serial.h"
#include <string.h>

#define CFG_CLI_ENABLED 1
#define CLI_VALUE_LEN 32

typedef enum{
	CLI_CMD_OP_HELP,
	CLI_CMD_OP_DESC,
	CLI_CMD_OP_EXEC
}CLI_CMD_OP;

typedef struct{
	char cmd;
	char para;
	char value[CLI_VALUE_LEN];
}CLI_CMD_S;

extern const char* strInvalueCmd;
extern const char* strMYE;
extern const char* strNewLine;



typedef void(*CLI_CMD_FUNC)(CLI_CMD_S *,CLI_CMD_OP);

#ifdef SWUART
#define cli_printf(x)     uart_sendstr((u8*)x,strlen(x))
#else
#define cli_printf(x) serial_send((u8*)x,strlen(x))
#endif

#define UI_KB_LF            0x0a
#define UI_KB_ESC           0x1b
#define UI_KB_BS            0x08
#define UI_KB_CR            0x0d

#define SAL_ASSERT(value)   {}
int cli_sprintf(char *buff,int len,const char *format,...);
void cli_init(void);
void cli_cmd_init();
void cli_exec(u8 *cmdstr);
bool cli_add_cmd(char cmd, CLI_CMD_FUNC func);
void cli_remove_cmd(char cmd);

#endif

