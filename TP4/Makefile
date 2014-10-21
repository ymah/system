CC=gcc
CFLAGS= -Wall
CFLAGS+= -g
CFLAGS+= -m32
CFLAGS+= -Werror
#CFLAGS+= -pedantic

RM= rm -rf

TRASH=

FILE=contexte





all: contexte

sem.o:sem.h sem.c
	$(CC) $(CFLAGS) -o sem.o -c sem.c

hw.o:hw.h hw.c
	$(CC) $(CFLAGS) -o hw.o -c hw.c

contexte.o:sem.h hw.h contexte.h contexte.c
	$(CC) $(CFLAGS) -o contexte.o -c contexte.c

contexte:contexte.o hw.o sem.o
	$(CC) $(CFLAGS) -o contexte contexte.o hw.o sem.o

clean:
	$(RM) *# *~ *.o
realclean:
	$(RM) $(TRASH) $(FILE)
