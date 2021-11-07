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
   p->stack= (unsigned char *) malloc( ((size_t) p->psize) *
                                        sizeof(unsigned char) );
   if( p->registers == NULL || p->prog == NULL || p->stack == NULL ) {
      if( p->registers != NULL ) free( p->registers );
      if( p->prog != NULL ) free( p->prog );
      if( p->stack != NULL ) free( p->stack );
      return -1;
   }

   memset( p->prog, '\0', (size_t) p->psize );
   memset( p->stack, '\0', (size_t) p->ssize );

   p->sptr = 0;

   p->flags.iof = 0;
   p->flags.fpe = 0;

   int n;
   for(n=0;n<256;++n) p->opcodes[n] = NULL;

   p->id = num_vm++;
   memset( p->name, '\0', 32 );

   p->state = RUNNING;

   return 0;
}

