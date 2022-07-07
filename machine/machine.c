#include <stdio.h>
#include <stdlib.h>

#include "registers.h"
#include "ops.h"
#include "a1/op.h"

char* gtkn (FILE* file)
{
  char* out = malloc(sizeof(uint8_t) * 2);
  fread(out, sizeof(uint8_t), 1, file);
  out[1] = '\0';
  return out;
}

int main (int argc, char* argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: incorrect number of arguments");
    return 1;
  }

  char* filename = argv[1];
  printf("Executing file: %s\n", filename);

  FILE* file = fopen(filename, "rb");
  registers* r = reg_new();

  void* funcs[] = {
    NULL,
    op_set,
    op_out,
    NULL,
    op_add,
    op_mul,
    op_sub,
    op_div,
    op_cmp,
    NULL,
    op_jmp,
    op_jne
  };

  size_t read;
  uint8_t opr;

  while ((read = fread(&opr, sizeof(uint8_t), 1, file)) != 0)
  {
    uint8_t* lhs = gtkn(file);
    uint8_t* rhs = opr != OUT && opr != JMP && opr != JNE ? gtkn(file) : NULL;

    printf("Executing (%d) %s - %s %s \n", opr, OP_STR[opr], lhs, rhs);

    if (opr != ERR && opr != END && opr != LBL)
    {
      if (opr == JMP || opr == JNE)
      {
        ((void (*)(registers*, char*, FILE*))funcs[opr])(r, lhs, file);
      }
      else if (opr == OUT)
      {
        ((void (*)(registers*, char*))funcs[opr])(r, lhs);
      }
      else
      {
        ((void (*)(registers*, char*, char*))funcs[opr])(r, lhs, rhs);
        dbg_log_registers(r);
      }

      goto next;
    }

    switch (opr)
    {
    case LBL:
      long off = ftell(file);
      fprintf("Error: Label leftover from compilation at %p\n", off);
      fclose(file);
      return 1;
    
    case ERR:
      fclose(file);
      return 1;

    case END:
      fclose(file);
      return 0;
    
    default:
      printf("Error: Unknown OP (%d)\n", opr);
      return 1;
    }

    next:
    free(lhs);
    if (opr != OUT)
      free(rhs);
  }

  fclose(file);
  printf("Program done\n");

  return 0;
}
