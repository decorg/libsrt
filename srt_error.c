#include "srt_error.h"

static srt_error srt_last_error = SRT_ERR_NONE;

void srt_clear_error(void)
{
	srt_last_error = SRT_ERR_NONE;
}

void srt_raise_error(const srt_error error)
{
	if(error >= SRT_ERR_NONE && error < SRT_ERR_LAST)
		srt_last_error = error;
	else
		srt_last_error = SRT_ERR_UNKNOWN;
}

srt_error srt_get_last_error(void)
{
	return srt_last_error;
}
