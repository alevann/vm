#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <libgen.h>
#include "op.h"

#define TOKEN_MAX_SIZE 1024


struct token {
  char* string;
  int length;
};

struct token* new_token (char* string, int length)
{
  struct token* token = malloc(sizeof(struct token));
  token->string = string;
  token->length = length;

  return token;
}


// SKips whitespace in the passed file pointer
// Guarantees the next call to fegtc(file)
// returns a alphanumeric character
void skip_ws (FILE* file)
{
  char c;
  while (isspace(c = fgetc(file)));
  ungetc(c, file);
}

char fpeek (FILE* file)
{
  char c = fgetc(file);
  ungetc(c, file);
  return c;
}

// Returns the next token in the file
// A token is a string of alphanumeric numbers
struct token* gtkn (FILE* file)
{
  // Skip any preceding whitespace
  skip_ws(file);

  // Check if we reached the end of the file
  if (fpeek(file) == EOF)
    return NULL;

  // Allocate a string which will hold our token
  char* token = malloc(sizeof(char) * TOKEN_MAX_SIZE);

  int i = 0;
  int c = 0;

  // Compose our token - 
  // Note: after this loop c will hold a non-alphanumeric character it's ok 
  // to ignore it since we don't support any non-alphanumeric character yet
  while (isalnum(c = fgetc(file)))
    token[i++] = c;
  token[i++] = '\0';

  return new_token(token, i);
}

void tokenize (FILE* file, struct token* tokens[], size_t* count)
{
  struct token* token;

  while ((token = gtkn(file)) != NULL)
  {
    tokens[(*count)++] = token;
  }
}


uint8_t gop (struct token* token)
{
  if (strcmp(token->string, "SET") == 0)
    return SET;
  if (strcmp(token->string, "ADD") == 0)
    return ADD;
  if (strcmp(token->string, "OUT") == 0)
    return OUT;
  
  return ERR;
}


int main (int argc, char* argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: invalid argument count.");
    return 1;
  }

  char* file = argv[1];
  printf("Compiling %s\n", file);

  FILE* in = fopen(file, "r");
  
  struct token* tokens[1024];
  size_t token_count = 0;
  tokenize(in, tokens, &token_count);

  fclose(in);


  // find the name of the output file 
  char* name = basename(file);
  size_t len = strlen(name);

  char* filename = malloc(sizeof(char) * len);
  strncpy(filename, name, len - 3);
  strcpy(filename + (len - 3), ".ax");

  printf("Generating file: %s\n", filename);


  // An A1 file is structured as follows
  //  <op> <a1> <a1?>
  // Basically, an instruction followed by one or two arguments

  FILE* ot = fopen(filename, "wb");

  for (int i = 0; i < token_count; i++)
  {
    struct token* token = tokens[i];

    // Get the operation and write it in the file 
    uint8_t op = gop(token);
    fwrite(&op, sizeof(uint8_t), 1, ot);

    switch (op)
    {
    case SET:
    case ADD:
      // Get the two operands and write them to the file
      struct token* lhs = tokens[++i];
      struct token* rhs = tokens[++i];
      
      fwrite(lhs->string, sizeof(char), lhs->length, ot);
      fwrite(rhs->string, sizeof(char), rhs->length, ot);
      break;
    case OUT:
      struct token* tkn = tokens[++i];

      fwrite(tkn->string, sizeof(char), tkn->length, ot);
      break;
    case ERR:
    default:
      fprintf(stderr, "Error while writing tokens to file: %s", token->string);
      fclose(ot);
      return 1;
    }
  }

  // Remember to close the file
  fclose(ot);

  return 0;
}