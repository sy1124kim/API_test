#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "server_IF_stub.h"

char * capi_hello(char *, int);
char * capi_hello3(cmdFunctionPtr ptr, char * data, int count);
char *notify_function(cmdFunctionPtr ptr, char *data);

cmd_mapping_tables cmd_mapping_table[] = 
{
	{"capi_hello",  capi_hello, 2, {e_type_string, e_type_int}},
	{"capi_hello3",  capi_hello3, 3, {e_type_funcPtr, e_type_string, e_type_int}},
	{"notify_function",notify_function, 2, {e_type_funcPtr, e_type_string}} 
};

void register_function()
{
	server_table_init(cmd_mapping_table, sizeof(cmd_mapping_table)/sizeof(cmd_mapping_tables));
}

char *notify_function(cmdFunctionPtr ptr, char *data)
{
	sleep(3);
	NotifySendMessage(__FUNCTION__,ptr, data);
}

char * capi_hello3(cmdFunctionPtr ptr, char * data, int count)
{		
	notify_function(ptr, "notify test OK!!!!");

	return "OK~~";
}

char * capi_hello(char *data, int count)
{
	char *ret_buf;
	char test_buf[1024]={0,};
	char *p_test_buf = NULL;
	int i = 0;

	p_test_buf = test_buf;
	for(i = 0; i<count; i++)
	{
		sprintf(p_test_buf, "%s ", data);
		p_test_buf += strlen(data)+1;
	}
	
	ret_buf = (char *)malloc((strlen(test_buf)));
	memset(ret_buf, 0x00, strlen(test_buf));
	memcpy(ret_buf, test_buf, strlen(test_buf) - 1);
	
	return ret_buf;

}

