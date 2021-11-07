#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "invm.h"
#include "invm_opcodes.h"


void vm_noop( invm_t *p )
{
#ifdef _DEBUG_
   fprintf( stdout, " [VM]  NOOP \n");
#endif
   ++( p->idx );
}


void vm_exit( invm_t *p )
{
#ifdef _DEBUG_
   fprintf( stdout, " [VM]  EXIT \n");
#endif
   p->state = STOPPED;
   ++( p->idx );
}


int invm_Opcodes_Set( invm_t *p )
{
   if( p == NULL ) return 1;

   p->opcodes[NOP] = vm_noop;
   p->opcodes[EXIT] = vm_exit;
   p->opcodes[STORE_REG] = NULL;
   p->opcodes[JUMP_TO] = NULL;
   p->opcodes[JUMP_Z] = NULL;
   p->opcodes[JUMP_NZ] = NULL;

   p->opcodes[PEEK] = NULL;
   p->opcodes[POKE] = NULL;
   p->opcodes[MEMCPY] = NULL;

   p->opcodes[STACK_PUSH] = NULL;
   p->opcodes[STACK_POP] = NULL;
   p->opcodes[STACK_RET] = NULL;
   p->opcodes[STACK_CALL] = NULL;

   p->opcodes[XOR] = NULL;
   p->opcodes[ADD] = NULL;
   p->opcodes[SUB] = NULL;
   p->opcodes[MUL] = NULL;
   p->opcodes[DIV] = NULL;
   p->opcodes[INC] = NULL;
   p->opcodes[DEC] = NULL;
   p->opcodes[AND] = NULL;
   p->opcodes[OR] = NULL;

   p->opcodes[INT_STORE] = NULL;
   p->opcodes[INT_PRINT] = NULL;
   p->opcodes[INT_TOSTRING] = NULL;
   p->opcodes[INT_RANDOM] = NULL;

   p->opcodes[STRING_STORE] = NULL;
   p->opcodes[STRING_PRINT] = NULL;
   p->opcodes[STRING_CONCAT] = NULL;
   p->opcodes[STRING_SYSTEM] = NULL;
   p->opcodes[STRING_TOINT] = NULL;

   return 0;
}

