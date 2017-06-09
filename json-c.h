#ifndef __DOCKZEN_JSON_H__
#define __DOCKZEN_JSOn_H__

typedef struct _hello_info_s
{
	char *data;
	int *count;
}hello_info;

char *json_getString(char *data, char * key);
char *json_getObject(char *data, char *key);
char *hello_makeJson(char *funName, hello_info *helloInfo);
int json_getNumber(char *data, char *key);


#endif /* __DOCKZEN_JSON_H__ */

