#ifndef __A1_TOKEN_H_
#define __A1_TOKEN_H_

#include <stdio.h>
#include <stdint.h>
#include "string.h"


#define TOKEN_STR_MAX_SIZE 64

enum tokentype {
  UNK,
  OPR,
  LIT,
  REG
};

typedef struct {
  string* str; 
  enum tokentype type;
} token;

token* new_token(string*, enum tokentype);

void tokenize(FILE*, token* [], size_t*);

void dbg_log_tkns (token* [], size_t);
char* dbg_display_tkn (token*);

#endif
