#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "capi_client_hello.h"


int test_hello(void)
{

	pthread_detach(pthread_self());
	
	printf("client>> test hello_1!!\n");
	capi_hello("test", 1);

	pthread_exit((void*)0);
}

int test_hello2(void)
{
	pthread_detach(pthread_self());

	printf("client>> test hello_2!!\n");
	capi_hello("Hello!!", 2);
	pthread_exit((void*)0);
}


