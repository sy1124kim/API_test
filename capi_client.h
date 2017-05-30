struct handle_info_ 
{
	int client_h;
};

typedef struct handle_info_ HANDLE;

int SendMessage(HANDLE *phandle, char * msg, int size);

