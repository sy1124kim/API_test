#include <stdio.h>
#include "server.h"

extern void register_function();

void receiver_loop_stup()
{
	server_init();
}


int main()
{
	int rc;

//	register_function();

	receiver_loop_stup();

    printf("done~~~\n");

	return 0;
}
