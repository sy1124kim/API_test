#include <stdio.h>
#include <stdbool.h>
#include "client.h"

char * capi_hello(char *data, int count);
char *capi_hello3(callbackFuncPtr func, char *data, int count);
char capi_hello_notify(char * a);


client_mapping_tables client_mapping_table[] = 
{
	{"capi_hello",			  capi_hello, 			2, {e_type_string, e_type_int}},
	{"capi_hello3",  		  capi_hello3, 			3, {e_type_funcPtr,e_type_string, e_type_int}},
	{"capi_hello_notify",  	  capi_hello_notify, 	1, {e_type_string}}
};

void registerFuncTable()
{
	RegisterFunctionTables(client_mapping_table, sizeof(client_mapping_table)/sizeof(client_mapping_tables));
}


char * capi_hello(char *data, int count)
{
	char *ret_buf;

	ret_buf = SendMessage(__FUNCTION__, data, count);

	return ret_buf;
	
}

char *capi_hello3(callbackFuncPtr func, char *data, int count)
{
	char * ret_buf = NULL;
	
	ret_buf = SendMessageWithNotify(__FUNCTION__,capi_hello_notify, data, count);

	return ret_buf;
}

char capi_hello_notify(char * a)
{
	printf("client>> notify=%s \n", a);
}

