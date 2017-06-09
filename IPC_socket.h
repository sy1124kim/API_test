#ifndef __DOCKZEN_IPC_SOCKET_H__
#define __DOCKZEN_IPC_SOCKET_H__

int IPC_SocketOpenServerConnection(char *server, SOCKET_HANDLE *phandle);
int IPC_SocketOpenServerClose(SOCKET_HANDLE *phandle);

int IPC_SocketRecvMessage(SOCKET_HANDLE *phandle, char * buf, int size);
int IPC_SocketServerSendMessage(SOCKET_HANDLE *phandle, char *buf, int size);
int IPC_SocketGetMessage(SOCKET_HANDLE *phandle, int *msgSize);
int IPC_SocketClientAccept(SOCKET_HANDLE *phandle);

int IPC_SocketOpenClientConnection(char *server, SOCKET_HANDLE *phandle);
int IPC_SocketOpenClientClose(SOCKET_HANDLE *phandle);

#endif /* _DOCKZEN_IPC_SOCKET_H__ */
