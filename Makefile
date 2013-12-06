DEBUG=-g
PREFIX?=/usr/local
INCDIRS=-I$(PREFIX)/include
LIBDIRS=-L$(PREFIX)/lib
CFLAGS=$(DEBUG) $(INCDIRS) -std=c99 -Wall -Wextra -pedantic -Werror -D_WITH_GETLINE
LDFLAGS=$(DEBUG) $(LIBDIRS)

DEPENDS=$(INCDIRS) -MP -MD -MF

LIBS=-lpthread

CC=clang
CPP=clang-cpp

all: main

depend::
	$(CC) $(INCDIRS) -E -MM *.c >.depend

.SUFFIXES: .o .c

.c.o:
	$(CC) $(CFLAGS) -c $(.IMPSRC)

OBJS = main.o

main: $(OBJS)
	$(CC) $(LDFLAGS) -o $(.TARGET) $(.ALLSRC) $(LIBS)

clean::
	rm -f *.o
	rm -f main

clean-depend::
	rm -f .depend


