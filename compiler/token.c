#include "token.h"
#include <string.h>
#include <ctype.h>


token* new_token (string* str, enum tokentype type)
{
  token* tkn = malloc(sizeof(token));
  tkn->str = str;
  tkn->type = type;
  return tkn;
}

void dbg_log_tkns (token* tkns [], size_t size)
{
  printf("Tokens:\tamount %d\n", size);
  for (int i = 0; i < size; i++)
    printf("\t%s\n", dbg_display_tkn(tkns[i]));
}

char* dbg_display_tkn (token* tkn)
{
  char* types [] = { "UNK", "OPR", "LIT", "REG" };
  
  // hope to never overflow, pc battery is almost dead
  char* out = malloc(sizeof(char) * 2048);
  sprintf(out, "Token\t%s\t(%s)", tkn->str->data, types[tkn->type]);
  return out;
}

// Returns true if the passed string matches any
// of the supported operations
int isop (string* str)
{
  if (str->length < 3)
    return 0;
  
  char* ops[5] = { "ERR", "SET", "ADD", "END", "OUT" };

  for (int i = 0; i < 5; i++)
  {
    if (strcmp(str->data, ops[i]) == 0)
      return 1;
  }
  return 0;
}

int isnum (string* str)
{
  for (int i = 0; i < str->length; i++)
  {
    if (!isdigit(str->data[i]))
      return 0;
  }
  return 1;
}

void skip_ws (FILE* f)
{
  char c;
  while (isspace(c = fgetc(f)));
  ungetc(c, f);
}

char fpeek (FILE* f)
{
  char c = fgetc(f);
  ungetc(c, f);
  return c;
}

token* gtkn (FILE* file)
{
  // Skip any whitespace
  skip_ws(file);

  if (fpeek(file) == EOF)
    return NULL;
  

  string* str = str_new(TOKEN_STR_MAX_SIZE);

  int i = 0;
  int c = 0;

  while (isalnum(c = fgetc(file)))
    str_add_c_s(str, c);
  

  enum tokentype type = UNK;

  // Registers are single letters ranging from A to F (65-70 in ASCII)
  if (str->length == 1 && str->data[0] - 'A' <= 5 && str->data[0] - 'A' >= 0)
    type = REG;
  else if (isop(str))
    type = OPR;
  else if (isnum(str))
    type = LIT;
  
  return new_token(str, type);
}

void tokenize (FILE* file, token* tkns [], size_t* count)
{
  token* tkn;
  
  while ((tkn = gtkn(file)) != NULL)
  {
    tkns[(*count)++] = tkn;
  }
}
