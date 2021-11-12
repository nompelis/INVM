#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "invm.h"
#include "invm_opcodes.h"


int invm_Machine_Init( invm_t *p )
{
   static unsigned int num_vm=0;

   if( p == NULL ) return 1;

   p->nreg = 10;
   p->registers = (vmreg_t*) malloc( ((size_t) p->nreg) *
                                        sizeof(vmreg_t) );
   p->psize = (size_t) (0x0001 << 12);     // 4096 bytes
   p->prog = (unsigned char *) malloc( ((size_t) p->psize) *
                                        sizeof(unsigned char) );
   p->ssize = (size_t) (0x0001 << 10);     // 1024 bytes
   p->stack = (unsigned char *) malloc( ((size_t) p->psize) *
                                        sizeof(unsigned char) );
   if( p->registers == NULL || p->prog == NULL || p->stack == NULL ) {
      if( p->registers != NULL ) free( p->registers );
      if( p->prog != NULL ) free( p->prog );
      if( p->stack != NULL ) free( p->stack );
#ifdef _DEBUG_
      fprintf( stdout, " [INVM]  Error: memory allocation for VM \n");
#endif
      return -1;
#ifdef _DEBUG_
   } else {
      fprintf( stdout, " [INVM]  Allocated VM data: \n %s%d%s%d%s%d \n",
               "  Registers: ", p->nreg,
               "  Prog bytes: ", (int) p->psize,
               "  Stack bytes: ", (int) p->ssize );
#endif
   }

   int n;
   for(n=0;n<p->nreg;++n) {
      p->registers[n].type = INTEGER;     // make this a default
      // the remaining contents are cleaned-up
      p->registers[n].content.i = 0;
      p->registers[n].content.s = NULL;
   }
#ifdef _DEBUG_
   for(n=0;n<p->nreg;++n) p->registers[n].type = STRING;
#endif

   memset( p->prog, '\0', (size_t) p->psize );
   memset( p->stack, '\0', (size_t) p->ssize );

   p->sptr = 0;

   p->flags.ir = 0;
   p->flags.iof = 0;
   p->flags.fpe = 0;

   for(n=0;n<256;++n) p->opcodes[n] = NULL;

   p->id = num_vm++;
   memset( p->name, '\0', 32 );

   p->state = RUNNING;

   return 0;
}


int invm_Machine_DumpRegisters( invm_t *p )
{
   if( p == NULL ) return 1;

   int n;
   for(n=0;n<p->nreg;++n) {
      if( p->registers[n].type == INTEGER ) {
         fprintf( stdout, " Register %.2d  Int %d10d   Hex %8x \n",
                  n, p->registers[n].content.i,
                     p->registers[n].content.i );
      } else
      if( p->registers[n].type == STRING ) {
         fprintf( stdout, " Register %.2d  String: \"%s\" \n",
                  n, p->registers[n].content.s );
      } else {
         // this should never happen
      }
   }

   return 0;
}


int invm_Machine_Clear( invm_t *p )
{
   if( p == NULL ) return 1;
#ifdef _DEBUG_
   fprintf( stdout, " [INVM]  Clearing VM struct \n");
#endif

   free( p->registers );
   free( p->prog );
   free( p->stack );
   int n;
   for(n=0;n<256;++n) p->opcodes[n] = NULL;
   p->nreg = 0;
   p->psize = 0;
   p->ssize = 0;
   p->id = 0;
   memset( p->name, '\0', 32 );
   p->state = UNREADY;

   return 0;
}


int invm_Machine_Run( invm_t *p, int num_steps )
{
   if( p == NULL ) return -1;

   int step=0;
   unsigned char opcode;

   // initial step
   p->idx = 0;
   while( p->state == RUNNING ) {

      // stop the machine if we reached the end of the program
      if( p->idx >= (int) p->psize ) {
         p->opcodes[EXIT]( p );
      } else {
         opcode = p->prog[ p->idx ];   // get instruction from program
         // execute if this is a valid instruction
         if( p->opcodes[opcode] != NULL ) {
            p->opcodes[opcode]( p );
         } else {
            fprintf( stdout, " [INVM]  Null (non-existent) instruction \n");
         }
      }

      // increase step
      ++step;
      // check if we need to stop
      if( num_steps > 0 ) if( step == num_steps ) p->state = STOPPED;
   }

   return 0;
}

