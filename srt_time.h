#ifndef __SRT_TIME_H
#define __SRT_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <limits.h>

#if CHAR_BIT >= 8
typedef unsigned short srt_msec;
typedef unsigned char srt_sec;
typedef unsigned char srt_min;
typedef unsigned char srt_hour;
#else
# warning You have got a strange system...
typedef unsigned short srt_msec;
typedef unsigned short srt_sec;
typedef unsigned short srt_min;
typedef unsigned short srt_hour;
#endif

struct srt_time
{
	srt_msec msec;
	srt_sec sec;
	srt_min min;
	srt_hour hours;
};
typedef struct srt_time srt_time;

struct srt_frame;

#define srt_time_gt(t1, t2) (srt_time_cmp((t1),(t2)) == 1)
#define srt_time_eq(t1, t2) (srt_time_cmp((t1),(t2)) == 0)
#define srt_time_lt(t1, t2) (srt_time_cmp((t1),(t2)) == -1)

#define srt_time_ge(t1, t2) (srt_time_gt((t1),(t2)) || srt_time_eq((t1),(t2)))
#define srt_time_le(t1, t2) (srt_time_lt((t1),(t2)) || srt_time_eq((t1),(t2)))

void srt_init_time(srt_time* t, srt_msec ms, srt_sec s, srt_min min, srt_hour h);

int srt_time_cmp(const srt_time* t1, const srt_time* t2);
int srt_add_times(const srt_time* t1, const srt_time* t2, srt_time *res);

int srt_scan_time(const char* str, srt_time *t);
int srt_read_times(const char* str, struct srt_frame *f);

int srt_is_time_valid(const srt_time* t);

#ifdef __cplusplus
}
#endif

#endif
