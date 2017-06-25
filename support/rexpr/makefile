SRC=test.c rexpr.c
OBJ=test.o rexpr.o
CFLAGS_DEBUG = -g
CC ?= gcc

test: $(OBJ) $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -o texpr $(OBJ)

clean:
	rm -rf *.o core texpr

scrub: clean
	
