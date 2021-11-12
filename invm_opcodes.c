#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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


void vm_sleep( invm_t *p )
{
#ifdef _DEBUG_
   fprintf( stdout, " [VM]  SLEEP \n");
#endif
   // advance 1 byte to start with the payload; first the 1-byte register number
   p->idx += 1;

   // get the register number
   unsigned char reg = p->prog[ p->idx ];
#ifdef _DEBUG2_
   fprintf( stdout, " [DEBUG]  Got register number: \%d \n", reg );
#endif

   // check for the register being valid (we only have so many registers)
   if( reg >= p->nreg ) {
#ifdef _DEBUG2_
      fprintf( stdout, " [DEBUG]  Bad register number: \%d \n", reg );
#endif
      // set a flag on 'register error" (should be done by the compiler, but...)
      p->flags.ir = 1;

      // trigger an error
      ////// we invoke an error-handler

      return;
   }

   // check for the register storing an integer (just an FYI to the user)
   if( p->registers[ reg ].type != INTEGER ) {
#ifdef _DEBUG2_
      fprintf( stdout, " [DEBUG]  FYI, register is not storing an integer! \n");
#endif
   }

   // we copy the value from the register to a local variable
   int num_sec = p->registers[ reg ].content.i;

   // execute the instruction
   sleep( num_sec );
#ifdef _DEBUG2_
   fprintf( stdout, " [DEBUG]  Slept for %d sec \n", num_sec );
#endif

   // advance 1 byte to go to the next instruction
   p->idx += 1;
}


void vm_int_store( invm_t *p )
{
#ifdef _DEBUG_
   fprintf( stdout, " [VM]  INT_STORE \n");
#endif
   // advance 1 byte to start with the payload; first the 1-byte register number
   p->idx += 1;

   // get the register number
   unsigned char reg = p->prog[ p->idx ];
#ifdef _DEBUG2_
   fprintf( stdout, " [DEBUG]  Got register number: \%d \n", reg );
#endif
   p->idx += 1;

   // check for the register being valid
   if( reg >= p->nreg ) {
#ifdef _DEBUG2_
      fprintf( stdout, " [DEBUG]  Bad register number: \%d \n", reg );
#endif
      // set a flag on 'register error" (should be done by the compiler, but...)
      p->flags.ir = 1;

      // trigger an error
      ////// we invoke an error-handler

      return;
   }

   // if the register is storing a string, drop the referenced memory chunk
   if( p->registers[ reg ].type == STRING ) {
      if( p->registers[ reg ].content.s != NULL ) {
         free( p->registers[ reg ].content.s );
         p->registers[ reg ].content.s = NULL;
      }
   }

   // get the payload (4 bytes)
   int num=0;
   unsigned char uc;
#ifdef _DEBUG2_
      fprintf( stdout, " [DEBUG]  Int-by-segment: ");
#endif
   uc = p->prog[ p->idx ];
   num = num | ( ((int) uc) << 24 );
   p->idx += 1;
#ifdef _DEBUG2_
      fprintf( stdout, " %x ", num );
#endif
   uc = p->prog[ p->idx ];
   num = num | ( ((int) uc) << 16 );
   p->idx += 1;
#ifdef _DEBUG2_
      fprintf( stdout, " %x ", num );
#endif
   uc = p->prog[ p->idx ];
   num = num | ( ((int) uc) << 8 );
   p->idx += 1;
#ifdef _DEBUG2_
      fprintf( stdout, " %x ", num );
#endif
   uc = p->prog[ p->idx ];
   num = num | (int) uc;
   p->idx +=1;
#ifdef _DEBUG2_
      fprintf( stdout, " %x \n", num );
#endif

   // store the integer to the register
   p->registers[ reg ].content.i = num;

#ifdef _DEBUG2_
   fprintf( stdout, " [DEBUG]  Stored integer %d to register %d \n", num, reg );
#endif
}


int invm_Opcodes_Set( invm_t *p )
{
   if( p == NULL ) return 1;

   p->opcodes[NOP] = vm_noop;
   p->opcodes[EXIT] = vm_exit;
   p->opcodes[SLEEP] = vm_sleep;
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

   p->opcodes[INT_STORE] = vm_int_store;
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

