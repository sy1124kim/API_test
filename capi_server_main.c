#include <stdio.h>
#include <pthread.h>
#include "dockzen_json.h"

extern void receiver_loop(void);

extern int test_hello(void);
extern int test_hello2(void);


char * hello(char *buf)
{
	char *ret_buf;
	char *data;
	int count;
	char test_buf[1024]={0,};
	char *p_test_buf = NULL;
	int i = 0;
	
	// hello routine;
	printf("SERVER >> Hello [%s][%d]\n", __FUNCTION__, __LINE__);

	data = json_getString(buf, "data");
	count = json_getNumber(buf, "count");
	printf("SERVER>> data = %s, count = %d\n",data, count );

	p_test_buf = test_buf;
	for(i = 0; i<count; i++)
	{
		sprintf(p_test_buf, "%s ", data);
		p_test_buf += strlen(data)+1;
	}


	
	ret_buf = (char *)malloc((strlen(test_buf)));
	memset(ret_buf, 0x00, strlen(test_buf));
	memcpy(ret_buf, test_buf, strlen(test_buf) - 1);
	
	printf("SERVER>> test_buf = %s, ret_buf = %s\n", test_buf, ret_buf);

	return ret_buf;
}


int main()
{
	pthread_t thread_server, thread_client;
	int rc;

	rc = pthread_create(&thread_server, NULL, (void *) receiver_loop, NULL);

	if (rc){
       printf("ERROR; return code from receiver_loop pthread_create() is %d\n", rc);
       exit(-1);
    }

	rc = pthread_create(&thread_client, NULL, (void *)test_hello, NULL);
    if (rc){
       printf("ERROR; return code from caller_loop pthread_create() is %d\n", rc);
       exit(-1);
    }

    rc = pthread_create(&thread_client, NULL, (void *)test_hello2, NULL);
    if (rc){
       printf("ERROR; return code from caller_loop pthread_create() is %d\n", rc);
       exit(-1);
    }

    pthread_join(thread_server, NULL);
    pthread_join(thread_client, NULL);

    printf("done~~~\n");
}
