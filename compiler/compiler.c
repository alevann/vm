#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include "token.h"
#include "op.h"
#include "hashmap.h"


uint8_t gop (token* tkn)
{
  if (strcmp(tkn->str->data, "SET") == 0)
    return SET;
  if (strcmp(tkn->str->data, "ADD") == 0)
    return ADD;
  if (strcmp(tkn->str->data, "OUT") == 0)
    return OUT;
  if (strcmp(tkn->str->data, "MUL") == 0)
    return MUL;
  if (strcmp(tkn->str->data, "DIV") == 0)
    return DIV;
  if (strcmp(tkn->str->data, "SUB") == 0)
    return SUB;
  if (strcmp(tkn->str->data, "CMP") == 0)
    return CMP;
  if (strcmp(tkn->str->data, "LBL") == 0)
    return LBL;
  if (strcmp(tkn->str->data, "JMP") == 0)
    return JMP;
  if (strcmp(tkn->str->data, "JNE") == 0)
    return JNE;
  
  return ERR;
}


int main (int argc, char* argv [])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: invalid argument count.");
    return 1;
  }


  char* file = argv[1];
  printf("Compiling file: %s...\n", file);

  FILE* in = fopen(file, "r");
  
  token* tokens[1024];
  size_t token_count = 0;
  tokenize(in, tokens, &token_count);

  fclose(in);

  dbg_log_tkns(tokens, token_count);

  // find the name of the output file 
  char* name = basename(file);
  size_t len = strlen(name);

  char* filename = malloc(sizeof(char) * len);
  strncpy(filename, name, len - 3);
  strcpy(filename + (len - 3), ".ax");

  printf("Generating file: %s...\n", filename);

  // An A1 file is structured as follows
  //  <op> <a1> <a1?>
  // Basically, an instruction followed by one or two arguments

  FILE* ot = fopen(filename, "wb");
  hmap* labels = hmp_new(sizeof(uint8_t), sizeof(string*), NULL);
  printf("Looping now\n");

  for (int i = 0; i < token_count; i++)
  {
    token* tkn = tokens[i];

    // Get the operation and write it in the file 
    uint8_t op = gop(tkn);
    // Labels are just offsets in the file so they aren't written anywhere
    if (op != LBL)
    {
      fwrite(&op, sizeof(uint8_t), 1, ot);
    }

    switch (op)
    {
    case SET:
    case ADD:
    case MUL:
    case DIV:
    case SUB:
    case CMP:
      // Get the two operands and write them to the file
      token* lhs = tokens[++i];
      token* rhs = tokens[++i];
      
      fwrite(lhs->str->data, sizeof(char), lhs->str->length, ot);
      fwrite(rhs->str->data, sizeof(char), rhs->str->length, ot);
      break;
    
    case OUT:
      token* arg = tokens[++i];

      fwrite(arg->str->data, sizeof(char), arg->str->length, ot);
      break;
    
    // Define a label by adding it to the labels hashmap
    case LBL:
      token* lbl = tokens[++i];
      uint8_t pos = ftell(ot);
      hmp_set(labels, lbl->str->data, pos);
      break;
    
    // Write the label id
    case JMP:
    case JNE:
      token* tkn = tokens[++i];
      uint8_t off = hmp_get(labels, tkn->str->data);
      if (!off)
      {
        fprintf(stderr, "Error: label %s used before declaration\n", tkn->str->data);
        return 1;
      }
      else
      {
        fwrite(&off, sizeof(uint8_t), 1, ot);
      }
      break;


    case ERR:
    default:
      fprintf(stderr, "Error while writing tokens to file: %s", dbg_display_tkn(tkn));
      fclose(ot);
      return 1;
    }
  }

  fclose(ot);
  hmp_free(labels);

  return 0;
}