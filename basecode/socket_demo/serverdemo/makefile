CC=g++
CFLAGS=-I.
DEPS=test.h
OBJ= serverdemo.o 

%.o:%.c $(DEPS)
	$(CC -c -o $@ $< $(CFLAGS)
server:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

