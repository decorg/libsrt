#include "srt_frame.h"
#include <stdlib.h>

srt_frame* srt_new_frame()
{
	srt_frame *ret = malloc(sizeof(srt_frame));
	
	if(ret != NULL)
	{
		ret->id = 0;
		srt_init_time(&(ret->start_time), 0, 0, 0, 0);
		srt_init_time(&(ret->end_time), 0, 0, 0, 0);
		ret->text = NULL;
	}
	
	return ret;
}

void srt_free_frame(srt_frame *f)
{
	if(f != NULL)
	{
		if(f->text != NULL)
			free(f->text);

		free(f);
	}
}

int srt_is_frame_valid(const srt_frame *f)
{
	int ret = 0;
	
	if(f != NULL)
	{
		if(f->text != NULL && f->id > 0)
		{
			ret = srt_time_lt(&(f->start_time), &(f->end_time));
		}
	}
	
	return ret;
}
