#ifndef __SERVER_IF_STUB_H__
#define __SERVER_IF_STUB_H__

typedef char *(*cmdFunctionPtr)(void *, ...);

struct _cmd_table_s
{
	char cmd[50];
	cmdFunctionPtr func;
};

typedef struct _cmd_table_s cmd_mapping_tables;
typedef struct _cmd_table_s *pcmd_mapping_tables;


char *capi_mainLoop(char *data, int size);

#endif /* __SERVER_IF_STUB_H__*/

