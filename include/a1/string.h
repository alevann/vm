#ifndef __A1_STRING_H_
#define __A1_STRING_H_


#include <stdlib.h>
#include <string.h>

// Size delta added to a string when reallocating
#define REALLOC_SIZE 64

/**
 * Representation of a string because I thought
 * it might make things easier. I'm no-where near
 * smart enough to handle Unicode so obviously
 * this string type only (barely) supports ASCII.
 * 
 * `string`s should only be modified through the
 * `str_` methods declared in this header file.
 * 
 * @param data underlying buffer of the string
 * @param length the amount of characters in the string,
 *  does not include the null-terminator
 * @param alloc the size of the allocation of the underlying buffer,
 *  needed for housekeeping purposes
 */
typedef struct {
  char* data;
  size_t length;
  size_t alloc;
} string;


/**
 * Returns a new string with some amount of memory preallocated.
 * 
 * @param prealloc the amount of memory to preallocate
 *  `(sizeof(char) * prealloc)`
 */
string* str_new (size_t prealloc);

/**
 * Frees a string and its underlying buffer
 */
string* str_free (string*);


/**
 * Safe version of `str_adds`.
 * 
 * Appends a string at the of another and guarantees
 * it's terminated. The function may reallocate the string
 * if needed.
 */
void str_adds_s (string*, char*);

/**
 * Unsafely adds a string at the end of another.
 * No bounds-checking is performed and 
 * the string is not null-terminated.
 */
void str_adds (string*, char*, size_t);


/**
 * Safe version of `str_addc`.
 * 
 * Appends a character at the end of the string and
 * guarantees it is followed by a null-terminator,
 * further the function checks if the underlying buffer
 * would overflow adding these characters and
 * reallocates the string if needed.
 */
void str_addc_s (string*, char);

/**
 * Unsafely adds a character at the end of a string.
 * No bounds-checking is performed and the string 
 * is not null terminated.
 */
void str_addc (string*, char);

/**
 * Adds a null-terminator at the end of a string
 */
void str_terminate (string*);

/**
 * Reallocates the underlying buffer to be `str->alloc + delta`
 * allowing both shrinking and expanding of the string.
 */
void str_realloc (string*, size_t);


#endif
