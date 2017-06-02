#ifndef __SERVER_H__
#define __SERVER_H__

typedef enum
{
	e_type_int = 0,
	e_type_string,
	e_type_double,
	e_type_funcPtr
}type;

typedef char *(*cmdFunctionPtr)(char *, ...);

struct _cmd_table_s
{
	char cmd[50];
	cmdFunctionPtr func;
	int arguCnt;
	int arguType[10];
};

typedef struct _cmd_table_s cmd_mapping_tables;
typedef struct _cmd_table_s *pcmd_mapping_tables;

int server_init(void);
int server_table_init(pcmd_mapping_tables ptables, int cnt);
pcmd_mapping_tables searchCmdTables(char *funcName);
int NotifySendMessage(char *funcName, cmdFunctionPtr ptr, ...);

#endif /* __SERVER_H__ */
