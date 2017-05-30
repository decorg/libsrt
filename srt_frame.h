#ifndef __SRT_FRAME_H
#define __SRT_FRAME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "srt_time.h"
#include <stdlib.h>

/**
 * Represents a frame with all its metadata (id, times).
 * The field id is not read while writing the frame.
 */
struct srt_frame
{
	size_t id;/*!< Id of the frame (position in the file) */
	srt_time start_time;/*!< Time to start displaying the frame */
	srt_time end_time;/*!< Time to stop displaying the frame */
	char* text;/*!< The script to display (must finish by '\n') */
};
typedef struct srt_frame srt_frame;

/**
 * Generates a new frame.
 * \return A pointer to the new allocated frame.
 */
srt_frame* srt_new_frame(void);

/**
 * Deletes a frame.
 * \param f The frame to delete. If NULL, the function does nothing.
 */
void srt_free_frame(srt_frame *f);

/**
 * Check if a frame is valid or not.
 * \param f The frame to test.
 * \return 1 if f is valid and 0 else.
 */
int srt_is_frame_valid(const srt_frame *f);

#ifdef __cplusplus
}
#endif

#endif
