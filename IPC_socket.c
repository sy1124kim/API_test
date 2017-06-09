#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include "IPC_IF.h"
#include "IPC_socket.h"

#define MAXLINE 1024

int IPC_SocketOpenServerConnection(char *server, SOCKET_HANDLE *phandle)
{
	int state;	
	int server_sockfd;
	struct sockaddr_un serveraddr;
	SOCKET_HANDLE handle;

	if(access(server, F_OK) == 0)
	{
		unlink(server);
	}
	
	if((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		printf("<<IPC>> socket error !!! \n");
		return -1;
	}	

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, server);

	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(state == -1)
	{
		printf("<<IPC>> bind error !\n");
		return -1;
	}

	state = listen(server_sockfd, 5);
	if(state == -1)
	{	
		printf("<<IPC>> listen error !!! \n");
		return -1;
	}

	(phandle->server_h) = server_sockfd;

	return 0;

}

int IPC_SocketOpenServerClose(SOCKET_HANDLE *phandle)
{
	close(phandle->server_h);
}

int IPC_SocketOpenClientClose(SOCKET_HANDLE *phandle)
{
	close(phandle->client_h);
}

int IPC_SocketClientAccept(SOCKET_HANDLE *phandle)
{
	struct sockaddr_un clientaddr;
	int client_len;
	int client_sockfd;

	printf("<< Accept>> server_h=%d\n", phandle->server_h);
	client_len = sizeof(clientaddr);

	while(1)
	{
		client_sockfd = accept(phandle->server_h, (struct sockaddr *)&clientaddr, &client_len);
		if(client_sockfd == -1)
		{
			printf("<<IPC>> [%s][%d] Error!! \n", __FUNCTION__, __LINE__);
			sleep(1);
			continue;
		}

		break;
	}

	phandle->client_h = client_sockfd;
	printf("<<IPC>> client = %d\n", phandle->client_h);

	return 0;
}

int IPC_SocketGetMessage(SOCKET_HANDLE *phandle, int *msgSize)
{	
	int size = 0;
	char buf[100] = {0,};

	IPC_SocketRecvMessage(phandle, buf, 32);
	printf("<<IPC>> GetMessage = %s\n", buf);
	IPC_SocketSendMessage(phandle,buf,10);

	(*msgSize) = atoi(buf);
	return 0;
}

int IPC_SocketRecvMessage(SOCKET_HANDLE *phandle, char * buf, int size)
{
	int ret_size = 0;
	printf("<<IPC>> recvMessage buf = %s\n", buf);
	while(1)
	{
		ret_size = read(phandle->client_h, buf, size);
		if(ret_size > 0)
			break;
	}
	printf("<<IPC>> recvMessage = %s\n, size = %d, ret_size = %d", buf, size, ret_size);
	return 0;	
}

int IPC_SocketSendMessage(SOCKET_HANDLE *phandle, char *buf, int size)
{
	return write(phandle->client_h, buf, size);	
}


int IPC_SocketOpenClientConnection(char *server, SOCKET_HANDLE *phandle)
{
	struct sockaddr_un clientaddr;
	int client_sockfd;
	int client_len;	
	int cnt = 0;

	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(client_sockfd == -1)
	{
		printf("<<IPC>> client error !!\n");
		return -1;
	}

	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, server);
	client_len = sizeof(clientaddr);

	while(cnt < 5 )
	{
		if(connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) >= 0)
		{
			printf("<<IPC>> server connection OK!!\n");
			break;
		}
		else
		{
			cnt++;
			sleep(1);
		}
	}

	if(cnt == 5)
	{
		printf("<<IPC>> client connect error !!\n");
		phandle->client_h = -1;
	}
	else
		phandle->client_h = client_sockfd;

	return 0;
}


