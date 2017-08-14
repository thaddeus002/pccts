#
# PCCTS makefile for: SORCERER 1.00B17
# Uses PCCTS release 1.33MR23
#
PCCTS=..
BIN = $(PCCTS)/bin
ANTLR = $(BIN)/antlr
DLG = $(BIN)/dlg
PCCTS_H = $(PCCTS)/h
COMMON=$(PCCTS)/common
COMMON_H=$(PCCTS)/common

CC=cc
COPT=-g
CFLAGS = $(COPT) -Ih -I$(PCCTS_H) -I$(COMMON_H) -DZZLEXBUFSIZE=32000
AFLAGS = -gh -k 2 -gt
DFLAGS = -C2
GRM = sor.g
SRC = sor.c scan.c err.c main.c globals.c gen.c $(COMMON)/set.c hash.c look.c cpp.c
OBJ = sor.o scan.o err.o main.o globals.o gen.o set.o hash.o look.o cpp.o
GENHFILES = tokens.h mode.h stdpccts.h
SHELL=/bin/sh

all:
	$(MAKE) sor
	$(MAKE) -C lib CC="$(CC)" COPT="$(COPT)"

sor: $(OBJ) $(SRC)
	$(CC) -o sor $(CFLAGS) $(OBJ)
	mv sor $(BIN)/

sor.o : sor.c mode.h tokens.h
	$(CC) $(CFLAGS) -c -o sor.o sor.c

sor.c stdpccts.h parser.dlg tokens.h err.c : sor.g
	$(ANTLR) $(AFLAGS) sor.g

set.o : $(COMMON)/set.c
	$(CC) $(CFLAGS) -c -o set.o $(COMMON)/set.c

scan.c mode.h : parser.dlg
	$(DLG) $(DFLAGS) parser.dlg scan.c

scan.o : scan.c mode.h tokens.h

clean:
	rm -f *.o core sor
	$(MAKE) -C lib clean

scrub:
	rm -f *.o core $(GENHFILES) parser.dlg scan.c err.c sor.c sor
	$(MAKE) -C lib scrub
