CC=g++
CFLAGS=-I.
DEPS=demo.h
OBJ=clientdemo.o
%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< #(CFLAGS)
client:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

