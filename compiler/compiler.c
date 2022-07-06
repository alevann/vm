#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include "token.h"
#include "a1/string.h"
#include "a1/op.h"
#include "a1/hashmap.h"

#define VERSION "0.0.1"
#define CMPNAME "A1 Compiler"

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

/**
 * Returns the name of the output file given an input
 * As of now it basically just replaces .a1 with .ax
 * Also discards path information, the file is written in the CWD
 */
char* outfn (char* path)
{
  char* name = basename(path);
  size_t len = strlen(name);

  char* filename = malloc(sizeof(char) * len);
  strncpy(filename, name, len - 3);
  strcpy(filename + (len - 3), ".ax");

  return filename;
}

/**
 * Opens a file and tokenizes it
 * 
 * @param tokens is required to have enough space
 *  to store all the tokens in the file
 */
void opntknize (char* file, token* tokens[], size_t* token_count)
{
  FILE* in = fopen(file, "r");
  tokenize(in, tokens, token_count);
  fclose(in);

  dbg_log_tkns(tokens, *token_count);
}

/**
 * Writes two tokens to the passed file
 */
void wrttok (FILE* file, token* tokens[], int* cursor)
{
  token* tkn = tokens[++(*cursor)];
  fwrite(tkn->str->data, sizeof(char), tkn->str->length, file);
}

int wrttoks (FILE* file, token* tokens[], size_t tokenc)
{
  hmap* labels = hmp_new(sizeof(uint8_t), sizeof(string*), NULL);
  hmap* missed = hmp_new(sizeof(uint8_t), sizeof(string*), NULL);
  printf("Looping over %d tokens\n", tokenc);

  for (int i = 0; i < tokenc; i++)
  {
    token* tkn = tokens[i];

    // Get the operation and write it in the file 
    uint8_t op = gop(tkn);
    printf("Current OP: %s\n", OP_STR[op]);
    // Labels are just offsets in the file so they aren't written anywhere
    if (op != LBL)
    {
      printf("Writing op...\n");
      fwrite(&op, sizeof(uint8_t), 1, file);
    }

    switch (op)
    {
    case SET: case ADD: case MUL: case DIV: case SUB: case CMP:
      printf("Writing args...\n");
      wrttok(file, tokens, &i);
      wrttok(file, tokens, &i);
      break;
    
    case OUT:
      printf("Writing outs...\n");
      wrttok(file, tokens, &i);
      break;
    
    // Define a label by adding it to the labels hashmap
    case LBL:
      printf("Writing labl...\n");
      token* lbl = tokens[++i];
      uint8_t pos = ftell(file);
      hmp_set(labels, lbl->str->data, pos);
      break;
    
    // Write the label id
    case JMP:
    case JNE:
      printf("Writing jmps...\n");
      token* tkn = tokens[++i];
      uint8_t off = hmp_get(labels, tkn->str->data);
      if (!off)
      {
        // if the label is not defined:
        // check to see if this is the first occurence
        // if list is not null, it isn't
        struct list* list = hmp_get(missed, tkn->str->data);
        if (!list)
        {
          // initialize the list since there could be more
          // than one mention before it's defined
          list = list_new();
          hmp_set(missed, tkn->str->data, list);
        }
        // Add the current position in the file to the list
        // so that later it can be updated
        uint8_t inf = ftell(file);
        list_add(list, inf);
        printf("placeholder in %d\n", inf);
        // Write a placeholder value
        off = 0;
      }
      fwrite(&off, sizeof(uint8_t), 1, file);
      break;


    case ERR:
    default:
      printf("Writing err...\n");
      fprintf(stderr, "Error while writing tokens to file: %s", dbg_display_tkn(tkn));
      hmp_free(labels);
      return 1;
    }
  }

  if (missed->keys->size == 0)
    goto ret;

  // missed: hashmap<string, list<long>>
  // missed->keys->head: string
  // missed->keys[k]: list<long>
  // usages: list<long>


  // Loop over all missing labels
  struct node* head = missed->keys->head;
  struct node* foll;
  printf("Starting missed label process with %s\n", head->data);
  do {
    printf("Checking if %s has been defined\n", head->data);
    // Check if the label has been defined
    uint8_t contains = hmp_contains(labels, head->data, streq);
    if (!contains) {
      printf("%s has not been defined!\n", head->data);
      continue;
    }
    printf("%s has been defined\n", head->data);

    // Get the offset of the label (i.e. where to jump in the file)
    uint8_t off = hmp_get(labels, head->data);

    // Loop over all usages of the label in the file
    struct list* usages = hmp_get(missed, head->data);
    printf("There's %d usages of %s\n", usages->size, head->data);
    struct node* cursor = usages->head;
    printf("FIrst usage of %s is at %d\n", head->data, cursor->data);
    struct node* next;
    do {
      // Move where the label was used
      fseek(file, cursor->data, SEEK_SET);
      printf("Moved at correct offset!\n");
      // Write the label's offset
      fwrite(&off, sizeof(uint8_t), 1, file);
      printf("Usage updated!\n");
      // Remove the usage from the list
      next = cursor->next;
      list_del(usages, cursor->data, identity);
      printf("Usage removed! next up: %p\n", next);
    } while ((cursor = next));

    // After having updated all the usages with the right value
    // remove the label from the hashmap
    printf("Deleting remnants...\n");
    foll = head->next;
    hmp_del(missed, head->data);
    printf("Updated keys, %s has been resolved correctly\n", head->data);
  } while ((head = foll));

  if (missed->keys->size != 0)
    fprintf(stderr, "Something went wrong, %d labels left\n", missed->keys->size);

  ret:
  hmp_free(labels);
  hmp_free(missed);
  return 0;
}

int main (int argc, char* argv [])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: invalid argument count.");
    return 1;
  }

  char* file = argv[1];
  printf("%s v%s >> Compiling file: %s...\n", CMPNAME, VERSION, file);

  token* tokens[1024];
  size_t tokenc = 0;
  opntknize(file, &tokens, &tokenc);

  char* filename = outfn(file);
  printf("Generating file: %s...\n", filename);

  // An A1 file is structured as follows
  //  <op> <a1> <a1?>
  // Basically, an instruction followed by one or two arguments

  FILE* outfile = fopen(filename, "wb");
  int cmpres = wrttoks(outfile, tokens, tokenc);
  fclose(outfile);

  return cmpres;
}