#ifndef __SRT_UTILS_H
#define __SRT_UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Indicates if the given string is a number or not.
 * \param str The string to check.
 * \return 1 if the string is a number and 0 else. 
 */
int srt_is_number(const char* str);

/**
 * Appends a string to another one after having reallocated enough memory.
 * \param str The basic string.
 * \param str The string to add.
 * \return The address of the new string on success and NULL else.
 */
char* srt_append_str(char* str, const char* add);

/**
 * Duplicates a given string.
 * \param str The string to copy.
 * \return The copy of the given string.
 */
char* srt_strdup(const char* str);

/**
 * Indicates if a string corresponds to an end of a block.
 * \param str The string to check.
 * \return 1 if the string is a EOB and 0 else.
 */
int srt_is_end_of_block(const char* str);

/**
 * Convert a character into digit
 * \param c The character to convert.
 * \return The digit on success or (-1) on error.
 */
int srt_char_to_digit(const char c);

#ifdef __cplusplus
}
#endif

#endif
