#ifndef __SRT_FILE_H
#define __SRT_FILE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "srt_frame.h"
#include <stdio.h>

enum srt_mode
{
	SRT_READ,
	SRT_WRITE,
	SRT_LAST
};
typedef enum srt_mode srt_mode;

struct srt_file
{
	FILE* file;
	size_t cursor;
	char* buffer;
	size_t buffer_size;
	srt_mode mode;
};
typedef struct srt_file srt_file;

/*
 * General operations.
 */
 
srt_file* srt_open(const char* file, const srt_mode mode);

srt_file* srt_open_with_size(const char* file, const srt_mode mode, 
	const size_t size);


void srt_close(srt_file *file);

int srt_set_buffer_size(srt_file* file, size_t size);

/*
 * Reading operations.
 */

int srt_read_frame_header(srt_file *f, srt_frame* frame);
int srt_read_frame_content(srt_file *f, srt_frame* frame);
srt_frame* srt_read_limited_frame(srt_file *f);
srt_frame* srt_read_frame(srt_file* f);

/*
 * Writing operations.
 */

int srt_write_frame(srt_file* f, srt_frame* frame);

/*
 * Test operations.
 */
int srt_is_mode_valid(const srt_mode mode);
int srt_is_file_valid(const srt_file* file);

#ifdef __cplusplus
}
#endif

#endif
