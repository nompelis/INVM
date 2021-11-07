#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "invm.h"
#include "invm_opcodes.h"

int main( int argc, char *argv[] )
{
   invm_t vm;

   invm_Machine_Init( &vm );
   invm_Opcodes_Set( &vm );
   invm_Machine_DumpRegisters( &vm );

   vm.prog[0] = 0x02;   // this is presently the "SLEEP" instruction
   vm.prog[2] = 0x01;   // this is the "EXIT" instruction
   invm_Machine_Run( &vm, 4 );
   printf("Completed short run\n");

// vm.state = RUNNING;   // get it back to a running state...
// vm.idx = 0;           // ...and have it start from the start
// invm_Machine_Run( &vm, 0 );
// printf("Completed long run\n");

   invm_Machine_Clear( &vm );

   return 0;
}

