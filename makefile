CC=gcc
CFLAGS=-lm

trilat: trilat.o
	$(CC) -o trilat trilat.o $(CFLAGS)
