#ifndef __CLIENT_IF_STUB_H__
#define __CLIENT_IF_STUB_H__

typedef char *(*cmdFunctionPtr)(void *, ...);

struct _cmd_table_s
{
	char cmd[50];
	cmdFunctionPtr func;
};

typedef struct _cmd_table_s cmd_mapping_tables;
typedef struct _cmd_table_s *pcmd_mapping_tables;


int capi_hello(Hello *hello, Hello_Output *output);
int capi_hello_notify(callbackFuncPtr func, Hello *hello, Hello_Output *output);

int capi_notify_mainLoop(char *data, int size);

//void registerFuncTable();
//char *capi_hello3(callbackFuncPtr func, char *data, int count);

#endif /* __CLIENT_IF_STUB_H__*/

