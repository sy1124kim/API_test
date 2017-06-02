#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "IPC_IF.h"
#include "client.h"

#define SERVER	"./dockzen_launcher.sock"
#define NOTIFY_SERVER "./dockzen_launcher_notify.sock"

pclient_mapping_tables g_pclient_tables = NULL;
int g_pclient_table_cnt = 0;

pclient_mapping_tables RegisterFunctionTables(pclient_mapping_tables tables, int count)
{
	if(tables != NULL)
	{
		g_pclient_tables = tables;
		g_pclient_table_cnt = count;
	}	
}

pclient_mapping_tables getFuncTable(char *funcName)
{
	int i = 0;

	if(g_pclient_tables != NULL)
	{
		for(i = 0; i < g_pclient_table_cnt; i++)
		{
			
			if(strncmp((g_pclient_tables+i)->cmd, funcName, strlen(funcName))== 0)
			{
				break;
			}
		}
	}
	else
		return NULL;
	
	return (g_pclient_tables+i);
}

char * getFuncName(callbackFuncPtr funcPtr)
{
	int i = 0;
	char gTableFunc[15] = {0,};
		
	if(g_pclient_tables != NULL)
	{
		for(i = 0; i < g_pclient_table_cnt; i++)
		{
			sprintf(gTableFunc, "%x", (g_pclient_tables+i)->func);
			if(strncmp(funcPtr, gTableFunc, strlen(funcPtr)) == 0)
			{
				break;
			}
		}
	}
	else
		return NULL;
	
	return (g_pclient_tables+i)->cmd;
}


// {Callbackfunc,functionName,count of param,param1,param2...)
char * convertSendData(char *funcName, va_list ap)
{
	pclient_mapping_tables ptable;
	int i;
	char sendData[1024] = {0,};
	char *psendData = sendData;

	ptable = getFuncTable(funcName);

	if(ptable != NULL)
	{
		sprintf(psendData, "{%s,%d,", funcName, ptable->arguCnt);

		psendData += strlen(psendData);
		
		for(i = 0; i<ptable->arguCnt; i++)
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
			case e_type_funcPtr:
				sprintf(psendData, "%x,", va_arg(ap, int));
				psendData += strlen(psendData);
			}
		}
		psendData--;
		sprintf(psendData, "}");
	}
	else
	{	
		printf("<<Client>> Function Table empty!!!\n");
		return NULL;
	}

	printf("<<Client>> convertSendData >> %s\n", sendData);

	return sendData;
}

char *SendMessage(char *funcName, ...)
{
	SOCKET_HANDLE handle;
	int msg_size = 0;
	char *buf = NULL;
	int ret = 0;
	va_list ap;
	char *data;

	va_start(ap, funcName);
	data = convertSendData(funcName, ap);
	va_end(ap);
	
	ret = IPC_OpenClientConnection(SERVER, &handle);

	if(ret >= 0)
	{
		printf("<<Client>> SendMessage buf = %s\n", data);
		IPC_SendMessage(&handle, data, strlen(data));

		ret = IPC_GetMessage(&handle, &msg_size);
		if(msg_size > 0)
		{
			buf = (char *)malloc(msg_size +1);
			memset(buf, 0x00, msg_size);

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

void * NotifyData_parser(char *pdata)
{
	pclient_mapping_tables pcmd_mapping_table;
	callbackFuncPtr callbackfunc;
	char *funcName = NULL;
	int param_cnt = 0;
	char *buf = NULL;
	int *param_list[10];
	int i ;
	void *ret;
	char *notifyFuncName;

	buf = (char *)malloc(strlen(pdata));

	memset(buf, 0x00, strlen(buf));
	memcpy(buf, pdata+1, strlen(pdata)-2);

	funcName = strtok(buf, ",");
	param_cnt = atoi(strtok(NULL, ",")); // count of parameter
	callbackfunc = (callbackFuncPtr)strtok(NULL, ",");
	notifyFuncName = getFuncName(callbackfunc);

	pcmd_mapping_table = getFuncTable(notifyFuncName);

	for(i = 0; i<(param_cnt-1); i++)
	{
		switch(pcmd_mapping_table->arguType[i])
		{
		case e_type_int:
			param_list[i] = (int *)atoi(strtok(NULL, ","));
			break;
		case e_type_string:
			param_list[i] = (int *)strtok(NULL, ",");
			break;
		case e_type_double:
			param_list[i] = (int *)strtok(NULL, ",");
			break;
		}
	}
	
	ret = pcmd_mapping_table->func(param_list[0], param_list[1], param_list[2], param_list[3], param_list[4],param_list[5],
		param_list[6], param_list[7],param_list[8],param_list[9]);

	return ret;
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
					NotifyData_parser(buf);

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

