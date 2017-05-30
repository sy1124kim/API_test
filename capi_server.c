#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include "capi_server.h"
#include <stdbool.h>

#include "dockzen_json.h"


#define SERVER	"./dockzen_launcher.sock"
#define MAXLINE 1024


struct handle_info_s 
{
	int server_h;
	int client_h;

};
typedef struct handle_info_s HANDLES;


/*
{
	func : "xxxx"
	args : 
	{
	}
}
*/

struct _func_info_s
{
	char *Name;
	char *Args;
};

typedef struct _func_info_s func_info;


static func_info * convertStructure(char *data)
{
	func_info *funcInfo;

	funcInfo = (func_info*)malloc(sizeof(struct _func_info_s));

	funcInfo->Name = json_getString(data, "func");
	funcInfo->Args = json_getObject(data, "args");

	printf("SERVER>> funcInfo->Name = %s\n", funcInfo->Name);

	return funcInfo;
}

static char * data_parsing(func_info * funcInfo)
{
	int i = 0;
	char *ret_buf;
	
	printf("SERVER>> [%s][%d]\n", __FUNCTION__, __LINE__);
	
	if(funcInfo == NULL)
		return ;

	while(1)
	{
		
		if(!strncmp("end", cmd_mapping_table[i].cmd, strlen("end")))
		{
			break;
		}
		if(!strncmp(cmd_mapping_table[i].cmd, funcInfo->Name, strlen(cmd_mapping_table[i].cmd)))
		{
			printf("SERVER>> funcInfo-> Name = %s\n", funcInfo->Name);
			printf("SERVER>> funcInfo-> Args = %s\n", funcInfo->Args);
			ret_buf = cmd_mapping_table[i].func(funcInfo->Args);
			break;
			
		}
		i++;
	}

	return ret_buf;

}


int server_init(HANDLES *phandle)
{
	int state;	
	int server_sockfd;
	struct sockaddr_un serveraddr;

	if(access(SERVER, F_OK) == 0)
	{
		unlink(SERVER);
	}
	
	if((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error !!! \n");
		return -1;
	}	

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, SERVER);

	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(state == -1)
	{
		printf("bind error !\n");
		return -1;
	}

	state = listen(server_sockfd, 5);
	if(state == -1)
	{	
		printf("listen error !!! \n");
		return -1;
	}

	(phandle->server_h) = server_sockfd;

}

int Server_recvMessage(HANDLES *phandle, char * buf, int size)
{
	struct sockaddr_un clientaddr;
	int client_len;
	int client_sockfd;

	do
	{
		client_sockfd = accept(phandle->server_h, (struct sockaddr *)&clientaddr, &client_len);
		if(client_sockfd == -1)
		{
			printf("Error!!\n");
			return -1;
		}

		memset(buf, 0x00, MAXLINE);
		if(read(client_sockfd, buf, MAXLINE) > 0)
		{
			phandle->client_h = client_sockfd;
			break;
		}

		close(client_sockfd);
	}while(1);
	
	return 0;
}

int Server_SendMessage(HANDLES *phandle, char *buf, int size)
{
	write(phandle->client_h, buf, size);
	return 0;
	
}

void receiver_loop(void)
{
	HANDLES handle;
	char buf[MAXLINE];
	func_info *funcInfo;
	char *ret_buf = NULL;
	int ret;

	pthread_detach(pthread_self());

	printf("SERVER>> receiver_loop\n");

	ret = server_init(&handle);

	do
	{
		if(Server_recvMessage(&handle, buf, MAXLINE) == 0)
		{
			funcInfo = convertStructure(buf);
			ret_buf = data_parsing(funcInfo);

			if(ret_buf != NULL)
				Server_SendMessage(&handle, ret_buf, strlen(ret_buf));

			
		}
	} while(1);

	pthread_exit((void*)0);
}

