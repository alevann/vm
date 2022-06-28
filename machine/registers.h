#ifndef __A1_REGISTERS_H_
#define __A1_REGISTERS_H_

#include <stdint.h>


#define REG(r, s) *(((uint32_t*) r) + (s[0] - 'A'))


typedef struct {
  uint32_t A;
  uint32_t B;
  uint32_t C;
  uint32_t D;
  uint32_t E;  // return register
  uint32_t F;  // flags register
} registers;

registers* reg_new ();
int reg_isreg (char*);
void dbg_log_registers (registers*);

#endif
