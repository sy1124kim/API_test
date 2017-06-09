#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IPC_IF.h"
#include "IPC_socket.h"

void ltoa(int val, char *string, int base)
{
	char buf[32] = {0};
	int i = 30;
	for(; val && i; --i, val /= base)
	{
		buf[i] = "0123456789abcdef"[val%base];
	}

	strncpy(string, &buf[i+1], strlen(&buf[i+1]));
}

int IPC_OpenServerConnection(char *server, SOCKET_HANDLE *phandle)
{
	return IPC_SocketOpenServerConnection(server, phandle);
}

int IPC_OpenServerClose(SOCKET_HANDLE *phandle)
{
	IPC_SocketOpenServerClose(phandle);
}

int IPC_SendMessage(SOCKET_HANDLE *phandle, char *buf, int size)
{
	char size_buf[32]={0};
	char recvbuf[32] = {0};
	
	ltoa(size, size_buf, 10);

	printf("<<IPC>> sendmessage!!\n");
	IPC_SocketSendMessage(phandle, size_buf, strlen(size_buf));
	printf("<<IPC>> sendmessage size_buf=%s!!\n", size_buf);
	while(1)
	{
		if(IPC_SocketRecvMessage(phandle,recvbuf, 10) >= 0)
		{
			printf("<<IPC>> sendmessage recvbuf=%s!!\n", recvbuf);
			break;
		}
	}
	printf("<<IPC>> sendmessage buf=%s\n", buf);
	IPC_SocketSendMessage(phandle, buf, size);

	return 0;
}

int IPC_RecvMessage(SOCKET_HANDLE *phandle, char *buf, int size)
{
	IPC_SocketRecvMessage(phandle, buf, size);
	return 0;
}

int IPC_GetMessage(SOCKET_HANDLE *phandle, int *msgSize)
{	
	return IPC_SocketGetMessage(phandle, msgSize);
}

int IPC_ClientAccept(SOCKET_HANDLE *phandle)
{
	return IPC_SocketClientAccept(phandle);
}

int IPC_OpenClientConnection(char *server, SOCKET_HANDLE *phandle)
{
	return IPC_SocketOpenClientConnection(server, phandle);
}

int IPC_OpenClientClose(SOCKET_HANDLE *phandle)
{
	return IPC_SocketOpenClientClose(phandle);
}

