#include "registers.h"
#include <stdio.h>
#include <stdlib.h>

// Returns either the value of the register if the string
// is a register or the number it represents (hoping) it
// represents a number lmao
uint32_t reg_or_literal (registers* r, char* str)
{
  if (reg_isreg(str))
    return REG(r, str);
  else
    return atoi(str);
}

void op_err ()
{
  fprintf(stderr, "Reached error instruction\n");
  exit(1);
}

void op_end ()
{
  printf("Reached end instruction\n");
  exit(0);
}

void op_set (registers* r, char* lhs, char* rhs)
{
  REG(r, lhs) = reg_or_literal(r, rhs);
}

void op_add (registers* r, char* lhs, char* rhs)
{
  REG(r, lhs) = REG(r, lhs) + reg_or_literal(r, rhs);
}

void op_sub (registers* r, char* lhs, char* rhs)
{
  REG(r, lhs) = REG(r, lhs) - reg_or_literal(r, rhs);
}

void op_mul (registers* r, char* lhs, char* rhs)
{
  REG(r, lhs) = REG(r, lhs) * reg_or_literal(r, rhs);
}

void op_div (registers* r, char* lhs, char* rhs)
{
  REG(r, lhs) = REG(r, lhs) / reg_or_literal(r, rhs);
}

void op_out (registers* r, char* lhs)
{
  printf("%d\n", REG(r, lhs));
}

void op_cmp (registers* r, char* lhs, char* rhs)
{
  uint8_t bit = REG(r, lhs) == reg_or_literal(r, rhs);
  r->F ^= (-(unsigned long)bit ^ r->F) & (1UL << 0);
}

void op_jmp (registers* r, char* lhs, FILE* f)
{
  if ((r->F >> 0) & 1UL)
    fseek(f, (uint8_t)*lhs, SEEK_SET);
}

void op_jne (registers* r, char* lhs, FILE* f)
{
  if (!((r->F >> 0) & 1UL))
    fseek(f, (uint8_t)*lhs, SEEK_SET);
}
