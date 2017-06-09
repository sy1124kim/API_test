#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "IPC_IF.h"
#include "API_type.h"
#include "client.h"
#include "client_IF_stub.h"

#define SERVER	"./dockzen_launcher.sock"
#define NOTIFY_SERVER "./dockzen_launcher_notify.sock"

char *SendMessage(char * data, int size)
{
	SOCKET_HANDLE handle;
	int msg_size = 0;
	char *buf = NULL;
	int ret = 0;
	//char *data;
	
	ret = IPC_OpenClientConnection(SERVER, &handle);
	if(ret >= 0)
	{
		printf("<<Client>> SendMessage buf = %s\n", data);
		IPC_SendMessage(&handle, data, strlen(data));

		ret = IPC_GetMessage(&handle, &msg_size);
		if(msg_size > 0)
		{
			buf = (char *)malloc(msg_size +1);
			memset(buf, 0x00, msg_size+1);

			IPC_RecvMessage(&handle,buf,msg_size);
		}
		printf("<<Client>> recvMessage buf = %s\n", buf);

		IPC_OpenClientClose(&handle);
	}
	else
	{
		printf("<<Client>> server connection error !!\n");
		return NULL;
	}
	
	return buf;
}

int Notify_server_init(void)
{	
	SOCKET_HANDLE handle;
	int ret = 0;
	char *buf = NULL;
	int msgSize = 0;

	printf("<<Client>> Notify_server start \n");

	ret = IPC_OpenServerConnection(NOTIFY_SERVER, &handle);

	if(ret >= 0)
	{
		while(1)
		{
			if(IPC_ClientAccept(&handle) != -1)
			{
				ret = IPC_GetMessage(&handle, &msgSize);

				if(msgSize > 0)
				{
					buf = (char *)malloc(msgSize +1);
					memset(buf, 0x00, msgSize);

					IPC_RecvMessage(&handle, buf, msgSize);
					capi_notify_mainLoop(buf, strlen(buf));

					if(buf != NULL)
						free(buf);
				}
			}
		}
		ret = IPC_OpenServerClose(&handle);
	}
	else
	{
		printf("<<Client>> Server connection error !!\n");
		return -1;
	}
	
	printf("<<Client>> Notify_server close\n");	
}

/*
char *SendMessageWithNotify(char *funcName, ...)
{
	SOCKET_HANDLE handle;
	int msg_size = 0;
	char *buf = NULL;
	int ret = 0;
	va_list ap;
	char *data = NULL;
	
	va_start(ap, funcName);
	data = convertSendData(funcName, ap);
	va_end(ap);

	ret = IPC_OpenClientConnection(SERVER, &handle);

	if(ret >= 0)
	{		
		printf("<<Client>> Client Connection !!!\n");
		printf("<<Client>> SendData =%s!!!\n", data);
		IPC_SendMessage(&handle, data, strlen(data));
		printf("<<Client>> sendData !!!\n");
		ret = IPC_GetMessage(&handle, &msg_size);
		if(msg_size > 0)
		{
			buf = (char *)malloc(msg_size +1);
			memset(buf, 0x00, msg_size);
			
			IPC_RecvMessage(&handle,buf,msg_size);
			printf("<<Client>> RecvData =%s!!!\n", buf);
			if(buf != NULL)
				free(buf);
		}

		IPC_OpenClientClose(&handle);
	}
	else
	{
		printf("<<Client>> Server connection error!!!\n");
		return NULL;
	}

	return buf;	
}
*/
