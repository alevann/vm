#include <string.h>
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

string* str_free (string* str)
{
  free(str->data);
  free(str);
}


void str_terminate (string* str)
{
  str->data[str->length] = '\0';
}


void str_adds_s (string* str, char* cptr)
{
  size_t csize = strlen(cptr);
  
  if (str->length + csize >= str->alloc)
    str_realloc(str, csize + REALLOC_SIZE);

  str_adds(str, cptr, csize);
  str_terminate(str);
}

void str_adds (string* str, char* cptr, size_t size)
{
  for (int i = 0; i < size; i++)
    str_addc(str, cptr[i]);
}

// Safe version of `str_add_c` which adds a null-terminator
// after the passed character and checks the bound of the
// string to avoid overflows, reallocates if necessary
void str_addc_s (string* str, char c)
{
  if (str->length + 2 >= str->alloc)
    str_realloc(str, REALLOC_SIZE);
  
  str_addc(str, c);
  str_terminate(str);
}

// Unsafe add character function which appends a character 
// at the end of the of the string 
// without adding a null-terminator and without checking bounds
void str_addc (string* str, char c)
{
  str->data[(str->length)++] = c;
}

// reallocates the data ptr of a string to contain more memory
void str_realloc (string* str, size_t delta)
{
  str->alloc = str->alloc + delta;
  str->data = realloc(str->data, sizeof(char) * str->alloc);
}
