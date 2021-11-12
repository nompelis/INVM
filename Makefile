###### Compilers
CC = gcc
COPTS = -g -Wall -fPIC -O0
CXX = g++
CXXOPTS = -g -Wall -fPIC -O0

###### libraries
LIBS = -lm 

###### debugging switches
DEBUG += -D   _DEBUG_
DEBUG += -D   _DEBUG2_


all:
	$(CC) -c $(DEBUG) $(COPTS) invm.c
	$(CC) -c $(DEBUG) $(COPTS) invm_opcodes.c
	$(CC)    $(DEBUG) $(COPTS) main.c \
            invm.o invm_opcodes.o \
            $(LIBS)

clean:
	rm -f *.o *.a a.out

