#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "API_type.h"
#include "client.h"
#include "client_IF_stub.h"


int test_hello(void)
{
	Hello hello;
	Hello_Output output;
	char *buf;

	hello.Name = (char *)malloc(strlen("TIZEN") + 1);
	memset(hello.Name, 0x00, strlen("TIZEN") + 1);
	sprintf(hello.Name, "TIZEN");
	hello.count = 2;
	
	capi_hello(&hello, &output);
	
	//buf =capi_hello3(test_hello, "TEST", 1);

	//printf("client>> return buf = %s\n", buf);
	
}

int notify_Hello_test(void)
{
	Hello hello;
	Hello_Output output;
	char *buf;

	hello.Name = (char *)malloc(strlen("TIZEN") + 1);
	memset(hello.Name, 0x00, strlen("TIZEN") + 1);
	sprintf(hello.Name, "TIZEN");
	hello.count = 2;

	capi_hello_notify(test_hello, &hello, &output);

	while(1)
	{
	
	}
	
	return 0;
	
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
	int num = 1;

	printf("============ Menu ================\n");
	printf("1. Normal case\n");
	printf("2. Notify case\n");
	printf("ex) >> 1\n");
	printf(">> ");
	fflush(stdin);
	scanf("%d", &num);

	switch(num)
	{
	case 2:
		
		rc = pthread_create(&thread_server, NULL, (void *) Notify_server_setup, NULL);
		
		if (rc){
		   printf("ERROR; return code from receiver_loop pthread_create() is %d\n", rc);
		   exit(-1);
		}
		notify_Hello_test();
		break;
	case 1:
	default:
		test_hello();
	}

    printf("done~~~\n");

	return 0;
}

