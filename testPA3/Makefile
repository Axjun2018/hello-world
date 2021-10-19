CC=gcc
CFLAGS=-I. -g -w
DEPS = chess.h
OBJ = chess.o move.o capture.o show.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chess: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
