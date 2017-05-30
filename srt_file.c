#include "srt_file.h"
#include "srt_utils.h"
#include "srt_time.h"
#include "srt_frame.h"
#include "srt_error.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SRT_DEFAULT_BUFFER_SIZE 256

srt_file* srt_open(const char* file, const srt_mode mode)
{
	assert(file != NULL);
	assert(srt_is_mode_valid(mode));
	
	return srt_open_with_size(file, mode, SRT_DEFAULT_BUFFER_SIZE);
}

srt_file* srt_open_with_size(const char* file, const srt_mode mode, 
	const size_t size)
{
	srt_file *ret = NULL;
	
	assert(file != NULL);
	assert(srt_is_mode_valid(mode));
	assert(size > 0);
	
	ret = malloc(sizeof(srt_file));
	
	if(ret != NULL)
	{
		ret->cursor = 0;
		ret->mode = mode;
		ret->file = NULL;
		
		switch(mode)
		{
			case SRT_READ:
				ret->file = fopen(file, "r");
				break;
			case SRT_WRITE:
				ret->file = fopen(file, "w");
				break;
			default:
				srt_die("Fatal error: Invalid mode.\n");
				break;
		}
		
		/*
		 * If we opened the file, we allocate the buffer.
		 */
		if(ret->file != NULL)
		{
			ret->buffer = malloc(size * sizeof(char));
			if(ret->buffer != NULL)
			{
				ret->buffer_size = size;
			}
			else
			{
				srt_raise_error(SRT_ERR_BAD_ALLOC);
				fclose(ret->file);
				free(ret);
				ret = NULL;
			}
		}
		else
		{
			srt_raise_error(SRT_ERR_BAD_IO);
			free(ret);
			ret = NULL;
		}
	}
	
	return ret;
}

void srt_close(srt_file *file)
{
	if(file != NULL)
	{
		if(file->file != NULL)
			fclose(file->file);
			
		if(file->buffer != NULL)
			free(file->buffer);
			
		free(file);
	}
}

int srt_set_buffer_size(srt_file* file, size_t size)
{
	int ret = 0;
	char* new_buffer = NULL;
	
	assert(srt_is_file_valid(file));
	assert(size > 0);
	
	new_buffer = realloc(file->buffer, size * sizeof(char));
	
	if(new_buffer != NULL)
	{
		file->buffer = new_buffer;
		file->buffer_size = size;
		ret = 1;
	}
	else
	{
		srt_raise_error(SRT_ERR_BAD_ALLOC);
	}
	
	return ret;
}

int srt_read_frame_header(srt_file *f, srt_frame* frame)
{
	int ret = 0;
	
	assert(srt_is_file_valid(f));
	assert(f->mode == SRT_READ);
	assert(frame != NULL);
	
	/* We read the first line of the block. */
	if(fgets(f->buffer, f->buffer_size, f->file) != NULL)
	{
		/* If the buffer was large enough */
		if(strchr(f->buffer,'\n') != NULL)
		{
	
			/* If the read line is the frame id, we skip it */
			if(srt_is_number(f->buffer))
			{
				if(fgets(f->buffer, f->buffer_size, f->file)!=NULL)
				{
					/* If the buffer was large enough */
					if(strchr(f->buffer,'\n') != NULL)
					{
						ret = 1;
					}
					else
					{
						srt_raise_error(SRT_ERR_TOO_LONG);
					}
				}
				else
				{
					srt_raise_error(SRT_ERR_BAD_IO);
				}
			}
			else
			{
				ret = 1;
			}
			
			/* 
			 * If everything is OK, we can now, parse the start and 
			 * the end time of the frame 
			 */
			if(ret)
			{
				if(!srt_read_times(f->buffer, frame))
				{
					ret = 0;
					srt_raise_error(SRT_ERR_BAD_TIME_FORMAT);
				}
			}
		}
		else /* strchr(f->buffer,'\n') == NULL */
		{
			srt_raise_error(SRT_ERR_TOO_LONG);
		}
	}
	else /* fgets(f->buffer, f->buffer_size, f->file) == NULL */
	{
		srt_raise_error(SRT_ERR_BAD_IO);
	}
	return ret;
}

int srt_read_frame_content(srt_file *f, srt_frame* frame)
{
	int ret = 1;
	int running = 1;
	char* tmp = NULL;
	
	assert(srt_is_file_valid(f));
	assert(f->mode == SRT_READ);
	assert(frame != NULL);
			
	/* We read the script */
	while(running)
	{
		if(fgets(f->buffer, f->buffer_size, f->file) != NULL)
		{
			/* 
			 * If we are at the end of the block, we stop the 
			 * reading.
			 */
			if(srt_is_end_of_block(f->buffer))
			{
				running = 0;
			}
			else
			{
				/*
				 * If the content is empty
				 */
				if(frame->text == NULL)
				{
					frame->text = srt_strdup(f->buffer);
					if(frame->text == NULL)
					{
						running = 0;
						ret = 0;
					}
				}
				else
				{
					/* We reallocate */
					tmp=srt_append_str(frame->text,f->buffer);
					if(tmp != NULL)
					{
						frame->text = tmp;
					}
					else
					{
						/* 
						 * On error, we release the 
						 * allocated memory 
						 */
						running = 0;
						ret = 0;
						free(frame->text);
						frame->text = NULL;
					}
				}
			}
		}
		else /* fgets(f->buffer, f->buffer_size, f->file) == NULL */
		{
			running = 0;
			ret = 0;
			srt_raise_error(SRT_ERR_BAD_IO);
		}				
	} /* while(running) */
	
	return ret;
}

srt_frame* srt_read_limited_frame(srt_file *f)
{
	srt_frame *ret = NULL;
	long current_pos = -1;
	
	assert(srt_is_file_valid(f));
	assert(f->mode == SRT_READ);
	
	ret = srt_new_frame();
	
	/* If the allocation succeeded */
	if(ret != NULL)
	{
		/* First, we save the current location in file */
		current_pos = ftell(f->file);
	
		/* We read the header of the frame */
		if(srt_read_frame_header(f, ret))
		{
			/* We read the content of the frame */
			if(!srt_read_frame_content(f, ret))
			{
				srt_free_frame(ret);
				ret = NULL;
			}
		}
		else
		{
			srt_free_frame(ret);
			ret = NULL;
		}
	}

	/* 
	 * If the reading failed, we restore the last position in the 
	 * file... 
	 */
	if(ret == NULL && current_pos >= 0)
	{
		/*
		 * ...unless we have reached the end of the file and 
		 * then we raise a specific 'error'.
		 */
		if(feof(f->file))
			srt_raise_error(SRT_ERR_EOF);
		else
			fseek(f->file, current_pos, SEEK_SET);
	}
	/* If everything is OK, we finalize the frame */
	else if(ret != NULL) 
	{
		f->cursor++;
		ret->id = f->cursor;
	}

	return ret;	
}

srt_frame* srt_read_frame(srt_file* f)
{
	srt_frame *ret = NULL;
	int running = 1;
	
	assert(srt_is_file_valid(f));
	assert(f->mode == SRT_READ);
	
	/*
	 * We're using a loop to retry if there was not enough allocated memory.
	 */
	while(running)
	{
		ret = srt_read_limited_frame(f);
		
		/* On error, we allocate a larger buffer if needed */
		if(ret == NULL)
		{
			if(srt_get_last_error() == SRT_ERR_TOO_LONG)
			{
				srt_clear_error();
				running=srt_set_buffer_size(f,f->buffer_size*2);
			}
			else
			{
				running = 0;
			}
		}
		else
		{
			running = 0;
		}
	}
	
	return ret;
}

int srt_write_frame(srt_file* f, srt_frame* frame)
{
	int ret = 1;
	
	assert(srt_is_file_valid(f));
	assert(srt_is_frame_valid(frame));
	assert(f->mode == SRT_WRITE);
	
	fprintf(f->file, "%u\n"
	                 "%.2u:%.2u:%.2u,%.3u --> %.2u:%.2u:%.2u,%.3u\n"
	                 "%s"
	                 "\n", 
	                 f->cursor+1, 
	                 frame->start_time.hours,
	                 frame->start_time.min,
	                 frame->start_time.sec,
	                 frame->start_time.msec,
	                 frame->end_time.hours,
	                 frame->end_time.min,
	                 frame->end_time.sec,
	                 frame->end_time.msec,
	                 frame->text
	                 );
	
	if(ret)
	{
		frame->id = f->cursor+1;
		f->cursor++;
	}
	
	return ret;
}

int srt_is_mode_valid(const srt_mode mode)
{
	int ret = 0;
	
	switch(mode)
	{
		case SRT_READ:
		case SRT_WRITE:
			ret = 1;
			break;
		default:
			break;
	}
	
	return ret;
}

int srt_is_file_valid(const srt_file* file)
{
	int ret = 0;
	
	if(file != NULL)
	{
		if(srt_is_mode_valid(file->mode))
		{
			ret = (file->buffer != NULL && file->file != NULL && 
				file->buffer_size > 0);
		}
	}
	
	return ret;
}
