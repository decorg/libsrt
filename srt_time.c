#include "srt_time.h"
#include "srt_error.h"
#include "srt_frame.h"
#include "srt_utils.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void srt_init_time(srt_time* t, srt_msec ms, srt_sec s, srt_min min, srt_hour h)
{
	assert(t != NULL);
	assert(ms < 1000);
	assert(s < 60);
	assert(min < 60);
	assert(h < 100);
	
	t->msec = ms;
	t->sec = s;
	t->min = min;
	t->hours = h;
}

int srt_time_cmp(const srt_time* t1, const srt_time* t2)
{
	int ret = 0;
	
	assert(srt_is_time_valid(t1));
	assert(srt_is_time_valid(t2));
	
	if(t1->hours > t2->hours)
	{
		ret = 1;
	}
	else if(t1->hours == t2->hours)
	{
		if(t1->min > t2->min)
		{
			ret = 1;
		}
		else if(t1->min == t2->min)
		{
			if(t1->sec > t2->sec)
			{
				ret = 1;
			}
			else if(t1->sec == t2->sec)
			{
				if(t1->msec > t2->msec)
				{
					ret = 1;
				}
				else if(t1->msec == t2->msec)
				{
					ret = 0;
				}
				else
				{
					ret = -1;
				}
			}
			else
			{
				ret = -1;
			}
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

int srt_add_times(const srt_time* t1, const srt_time* t2, srt_time *res)
{
	int ret = 1;
	int remainder = 0;
	
	assert(srt_is_time_valid(t1));
	assert(srt_is_time_valid(t2));
	assert(res != NULL);
	
	res->msec = (t1->msec + t2->msec) % 1000;
	remainder = (((t1->msec + t2->msec) / 1000) > 0);
	
	res->sec = (t1->sec + t2->sec + remainder) % 60;
	remainder = (((t1->sec + t2->sec + remainder) / 60) > 0);
	
	res->min = (t1->min + t2->min + remainder) % 60;
	remainder = (((t1->min + t2->min + remainder) / 60) > 0);
	
	res->hours = (t1->hours + t2->hours + remainder) % 100;
	remainder = (((t1->hours + t2->hours + remainder) / 100) > 0);
	
	/* Overflow */
	if(remainder)
	{
		ret = 0;
		srt_raise_error(SRT_ERR_TIME_OVERFLOW);
	}
	else
	{
		assert(srt_is_time_valid(res));
	}
	
	return ret;
}

int srt_scan_time(const char* str, srt_time *t)
{
	int ret = 0;
	
	assert(str != NULL);
	assert(t != NULL);
	
	if(strlen(str) >= 11)
	{
	
		t->hours = srt_char_to_digit(str[0])*10 + 
			srt_char_to_digit(str[1]);
		t->min = srt_char_to_digit(str[3])*10 + 
			srt_char_to_digit(str[4]);
		t->sec = srt_char_to_digit(str[6])*10 + 
			srt_char_to_digit(str[7]);
		t->msec = srt_char_to_digit(str[9])*100 + 
			srt_char_to_digit(str[10])*10 +
			srt_char_to_digit(str[11]);
			
		ret = isdigit(str[0]) && isdigit(str[1]) && isdigit(str[3]) &&
			isdigit(str[4]) && isdigit(str[6]) && isdigit(str[7]) &&
			isdigit(str[9]) && isdigit(str[10]) && isdigit(str[11]);
	}
	
	return ret;
}

int srt_read_times(const char* str, struct srt_frame *f)
{
	int ret = 1;
	
	assert(str != NULL);
	assert(f != NULL);
	
	ret = srt_scan_time(str, &(f->start_time)) && 
		srt_scan_time(str+17, &(f->end_time));
	
	return ret;
}

int srt_is_time_valid(const srt_time* t)
{
	int ret = 0;
	
	if(t != NULL)
	{
		if(t->msec < 1000 && t->min < 60 && 
		   t->sec < 60 && t->hours < 100)
			ret = 1;
	}
	
	return ret;
}
