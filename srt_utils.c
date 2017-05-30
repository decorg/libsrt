#include "srt_utils.h"
#include "srt_error.h"
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int srt_is_number(const char* str)
{
	int ret = 1;
	size_t i = 0;
	
	assert(str != NULL);
	
	for(i=0; str[i] != '\0' && str[i] != '\n' && ret; i++)
		ret = isdigit(str[i]);
	
	return ret;
}

char* srt_append_str(char* str, const char* add)
{
	char* ret = NULL;
	
	assert(str != NULL);
	assert(add != NULL);
	
	ret = realloc(str, (strlen(str)+strlen(add)+1)*sizeof(char));
	
	if(ret != NULL)
	{
		strcat(ret, add);
	}
	else
	{
		srt_raise_error(SRT_ERR_BAD_ALLOC);
	}
	
	return ret;
}

char* srt_strdup(const char* str)
{
	char* ret = NULL;
	
	assert(str != NULL);
	
	ret = malloc((strlen(str)+1)*sizeof(char));
	
	if(ret != NULL)
	{
		strcpy(ret, str);
	}
	else
	{
		srt_raise_error(SRT_ERR_BAD_ALLOC);
	}
	
	return ret;
}

int srt_is_end_of_block(const char* str)
{
	int ret = 1;
	unsigned int i;
	
	assert(str != NULL);
	
	for(i=0; str[i] != '\0' && ret; i++)
	{
		switch(str[i])
		{
			case ' ':
			case '\t':
			case '\n':
				break;
			default:
				ret = 0;
				break;
		}
	}
	
	return ret;
}

int srt_char_to_digit(const char c)
{
	int ret = -1;
	
	switch(c)
	{
		case '0':
			ret = 0;
			break;
		case '1':
			ret = 1;
			break;
		case '2':
			ret = 2;
			break;
		case '3':
			ret = 3;
			break;
		case '4':
			ret = 4;
			break;
		case '5':
			ret = 5;
			break;
		case '6':
			ret = 6;
			break;
		case '7':
			ret = 7;
			break;
		case '8':
			ret = 8;
			break;
		case '9':
			ret = 9;
			break;
		default:
			break;
	}
	
	return ret;
}
