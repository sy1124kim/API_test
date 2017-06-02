#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "client.h"
#include "client_IF_stub.h"


int test_hello(void)
{
	
	char *buf;

	registerFuncTable();

	buf = capi_hello("TIZEN", 2);
	buf =capi_hello3(test_hello, "TEST", 1);

	printf("client>> return buf = %s\n", buf);
	
}

void Notify_server_setup(void)
{
	pthread_detach(pthread_self());
	Notify_server_init();
	pthread_exit((void*)0);

}

int main()
{
	pthread_t thread_server;
	int rc;

	rc = pthread_create(&thread_server, NULL, (void *) Notify_server_setup, NULL);

	if (rc){
	   printf("ERROR; return code from receiver_loop pthread_create() is %d\n", rc);
	   exit(-1);
	}

	test_hello();
	

    printf("done~~~\n");

	return 0;
}

