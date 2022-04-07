CC=gcc
CFLAGS=-Ofast -Wall
OBJ=mem.o parser.o scanner.o

debug: CFLAGS=-Og -g -Wall

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $*.c

clean:
	rm *.o