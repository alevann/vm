#ifndef __A1_OP_H_
#define __A1_OP_H_

enum Op
{
  ERR,
  SET,
  OUT,
  END,
  ADD,
  MUL,
  SUB,
  DIV,
  CMP
};

extern char* OP_STR [9];

#endif
