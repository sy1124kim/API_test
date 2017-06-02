#ifndef __CAPI_CLIENT_H__
#define __CAPI_CLIENT_H__

typedef enum
{
	e_type_int = 0,
	e_type_string,
	e_type_double,
	e_type_funcPtr
}client_type;

typedef char *(*callbackFuncPtr)(char *, ...);

struct _client_cmd_tables_s
{
	char cmd[50];
	callbackFuncPtr func;
	int arguCnt;
	int arguType[10];

};

typedef struct _client_cmd_tables_s client_mapping_tables;
typedef struct _client_cmd_tables_s *pclient_mapping_tables;


char *SendMessage(char *funcName, ...);
int Notify_server_init(void);
pclient_mapping_tables RegisterFunctionTables(pclient_mapping_tables tables, int count);
pclient_mapping_tables searchFunction(char *funcName);
char *SendMessageWithNotify(char *funcName, ... );


#endif
