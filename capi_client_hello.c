#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "capi_client.h"
#include "dockzen_json.h"

/*
{
	func : string,
	args :
		{
			data : string,
			count : integer
		}
}
*/

extern int SendMessage(HANDLE *phandle, char * msg, int size);
extern int recvMessage(HANDLE *phandle, char * msg, int size);


int capi_hello(char *data, int count)
{
	hello_info helloInfo;
	char *buf;
	char ret_buf[1024];
	HANDLE handle;

	helloInfo.data = data;
	helloInfo.count = count;

	buf = hello_makeJson("hello", &helloInfo);

	printf("Client >> buf = %s\n", buf);

	client_init(&handle);
	
	SendMessage(&handle, buf, strlen(buf));

	recvMessage(&handle,ret_buf,1024);
	printf("Client >> ret_buf = %s\n", ret_buf);	
}


