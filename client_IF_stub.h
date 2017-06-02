#ifndef __CLIENT_IF_STUB_H__
#define __CLIENT_IF_STUB_H__

char *capi_hello(char *data, int count);
void registerFuncTable();
char *capi_hello3(callbackFuncPtr func, char *data, int count);

#endif /* __CLIENT_IF_STUB_H__*/

