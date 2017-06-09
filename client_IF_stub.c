#include <stdio.h>
#include <stdbool.h>
#include "API_type.h"
#include "client.h"
#include "client_IF_stub.h"
#include "json-c.h"
#include "/usr/local/include/json-c/json.h"

int capi_hello(Hello *hello, Hello_Output *output);
int capi_hello_notify(callbackFuncPtr func, Hello *hello, Hello_Output *output);
int callback_capi_hello_notify(char *data);

cmd_mapping_tables cmd_mapping_table[] = 
{
	{"capi_hello",  capi_hello},
	{"capi_hello_notify",  capi_hello_notify},
	{"callback_capi_hello_notify", callback_capi_hello_notify},
	{"end",NULL}
};


char * makeHelloInput(char *funcName, Hello *hello)
{
	struct json_object *newObj, *helloObj;
	char *buf = NULL;
	char *ret_buf = NULL;

	newObj = json_object_new_object();
	helloObj = json_object_new_object();

	json_object_object_add(newObj, "func", json_object_new_string(funcName));
	json_object_object_add(helloObj, "data", json_object_new_string(hello->Name));
	json_object_object_add(helloObj, "count", json_object_new_int(hello->count));
	json_object_object_add(newObj, "param", helloObj);

	buf = json_object_to_json_string_ext(newObj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

	ret_buf = (char *)malloc(strlen(buf)+1);

	if(ret_buf != NULL)
	{
		memset(ret_buf, 0x00, strlen(buf)+1);
		memcpy(ret_buf, buf, strlen(buf));
	}

	json_object_put(newObj);

	return ret_buf;
}

Hello_Output *makeHelloOuput(char *data, Hello_Output *output)
{
	Hello_Output *output = NULL;
	
	printf("Client>> makeHelloOutput!!!\n");

	return output;
}

int capi_hello(Hello *hello, Hello_Output *output)
{
	char * data = NULL;
	int ret = 0;
	char *ret_buf = NULL;

	data = makeHelloInput(__FUNCTION__, hello);
	printf("Client>> SendMessage = %s\n", data);
	
	ret_buf = SendMessage(data, strlen(data));

	printf("Client>> ret_buf = %s\n", ret_buf);
	
	output = makeHelloOuput(data);

	return 0;
	
}

char * makeHello_notify_Input(callbackFuncPtr callbackfunc, callbackFuncPtr token, char *func, Hello *hello)
{	
	struct json_object *newObj, *helloObj;
	char *buf = NULL;
	char *ret_buf = NULL;
	char func_ptr[20];

	
	newObj = json_object_new_object();
	helloObj = json_object_new_object();

	json_object_object_add(newObj, "func", json_object_new_string(func));

	memset(func_ptr, 0x00, 20);
	sprintf(func_ptr, "%x", callbackfunc);
	json_object_object_add(helloObj, "callbackfunc", json_object_new_string(func_ptr));

	memset(func_ptr, 0x00, 20);
	sprintf(func_ptr, "%x", token);
	json_object_object_add(helloObj, "token", json_object_new_string(func_ptr));
	json_object_object_add(helloObj, "data", json_object_new_string(hello->Name));
	json_object_object_add(helloObj, "count", json_object_new_int(hello->count));
	json_object_object_add(newObj, "param", helloObj);

	buf = json_object_to_json_string_ext(newObj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

	ret_buf = (char *)malloc(strlen(buf)+1);

	if(ret_buf != NULL)
	{
		memset(ret_buf, 0x00, strlen(buf)+1);
		memcpy(ret_buf, buf, strlen(buf));
	}

	json_object_put(newObj);

	return ret_buf;
}

int callback_capi_hello_notify(char *data)
{
	printf("Client>> callback_capi_hello_notify !!!\n");
	
}

int makeHello_notify_Output()
{
	return 0;
}


int capi_hello_notify(callbackFuncPtr func, Hello *hello, Hello_Output *output)
{
	char *data = NULL;
	char *ret_buf = NULL;

	printf("Client >> callback_func=%x\n", callback_capi_hello_notify);
	printf("Client >> token =%x\n", func);
	data = makeHello_notify_Input(callback_capi_hello_notify, func, __FUNCTION__, hello);

	printf("Client>> buf = %s\n", data);

	ret_buf = SendMessage(data, strlen(data));

	printf("Client>> sendMessage ret_buf=%s\n", ret_buf);
	
}

int capi_notify_mainLoop(char *data, int size)
{
	char *callbackfunc = NULL;
	char funcptr[15];
	char *ret_buf;
	int i = 0;
	char *param = NULL;

	printf("\n\nClient>> capi_notify_mainLoop!!!\n");
	printf("\n\nClient>> data = %s\n", data);
	
	callbackfunc = json_getString(data, "func");
	printf("Client OK!!! func=%s\n", callbackfunc);
	//if(func == NULL)
	//	printf("Client>> callbackfunc null!!!\n");
	
	//printf("client>> calbackfunc = %s\n", func);
	
	param = json_getObject(data, "param");

	printf("Client OK!!! param=%s\n", param);
	
	while(1)
	{
		if(!strncmp(cmd_mapping_table[i].cmd, "end", strlen("end")))
		{
			break;
		}
		memset(funcptr, 0x00, 15);
		sprintf(funcptr, "%x", cmd_mapping_table[i].func);

		if(!strncmp(funcptr, callbackfunc, strlen(callbackfunc)))
		{
			ret_buf = cmd_mapping_table[i].func(param);
			break;
		}
		i++;
	}

}

