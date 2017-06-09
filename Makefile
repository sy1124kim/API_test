all: server client

client : 
	gcc -static client.c client_IF_stub.c IPC_IF.c IPC_socket.c hello_test.c json-c.c -L/usr/local/lib -ljson-c -l pthread -o testAPI_client

server :
	gcc -static main.c server.c server_IF_stub.c IPC_IF.c IPC_socket.c json-c.c -L/usr/local/lib -ljson-c -l pthread -o testAPI_server


.PHONY: all server client