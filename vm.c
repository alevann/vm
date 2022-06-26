#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "op.h"

typedef struct {
  int A;
  int B;
  int C;
  int D;
  int E;  // result register
  int F;  // flags register
} regis;

regis* new_registers (void)
{
  regis* regs = malloc(sizeof(regis));
  regs->A = 0;
  regs->B = 0;
  regs->C = 0;
  regs->D = 0;
  regs->E = 0;
  regs->F = 0;
  return regs;
}
void dbg_log_regs (regis* regs)
{
  printf("Registers:\n");
  printf("\tA: %d", regs->A);
  printf("\tB: %d", regs->B);
  printf("\tC: %d", regs->C);
  printf("\tD: %d", regs->D);
  printf("\tE: %d", regs->E);
  printf("\tF: %d\n", regs->F);
}


void skip_bytes (FILE* file, size_t bytes)
{
  fseek(file, bytes, SEEK_CUR);
}

char* gtkn (FILE* file)
{
  char* out = malloc(sizeof(uint8_t) * 1);
  fread(out, sizeof(uint8_t), 1, file);
  return out;
}

#define REG_V(r, x) *(((int*)r) + x)
#define REG_O(r) (r[0] - 'A')

void op_set (regis* r, FILE* f)
{
  char* lhs = gtkn(f);
  char* rhs = gtkn(f);

  REG_V(r, REG_O(lhs)) = atoi(rhs);

  free(lhs);
  free(rhs);
}

void op_add (regis* r, FILE* f)
{
  char* lhs = gtkn(f);
  char* rhs = gtkn(f);

  r->E = REG_V(r, REG_O(lhs)) + REG_V(r, REG_O(rhs));

  free(lhs);
  free(rhs);
}

void op_out (regis* r, FILE* f)
{
  char* tkn = gtkn(f);

  printf("%d\n", REG_V(r, REG_O(tkn)));

  free(tkn);
}

int main (int argc, char* argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "Error: incorrect number of arguments");
    return 1;
  }

  char* filename = argv[1];
  printf("Loading file: %s\n", filename);

  FILE* file = fopen(filename, "rb");
  regis* regs = new_registers();
  size_t read;
  uint8_t op;

  while ((read = fread(&op, sizeof(uint8_t), 1, file)) != 0)
  {
    switch (op)
    {
    case SET:
      printf("Reached SET: ");
      op_set(regs, file);
      dbg_log_regs(regs);
      break;
    case ADD:
      printf("Reached ADD\n");
      op_add(regs, file);
      dbg_log_regs(regs);
      break;
    case OUT:
      printf("Reached OUT\n");
      op_out(regs, file);
      break;
    
    case ERR:
      printf("Reached ERR instruction\n");
      fclose(file);
      return 1;
    
    case END:
      printf("Reached program END\n");
      fclose(file);
      return 0;
    
    default:
      fprintf(stderr, "Error: unkown OP %d\n", op);
      fclose(file);
      return 1;
    }
  }

  fclose(file);
  printf("Program done\n");

  return 0;
}