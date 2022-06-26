#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string.h"


// Creates a new string preallocating `prealloc` amount of memory
string* str_new (size_t prealloc)
{
  string* str = malloc(sizeof(string));
  str->data = malloc(sizeof(char) * prealloc);
  str->length = 0;
  str->alloc = prealloc;

  return str;
}

// Safe version of `str_add_c` which adds a null-terminator
// after the passed character and checks the bound of the
// string to avoid overflows, reallocates if necessary
void str_add_c_s (string* str, char c)
{
  // Check if oob with 2 more chars (passed char and null-terminator)
  if (str->length + 2 >= str->alloc)
    str_realloc(str, REALLOC_SIZE);

  str_add_c(str, c);
  str->data[str->length + 1] = '\0';
}

// Unsafe add character function which appends a character 
// at the end of the of the string 
// without adding a null-terminator and without checking bounds
void str_add_c (string* str, char c)
{
  str->data[(str->length)++] = c;
}

// reallocates the data ptr of a string to contain more memory
void str_realloc (string* str, size_t delta)
{
  str->alloc = str->alloc + delta;
  str->data = realloc(str->data, sizeof(char) * str->alloc);
}
