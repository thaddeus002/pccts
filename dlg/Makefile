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

COMMON=../common
COMMON_H=../common

CC ?= gcc
CPPFLAGS += -I$(COMMON_H)

#ifndef PREFIX
PREFIX=/usr/local
#endif


SRC = dlg_p.c dlg_a.c main.c error.c $(COMMON)/set.c support.c output.c \
        relabel.c automata.c $(COMMON)/logger.c

OBJ = dlg_p.o dlg_a.o main.o error.o set.o support.o output.o \
        relabel.o automata.o logger.o

dlg : $(OBJ) $(SRC)
		$(CC) $(CFLAGS) $(CPPFLAGS) -o dlg $(OBJ)



dlg_p.o : dlg_p.c dlg_p.h tokens.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_p.c

dlg_a.o : dlg_a.c dlg_p.h tokens.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c dlg_a.c

main.o : main.c dlg_p.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c main.c

set.o : $(COMMON)/set.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

logger.o: $(COMMON)/logger.c
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
