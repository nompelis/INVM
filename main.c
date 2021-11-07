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
   invm_Machine_Clear( &vm );

   return 0;
}

