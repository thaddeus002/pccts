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

PCCTS_H=../h
COMMON=../common
COMMON_H=../common

CC ?= gcc
CPPFLAGS += -I$(PCCTS_H) -I$(COMMON_H) -DUSER_ZZSYN -DZZLEXBUFSIZE=32000

#ifndef PREFIX
PREFIX=/usr/local
#endif


SRC = dlg_p.c dlg_a.c main.c err.c $(COMMON)/set.c support.c output.c \
        relabel.c automata.c

OBJ = dlg_p.o dlg_a.o main.o err.o set.o support.o output.o \
        relabel.o automata.o

dlg : $(OBJ) $(SRC)
		$(CC) $(CFLAGS) $(CPPFLAGS) -o dlg $(OBJ)



dlg_p.o : dlg_p.c dlg.h tokens.h mode.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_p.c

dlg_a.o : dlg_a.c dlg.h tokens.h mode.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_a.c

main.o : main.c dlg.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c main.c

set.o : $(COMMON)/set.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

#clean up all the intermediate files
clean:
	rm -f *.o core

#remove everything in clean plus binaries
scrub: clean
	rm -f dlg

install: dlg
	install dlg $(PREFIX)/bin
	install dlg.1 $(PREFIX)/man/man1
