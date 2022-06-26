#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int A;
  int B;
  int C;
  int D;
  int E;  // result register
  int F;  // flags register
} Registers;

Registers* make_registers (void)
{
  Registers* regs = malloc(sizeof(Registers));
  regs->A = 0;
  regs->B = 0;
  regs->C = 0;
  regs->D = 0;
  regs->E = 0;
  regs->F = 0;
  return regs;
}

void dbg_log_regs (Registers* regs)
{
  printf("Registers:\n");
  printf("\tA: %d", regs->A);
  printf("\tB: %d", regs->B);
  printf("\tC: %d", regs->C);
  printf("\tD: %d", regs->D);
  printf("\tE: %d", regs->E);
  printf("\tF: %d\n", regs->F);
}

#define OP_END -1
#define OP_ERR 0
#define OP_SET 1
#define OP_ADD 2
#define OP_OUT 3

#define TOKEN_MAX_SIZE 1024

char* get_next_token (FILE* f)
{
  char* token = malloc(sizeof(char) * TOKEN_MAX_SIZE);

  int i = 0;
  int c = 0;

  // Skip whitespace
  while ((c = fgetc(f)) == ' ' || c == '\n');
  if (c == EOF)
    return NULL;
  
  // After skipping whitespace c will be a letter
  token[i++] = c;


  while ((c = fgetc(f)) != EOF && c != ' ' && c != '\n')
    token[i++] = c;
  
  // Null terminate the string
  token[i] = '\0';

  return token;
}

int get_op (FILE* f)
{
  char* op = get_next_token(f);
  if (op == NULL)
    return OP_END;
  
  int opv = OP_ERR;

  if (strcmp(op, "SET") == 0)
    opv = OP_SET;
  if (strcmp(op, "ADD") == 0)
    opv = OP_ADD;
  if (strcmp(op, "OUT") == 0)
    opv = OP_OUT;

  free(op);
  return opv;
}

void skip_token (FILE* f)
{
  char* token = get_next_token(f);
  if (token != NULL)
    free(token);
}

#define REG_V(r, x) *(((int*)r) + x)
#define REG_O(r) (r[0] - 'A')

void op_set (Registers* r, FILE* f)
{
  char* rhs = get_next_token(f);
  char* lhs = get_next_token(f);

  REG_V(r, REG_O(rhs)) = atoi(lhs);

  free(rhs);
  free(lhs);
}

void op_add (Registers* r, FILE* f)
{
  char* rhs = get_next_token(f);
  char* lhs = get_next_token(f);

  r->E = REG_V(r, REG_O(rhs)) + REG_V(r, REG_O(lhs));

  free(rhs);
  free(lhs);
}

void op_out (Registers* r, FILE* f)
{
  char* tkn = get_next_token(f);

  printf("%d", REG_V(r, REG_O(tkn)));

  free(tkn);
}

int main ()
{
  Registers* r = make_registers();
  FILE* f = fopen("./add.a1", "r");
  
  int op;

  while ((op = get_op(f)) != OP_ERR && op != OP_END)
  {
    switch (op)
    {
      case OP_SET:
      {
        printf("op set\n");
        op_set(r, f);
        dbg_log_regs(r);
        break;
      }
      case OP_ADD:
      {
        printf("op add\n");
        op_add(r, f);
        dbg_log_regs(r);
        break;
      }
      case OP_OUT:
      {
        printf("op out\n");
        op_out(r, f);
        return 0;
      }
    }
  }

  fclose(f);

  return 0;
}
