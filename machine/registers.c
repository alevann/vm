#include "registers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

registers* reg_new ()
{
  registers* r = malloc(sizeof(registers));
  r->A = 0;
  r->B = 0;
  r->C = 0;
  r->D = 0;
  r->E = 0;
  r->F = 0;
  return r;
}

void print_bin(uint32_t value)
{
  for (int i = sizeof(uint32_t) * 7; i >= 0; i--)
    printf("%d", (value & (1 << i)) >> i );
}

void dbg_log_registers (registers* r)
{
  printf("Registers:\n");
  printf("\tA: %d", r->A);
  printf("\tB: %d", r->B);
  printf("\tC: %d", r->C);
  printf("\tD: %d", r->D);
  printf("\tE: %d", r->E);
  printf("\tF: "); print_bin(r->F);
  printf("\n");
}

int reg_isreg (char* str)
{
  if (strlen(str) > 1)
    return 0;
  
  int8_t x = str[0] - 'A';
  return x >= 0 && x <= 5;
}
