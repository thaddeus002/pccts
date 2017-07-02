##
##  7-Apr-97
##  added support/genmk/makefile to pccts 1.33MR1 distribution kit
##		(support/genmk/makefile" omitted from 1.33 distribution kit)
##
SRC=genmk.c
OBJ=genmk.o
# Define PC if you use a PC OS (changes directory symbol and object file extension)
# see pccts/h/pcctscfg.h
# CFLAGS=-I../../h -DPC

IFLAGS = -I../../h
CC ?= cc

#ifndef PREFIX
PREFIX=/usr/local
#endif

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $<

genmk: $(OBJ) $(SRC) ../../h/pcctscfg.h
	$(CC) $(CFLAGS) $(IFLAGS) -o genmk $(OBJ)

clean:
	rm -rf core *.o

scrub:
	rm -rf genmk core *.o

install:
	install genmk $(PREFIX)/bin
