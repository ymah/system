


CC=gcc
CFLAGS= -Wall
CFLAGS+= -g
CFLAGS+= -m32
CFLAGS+= -Werror
#CFLAGS+= -pedantic

RM= rm -rf

TRASH= *# *~ *.o

FILE=contexte





all: contexte


hw.o:hw.h hw.c
	$(CC) $(CFLAGS) -o hw.o -c hw.c 

contexte.o:hw.h contexte.h contexte.c
	$(CC) $(CFLAGS) -o contexte.o -c contexte.c 

contexte:contexte.o hw.o
	$(CC) $(CFLAGS) -o contexte contexte.o hw.o

clean:
	$(RM) *# *~ *o

realclean:
	$(RM) *# *~ *o $(FILE)
