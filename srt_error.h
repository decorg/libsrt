#ifndef __SRT_ERROR_H
#define __SRT_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NDEBUG
# include <stdio.h>
# define srt_die(str) do { fprintf(stderr, "%s", str); abort(); } while(0)
#else
# define srt_die(str)
#endif

/**
 * Represents an error code.
 */
enum srt_error
{
	SRT_ERR_NONE, /*!< No errors */
	SRT_ERR_BAD_ALLOC, /*!< (Re)allocation failed */
	SRT_ERR_BAD_IO,/*!< Input/output error */
	SRT_ERR_EOF,/*!< End of file reached */
	SRT_ERR_TOO_LONG,/*!< The buffer isn't enoughly sized */
	SRT_ERR_TIME_OVERFLOW,/*!< Time overflow (during an addition for ex) */
	SRT_ERR_BAD_TIME_FORMAT, /*!< Cannot read time */
	SRT_ERR_UNKNOWN,/*!< Unknown error */
	SRT_ERR_LAST/*!< Total number of possible errors */
};
typedef enum srt_error srt_error;

/**
 * Clears the last error. Does nothing if there was no error.
 */
void srt_clear_error(void);

/**
 * Raises an error. Deletes the previous error (if any).
 * \param error The error to raise.
 */
void srt_raise_error(const srt_error error);

/**
 * Returns the last error that occured.
 * \return The error.
 */
srt_error srt_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif
