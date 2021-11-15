#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "invm.h"
#include "invm_opcodes.h"


void integer_to_BigEndianUchar( int num, unsigned char s[] )
{
   // Set some convenience bit-shifted ints so that we can make a Big Endian
   // decomposition of a 32-bit integer
   int i3,i2,i1,i0;
   i0 = 0xff;
   i1 = i0 << 8;
   i2 = i1 << 8;
   i3 = i2 << 8;

#ifdef _DEBUG2_
   fprintf( stdout, "Bit-shifted 32-bit int: %x %x %x %x \n", i3, i2, i1, i0 );
#endif
   s[0] = (unsigned char) ((num & i3) >> 24 );
   s[1] = (unsigned char) ((num & i2) >> 16 );
   s[2] = (unsigned char) ((num & i1) >> 8 );
   s[3] = (unsigned char) ((num & i0) >> 0 );
#ifdef _DEBUG2_
   fprintf( stdout, "Big endian stored (as uchar): %x %x %x %x \n",
           s[0], s[1], s[2], s[3] );
   fprintf( stdout, "Integer in program: \"%x\" \n", num );
#endif
}


void make_program( unsigned char* prog )
{
   size_t i=0;
   int num=0;
// char string[] = "This is a string!";


   // inject a statement to store an integer to a register
   prog[i] = (unsigned char) INT_STORE;    // the instruction
   i = i + 1;
   prog[i] = (unsigned char) 3;            // the register at slot "3" (4th)
   i = i + 1;
   num = 2;       // payload, the number "2" (will be used for a 2 second sleep)
   integer_to_BigEndianUchar( num, &( prog[i] ) );
   i = i + 4;
#ifdef _DEBUG_
   printf("Big endian stored (as uchar): %x %x %x %x \n",
          prog[i-4], prog[i-3], prog[i-2], prog[i-1] );
   printf("Integer in program: \"%d\" \n", num );
#endif

   // inject a sleep statement; uses as payload the integer stored in a register
   prog[i] = (unsigned char) SLEEP;
   i = i + 1;
   prog[i] = (unsigned char) 3;            // the register number "3"
   i = i + 1;

   // add termination
   prog[i] = (unsigned char) EXIT;

   printf("Total number of bytes in program: %d \n", (int) i );
}


int main( int argc, char *argv[] )
{
   invm_t vm;

   invm_Machine_Init( &vm );
   invm_Opcodes_Set( &vm );
   invm_Machine_DumpRegisters( &vm );

   vm.prog[0] = 0x02;   // this is presently the "SLEEP" instruction
   vm.prog[1] =    2;   // this is presently the number "2" for register at 2
   vm.prog[2] = 0x01;   // this is the "EXIT" instruction
   printf(" ******* Execution ******* \n");
   invm_Machine_Run( &vm, 4 );
   printf("Completed short run\n");

   vm.state = RUNNING;   // get it back to a running state...
   vm.idx = 0;           // ...and have it start from the start
   invm_Machine_Run( &vm, 0 );
   printf("Completed long run\n");

   invm_Machine_Clear( &vm );

//-- test of more complex programs
   printf(" ******* More complex programs ******* \n");
   invm_Machine_Init( &vm );
   invm_Opcodes_Set( &vm );
   invm_Machine_DumpRegisters( &vm );
   (void) make_program( vm.prog );
   printf(" ******* Execution ******* \n");
   invm_Machine_Run( &vm, 4 );
   printf("Completed second run\n");

   invm_Machine_Clear( &vm );

   return 0;
}

