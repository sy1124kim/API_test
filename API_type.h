#ifndef __API_TYPE_H__
#define __API_TYPE_H__

typedef char *(*callbackFuncPtr)(char *, ...);

typedef struct _hello_s
{
	char *Name;
	int count;
}Hello;

typedef struct _hello_output_s
{
	char *string;
}Hello_Output;

typedef struct _hello_notify_s
{
	callbackFuncPtr *func;
	callbackFuncPtr *token;
	char *Name;
	int count;
}Hello_Notify;

typedef struct _hello_notify_output_s
{
	callbackFuncPtr *func;
	callbackFuncPtr *token;
	char *string;
}Hello_Notify_Output;


#endif /* __API_TYPE_H__ */
