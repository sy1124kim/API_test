#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "IPC_IF.h"
#include "server.h"
#include "server_IF_stub.h"

#define SERVER	"./dockzen_launcher.sock"
#define NOTIFY_SERVER "./dockzen_launcher_notify.sock"

int server_init(void)
{
	SOCKET_HANDLE handle;
	int ret = 0;
	char *buf = NULL;
	int msgSize = 0;
	void *ret_buf = NULL;

	printf("<<Server>> server start \n");

	ret = IPC_OpenServerConnection(SERVER, &handle);
	printf("<<Server>> ret=%d\n", ret);
	if(ret >= 0)
	{
		while(1)
		{
			if(IPC_ClientAccept(&handle) != -1)
			{
				ret = IPC_GetMessage(&handle, &msgSize);
				printf("<<Server>> msgSize = %d\n", msgSize);
				if(msgSize > 0)
				{
					buf = (char *)malloc(msgSize +1);
					memset(buf, 0x00, msgSize);

					IPC_RecvMessage(&handle, buf, msgSize);
					printf("<<Server>> recvData = %s\n", buf);
					ret_buf = capi_mainLoop(buf, strlen(buf));
					printf("<<Server>> sendData = %s\n", ret_buf);

					IPC_SendMessage(&handle, ret_buf, strlen(ret_buf));

					if(buf != NULL)
						free(buf);
				}
			}
		}
		ret = IPC_OpenServerClose(&handle);
	}
	else
	{
		printf("<<Server>> server open error !!\n");
		return -1;
	}

	printf("<<Server>> close\n");	
	return 0;
	
}

int SendMessage(char *data, int size)
{
	SOCKET_HANDLE handle;

	if(data == NULL)
	{
		printf("<<Server>> SendMessage empty!!!\n");
		return -1;
	}
	
	if(IPC_OpenClientConnection(NOTIFY_SERVER, &handle) >= 0)
	{	
		printf("Server>> Send Message data = %s, size = %d\n", data, size);
		IPC_SendMessage(&handle, data, strlen(data));
		IPC_OpenClientClose(&handle);
	}
	else
	{
		printf("<<Server>> notify server connection error!!\n");
		return -1;
	}
	
	return 0;
}

