#include <stdio.h>
#include <stdlib.h>

#include "registers.h"
#include "ops.h"
#include "op.h"

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

  size_t read;
  uint8_t opr;

  while ((read = fread(&opr, sizeof(uint8_t), 1, file)) != 0)
  {
    char* lhs = gtkn(file);
    char* rhs = opr != OUT ? gtkn(file) : NULL;

    printf("Executing (%d) %s - %s %s \n", opr, OP_STR[opr], lhs, rhs);

    switch (opr)
    {
    case SET:
      op_set(r, lhs, rhs);
      dbg_log_registers(r);
      break;
    
    case ADD:
      op_add(r, lhs, rhs);
      dbg_log_registers(r);
      break;

    case MUL:
      op_mul(r, lhs, rhs);
      dbg_log_registers(r);
      break;

    case SUB:
      op_sub(r, lhs, rhs);
      dbg_log_registers(r);
      break;

    case DIV:
      op_div(r, lhs, rhs);
      dbg_log_registers(r);
      break;

    case OUT:
      op_out(r, lhs);
      break;

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

    free(lhs);
    if (opr != OUT)
      free(rhs);
  }

  fclose(file);
  printf("Program done\n");

  return 0;
}
