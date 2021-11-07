
#ifndef _INVM_OPCODES_H_
#define _INVM_OPCODES_H_

//
// The OP codes of IN's VM (which is similar to ssk's VM!)
//
enum opcode_values {
   NOP = 0,
   EXIT,
   STORE_REG,
   JUMP_TO,
   JUMP_Z,
   JUMP_NZ,

   PEEK = 0x10,
   POKE,
   MEMCPY,

   STACK_PUSH = 0x20,
   STACK_POP,
   STACK_RET,
   STACK_CALL,

   XOR = 0x30,
   ADD,
   SUB,
   MUL,
   DIV,
   INC,
   DEC,
   AND,
   OR,

   INT_STORE = 0x40,
   INT_PRINT,
   INT_TOSTRING,
   INT_RANDOM,

   STRING_STORE = 0x50,
   STRING_PRINT,
   STRING_CONCAT,
   STRING_SYSTEM,
   STRING_TOINT,

   // we can add more here... I promise to go full-blown CISC, because I have to

};


//
// Function prototypes/signatures
//

int invm_Opcodes_Set( invm_t *p );

#endif

