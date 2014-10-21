CC=gcc
CFLAGS= -Wall
CFLAGS+= -g
CFLAGS+= -m32


all: contexte


contexte.o:contexte.h contexte.c
	$(CC) $(CFLAGS) -o contexte.o -c contexte.c

contexte:contexte.o
	$(CC) $(CFLAGS) -o contexte contexte.o

clean:
	rm -rf *# *~ *.o
fullclean:
	rm -rf *# *~ *.o contexte
