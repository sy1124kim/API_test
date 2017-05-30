#ifndef __CAPI_SERVER_H__
#define __CAPI_SERVER_H__

char * hello(char *);
typedef char * (*cmdFunctionPtr)(char *);

struct _cmd_table_s
{
	char cmd[50];
	cmdFunctionPtr func;
};

typedef struct _cmd_table_s cmd_mapping_tables;

cmd_mapping_tables cmd_mapping_table[] = 
{
	{"hello",  hello},
	{"end", NULL}
};

#endif /* __CAPI_SERVER_H__ */
