#
# Makefile for regex
#

CC 		= /usr/bin/g++

CFLAGS 		= -g -O4 -I..
LDFLAGS		=

all:	main exe

clean:
	-rm -f *.o *~

veryclean:
	-rm -f *.o *~ regex*

.c.o:
	${CC} ${CFLAGS} -c $*.c

stdinc.o : stdinc.h
int_set.o : stdinc.h int_set.h
linked_set.o : stdinc.h linked_set.h
wgraph.o : stdinc.h wgraph.h
partition.o : stdinc.h partition.h
dheap.o : stdinc.h dheap.h
dfa.o : stdinc.h dfa.h
subset.o : stdinc.h subset.h dfa.h nfa.h
nfa.o : stdinc.h nfa.h dfa.h
hybrid_fa.o : stdinc.h nfa.h dfa.h hybrid_fa.h
trace.o : stdinc.h trace.h
parser.o :  stdinc.h nfa.h dfa.h parser.h

main: stdinc.o int_set.o linked_set.o dheap.o partition.o wgraph.o dfa.o nfa.o subset.o parser.o trace.o hybrid_fa.o main.o cache.o fa_memory.o dfas_memory.o
	${CC} ${CFLAGS} stdinc.o int_set.o linked_set.o partition.o wgraph.o dheap.o dfa.o nfa.o subset.o parser.o main.o trace.o hybrid_fa.o cache.o fa_memory.o dfas_memory.o -o regex

exe:
	-chmod a+x regex*

libs:
	${CC} stdinc.c int_set.c linked_set.c dheap.c partition.c wgraph.c dfa.c nfa.c subset.c parser.c trace.c hybrid_fa.c cache.c fa_memory.c dfas_memory.c -fPIC -shared -o libregex.so

# EOF
