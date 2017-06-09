#ifndef __DOCKZEN_IPC_H__
#define __DOCKZEN_IPC_H__

struct _IPC_SOCKET_HANDLE_s
{
	int server_h;
	int client_h;
};

typedef struct _IPC_SOCKET_HANDLE_s SOCKET_HANDLE;

int IPC_OpenServerConnection(char *server, SOCKET_HANDLE *phandle);
int IPC_OpenServerClose(SOCKET_HANDLE *phandle);

int IPC_RecvMessage(SOCKET_HANDLE *phandle, char *buf, int size);
int IPC_SendMessage(SOCKET_HANDLE *phandle, char *buf, int size);
int IPC_ClientAccept(SOCKET_HANDLE *phandle);
int IPC_GetMessage(SOCKET_HANDLE *phandle, int *msgSize);

int IPC_OpenClientConnection(char *server, SOCKET_HANDLE *phandle);
int IPC_OpenClientClose(SOCKET_HANDLE *phandle);

void ltoa(int val, char *string, int base);

#endif /* __DOCKZEN_IPC_H__ */
