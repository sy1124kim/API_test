#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "IPC_IF.h"
#include "server.h"

#define SERVER	"./dockzen_launcher.sock"
#define NOTIFY_SERVER "./dockzen_launcher_notify.sock"

pcmd_mapping_tables g_pServer_tables = NULL;
int g_pServer_table_cnt = 0;

int server_table_init(pcmd_mapping_tables ptables, int cnt)
{
	if(ptables != NULL)
	{
		g_pServer_tables = ptables;
		g_pServer_table_cnt = cnt;
	}

	return 0;
}

pcmd_mapping_tables searchCmdTables(char *funcName)
{
	int i = 0;

	if(g_pServer_tables != NULL)
	{
		for(i=0; i<g_pServer_table_cnt; i++)
		{
			if(strncmp((g_pServer_tables+i)->cmd, funcName, strlen(funcName)) ==0)
			{
				break;
			}			
		}
	}
	else
	{
		return NULL;
	}

	return (g_pServer_tables+i);
}

void * capi_parser(char *pdata)
{
	pcmd_mapping_tables pcmd_mapping_table;
	char *funcName = NULL;
	int param_cnt = 0;
	char *buf = NULL;
	int *param_list[10];
	int i ;
	void *ret = NULL;

	buf = (char *)malloc(strlen(pdata));

	if(buf == NULL)
		return NULL;
	
	memset(buf, 0x00, strlen(buf));
	memcpy(buf, pdata+1, strlen(pdata)-2);

	funcName = strtok(buf, ",");
	param_cnt = atoi(strtok(NULL, ","));// count of parameter
	
	pcmd_mapping_table = searchCmdTables(funcName);

	if(param_cnt == pcmd_mapping_table->arguCnt)
	{
		for(i = 0; i<param_cnt; i++)
		{
			switch(pcmd_mapping_table->arguType[i])
			{
			case e_type_int:
				param_list[i] = (int *)atoi(strtok(NULL, ","));
				break;
			case e_type_string:
			case e_type_double:
			case e_type_funcPtr:
			default:
				param_list[i] = (int *)strtok(NULL, ",");
			}
		}
	}
	else
	{
		return NULL;
	}
	
	ret = pcmd_mapping_table->func(param_list[0], param_list[1], param_list[2], param_list[3], param_list[4],param_list[5],
		param_list[6], param_list[7],param_list[8],param_list[9]);

	return ret;
}

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
					ret_buf = capi_parser(buf);
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
char * convertSendData(char *funcName, cmdFunctionPtr ptr, va_list ap)
{
	pcmd_mapping_tables ptable;
	int i;
	char sendData[1024] = {0,};
	char *psendData = sendData;

	ptable = searchCmdTables(funcName);

	if(ptable != NULL)
	{
		sprintf(psendData, "{%s,%d,%s,", funcName, ptable->arguCnt, ptr);
		psendData += strlen(psendData);
			
		for(i = 1; i<ptable->arguCnt; i++)
		{
			switch(ptable->arguType[i])
			{
			case e_type_int:
				sprintf(psendData,"%d,", va_arg(ap, int));
				psendData += strlen(psendData);
				break;
			case e_type_string:
				sprintf(psendData,"%s,", va_arg(ap, int));
				psendData += strlen(psendData);
				break;
			default:
				sprintf(psendData,"%d,", va_arg(ap, int));
				psendData += strlen(psendData);	
			}
		}
		psendData--;
		sprintf(psendData, "}");
	}
	else
		return NULL;

	return sendData;
}

int NotifySendMessage(char *funcName,cmdFunctionPtr ptr, ...)
{
	SOCKET_HANDLE handle;
	va_list ap;
	char *data;

	va_start(ap, ptr);
	data = convertSendData(funcName,ptr, ap);
	va_end(ap);

	if(data == NULL)
	{
		printf("<<Server>> SendMessage empty!!!\n");
		return -1;
	}
	
	if(IPC_OpenClientConnection(NOTIFY_SERVER, &handle) >= 0)
	{	
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

