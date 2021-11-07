
#ifndef _INVM_H_
#define _INVM_H_

//
// The states of the machine
// (These need to be known to the "OP codes" implementation by including this
// file -- for servicing the stopping of the machine.)
//
enum vm_states {
   RUNNING = 0,
   STOPPED = 1,
   UNREADY = 2
};


//
// The machine's "register" is set up as a union in order to handle all of
// our types of variables that will participate in operations. (Great idea
// by skk: https://github.com/skx/simple.vm/blob/master/src/simple-vm.h)
//
typedef struct vmreg_s {
    union {
        unsigned int i;
        char *s;
    } content;
    enum { INTEGER, STRING } type;
} vmreg_t;

//
// A given flag is either set (1) or not set (0), but minimal storage is 8 bits
// (we can use a single bit, but then we worry about bitwise operations, so we
// will take a naive approach)
//
typedef struct vmflags_s {
   unsigned char iof;     // integer overflow
   unsigned char fpe;     // floating point exception (NO WORK)
   // ... (add more here)
} vmflags_t;

//
// an object that is the function that executes the OP code's implementation
// (Another great idea borrowed from ssk's VM.)
//
struct invm_s;
typedef void invm_opcode_f( struct invm_s * );

//
// the struct of the VM, containing registers, flags, program, and state
//
typedef struct invm_s {
   int id;
   char name[32];

   unsigned char state;
   vmflags_t flags;
   unsigned short nreg;
   vmreg_t *registers;

   unsigned char *prog;
   size_t psize;

   unsigned char *stack;
   size_t ssize;

   int sptr;                // stack pointer for zero memory reference
   unsigned int idx;        // index of (next) executing instruction

   invm_opcode_f *opcodes[256];
} invm_t;


//
// Function prototypes/signatures
//

int invm_Machine_Init( invm_t *p );

int invm_Machine_DumpRegisters( invm_t *p );

int invm_Machine_Clear( invm_t *p );

#endif

