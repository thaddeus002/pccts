#
# Makefile for DLG 1.33
# Terence Parr
# Purdue University, U of MN, Parr Research Corporation
# 1989-1994
#
# Ported to IBM C-Set/2 and Microsoft 6.0 by
# Ed Harfmann
# Micro Data Base Systems
# Lafayette, Indiana
#
SET=../support/set
PCCTS_H=../h
COMMON_H=../common

CC ?= gcc
CPPFLAGS += -I. -I$(SET) -I$(PCCTS_H) -I$(COMMON_H) -DUSER_ZZSYN -DZZLEXBUFSIZE=32000
OBJ = dlg_p.o dlg_a.o main.o err.o set.o support.o output.o \
        relabel.o automata.o

#ifndef PREFIX
PREFIX=/usr/local
#endif

dlg : $(OBJ) $(SRC)
		$(CC) $(CFLAGS) $(CPPFLAGS) -o dlg $(OBJ)


SRC = dlg_p.c dlg_a.c main.c err.c $(SET)/set.c support.c output.c \
        relabel.c automata.c

#dlg_p.c parser.dlg err.c tokens.h : dlg_p.g
#	$(ANTLR) dlg_p.g

#dlg_a.c mode.h : parser.dlg
#	$(DLG) -C2 parser.dlg dlg_a.c

dlg_p.o : dlg_p.c dlg.h tokens.h mode.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_p.c

dlg_a.o : dlg_a.c dlg.h tokens.h mode.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_a.c

main.o : main.c dlg.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c main.c

set.o : $(SET)/set.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(SET)/set.c

#clean up all the intermediate files
clean:
	rm -f *.o core

#remove everything in clean plus the PCCTS files generated
scrub:
	rm -f dlg $(PCCTS_GEN) *.o core

install: dlg
	install dlg $(PREFIX)/bin
	install dlg.1 $(PREFIX)/man/man1
