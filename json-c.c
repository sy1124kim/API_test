#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/local/include/json-c/json.h"
#include "json-c.h"

char *json_getString(char *data, char * key)
{
	struct json_object *inputObj;
	struct json_object *bodyObj;
	enum json_type type;
	char *ret_buf=NULL;
	char *buf = NULL;	
	int i, j = 0;

	inputObj = json_tokener_parse(data);
	type = json_object_get_type(inputObj);

	if(type == json_type_object)
	{
		if(json_object_object_get_ex(inputObj, key, &bodyObj))
		{

			buf = json_object_to_json_string_ext(bodyObj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
			ret_buf = malloc(strlen(buf) -1);
			if(ret_buf != NULL)
			{
				memset(ret_buf, 0x00, strlen(buf) -1); // "<-- exclude			

				for(i = 1, j = 0; i<strlen(buf)-1; i++) // "\ <-- exclude
				{
					if(buf[i] == '\\')
						continue;

					ret_buf[j++] = buf[i];
				}
			}	
		}
		else 
		{
			printf("ERROR : no data\n");

		}
	}

	json_object_put(inputObj);

	printf("JSON>> ret_buf = %s\n", ret_buf);
	
	return ret_buf;
}

int json_getNumber(char *data, char *key)
{
	struct json_object *inputObj;
	struct json_object *bodyObj;
	enum json_type type;
	int ret_num=0;
	
	inputObj = json_tokener_parse(data);
	type = json_object_get_type(inputObj);

	if(type == json_type_object)
	{
		if(json_object_object_get_ex(inputObj, key, &bodyObj))
		{
			ret_num = json_object_get_int(bodyObj);
		}
		else 
		{
			printf("ERROR : no data\n");

		}
	}
	json_object_put(inputObj);
	return ret_num;
}


char *json_getObject(char *data, char *key)
{

	struct json_object *inputObj = NULL;
	struct json_object *bodyObj = NULL;
	char *buf = NULL;
	char *ret_buf = NULL;

	inputObj = json_tokener_parse(data);

	if(inputObj != NULL)
	{
		if(json_object_object_get_ex(inputObj, key, &bodyObj))
		{
			if(json_object_get_type(bodyObj) == json_type_object)
			{
				buf = json_object_to_json_string_ext(bodyObj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
				ret_buf = (char *)malloc(strlen(buf)+1);

				if(ret_buf != NULL)
				{
					memset(ret_buf, 0x00, strlen(buf)+1);
					memcpy(ret_buf, buf, strlen(buf));
				}
			}
		}
		json_object_put(inputObj);
	}
	return ret_buf;
}

