#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include "capi_client.h"


#define SERVER	"./dockzen_launcher.sock"
#define MAXLINE 1024

void client_init(HANDLE *phandle)
{
	struct sockaddr_un clientaddr;
	int client_sockfd;
	int client_len;	

	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(client_sockfd == -1)
	{
		printf("client error !!\n");
		return;
	}

	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, SERVER);
	client_len = sizeof(clientaddr);

	if(connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
	{
		printf("client connect error !!\n");
		return;
	}

	phandle->client_h = client_sockfd;
}

int recvMessage(HANDLE *phandle, char * msg, int size)
{
	struct sockaddr_un clientaddr;
	int client_len;
	int client_sockfd;

	do
	{
		if(read(phandle->client_h, msg, size) > 0)
		{
			break;
		}
	}while(1);
	
	return 0;
}

int SendMessage(HANDLE *phandle, char * msg, int size)
{
	printf("CLIENT>> caller:TX\n");
	write(phandle->client_h, msg, size);
}

