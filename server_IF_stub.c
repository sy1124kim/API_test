#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "API_type.h"
#include "server_IF_stub.h"
#include "json-c.h"
#include "/usr/local/include/json-c/json.h"

char * capi_hello(char *);
char *capi_hello_notify(char *data);

cmd_mapping_tables cmd_mapping_table[] = 
{
	{"capi_hello",  capi_hello},
	{"capi_hello_notify",  capi_hello_notify},
	{"end",NULL}
};

Hello * makeHelloInput(char *data)
{
	Hello *hello = NULL;

	hello = (Hello*)malloc(sizeof(Hello));
	
	hello->Name = json_getString(data, "data");
	hello->count = json_getNumber(data,"count");

	return hello;
}

char * makeHelloOutput(Hello_Output *output)
{
	struct json_object *newObj;
	char *buf = NULL;
	char *ret_buf = NULL;

	newObj = json_object_new_object();

	json_object_object_add(newObj, "string", json_object_new_string(output->string));

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


char * capi_hello(char *data)
{
	Hello *hello = NULL;
	Hello_Output *output = NULL;
	char *ret_buf = NULL;

	hello = makeHelloInput(data);

	output = (Hello_Output *)malloc(sizeof(Hello_Output));
	output->string = (char *)malloc(strlen("TEST OK!!!")+1);
	memset(output->string, 0x00, strlen("TEST OK!!!")+1);
	sprintf(output->string, "TEST OK!!!");
	
	ret_buf = makeHelloOutput(output);
	

	return ret_buf;
}

Hello_Notify * makeHelloNotifyInput(char *data)
{
	Hello_Notify *hello_no = NULL;

	hello_no = (Hello_Notify*)malloc(sizeof(Hello_Notify));

	hello_no->func = json_getString(data, "callbackfunc");
	hello_no->token= json_getString(data, "token");
	hello_no->Name = json_getString(data, "data");
	hello_no->count = json_getNumber(data,"count");

	return hello_no;
}

char *makeHelloNotifyOutput(Hello_Notify_Output *output)
{
	struct json_object *newObj, *paramObj;
	char *buf = NULL;
	char *ret_buf = NULL;
	char func_ptr[20];

	newObj = json_object_new_object();
	paramObj = json_object_new_object();

	json_object_object_add(newObj, "func", json_object_new_string(output->func));
	json_object_object_add(paramObj, "token", json_object_new_string(output->token));
	json_object_object_add(paramObj, "string", json_object_new_string(output->string));
	json_object_object_add(newObj, "param", paramObj);

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

void notify_func(char *data)
{
	Hello_Notify *hello_notify;
	Hello_Notify_Output *output_notify;
	char *ret_buf = NULL;
	pthread_detach(pthread_self());

	printf("SERVER>> notify function !!! data = %s\n", data);

	sleep(2);

	hello_notify = makeHelloNotifyInput(data);

	output_notify = (Hello_Notify_Output *)malloc(sizeof(Hello_Notify_Output));
	output_notify->string = (char *)malloc(strlen("Notify OK!!!")+1);
	memset(output_notify->string, 0x00, strlen("Notify OK!!!")+1);
	sprintf(output_notify->string, "Notify OK!!!");
	output_notify->func = hello_notify->func;
	output_notify->token = hello_notify->token;

	ret_buf = makeHelloNotifyOutput(output_notify);

	printf("SERVER>> notify_func ret_buf=%s\n", ret_buf);
	
	SendMessage(ret_buf, strlen(ret_buf));

	pthread_exit((void*)0);
	return 0;
}

char *capi_hello_notify(char *data)
{
	Hello *hello = NULL;
	Hello_Output *output = NULL;
	char *ret_buf = NULL;
	pthread_t thread_notify;
	int rc;

	hello = makeHelloInput(data);
	output = (Hello_Output *)malloc(sizeof(Hello_Output));
	output->string = (char *)malloc(strlen("Notify TEST OK!!!")+1);
	memset(output->string, 0x00, strlen("Notify TEST OK!!!")+1);
	sprintf(output->string, "Notify TEST OK!!!");
	
	ret_buf = makeHelloOutput(output);
	
	rc = pthread_create(&thread_notify, NULL, (void *)notify_func, data);
	if (rc){
	   printf("ERROR; return code from receiver_loop pthread_create() is %d\n", rc);
	   exit(-1);
	}
	
	return ret_buf;
}


char *capi_mainLoop(char *data, int size)
{
	char *func = NULL;
	char *param = NULL;
	char *ret_buf = NULL;
	int i = 0;
	
	printf("SERVER>> data=%s\n", data);
	
	func = json_getString(data, "func");
	param = json_getObject(data, "param");

	while(1)
	{
		if(!strncmp(cmd_mapping_table[i].cmd, "end", strlen("end")))
		{
			break;
		}

		if(!strncmp(cmd_mapping_table[i].cmd, func, strlen(func)))
		{
			ret_buf = cmd_mapping_table[i].func(param);
			break;
		}
		i++;
	}

	printf("SERVER>> ret_buf=%s\n", ret_buf);
	return ret_buf;
}
