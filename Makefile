#
# Main makefile for PCCTS 1.33MR33	/* MRXXX */
#
# Terence Parr
# Parr Research Corporation
#


#ifndef PREFIX
PREFIX=/usr/local
#endif

# Where will go the executable binary files
BINDIR=$(PREFIX)/bin
INCDIR=$(PREFIX)/include/pccts

# This part added by Thomas Herter, M"unchen, Germany. See also manpages
# target.
MANDIR=$(PREFIX)/man
MANEXT=1
MANFILES=dlg/dlg.1 antlr/antlr.1

CC=gcc

# Set COPT if your build setup does not pass it in CFLAGS
#COPT=-O2

#
# 13 July 2015 - SLA
# Updated to split out make targets (for cccc) and enable jobserver.
#

.PHONY : pccts dlg antlr sorcerer genmk pre welcome fini testcpp

pccts: welcome antlr dlg fini

pre:
	@[ ! -d $(BINDIR) ] && install -d $(BINDIR) || true
	@[ ! -d $(MANDIR)/man$(MANEXT) ] && install -d $(MANDIR)/man$(MANEXT) || true

welcome	:
	@echo " "
	@echo "             Welcome to PCCTS 1.33MR33 installation"
	@echo " "
	@echo "             (Version 1.33 Maintenance Release #33)" # mrxxx
	@echo " "
	@echo "                  Released 19 April 2002"
	@echo " "
	@echo "                        Featuring"
	@echo "         ANTLR     -- ANother Tool for Language Recognition"
	@echo "         DLG       -- DFA-based Lexical Analyzer Generator"
	@echo "         SORCERER  -- Source-to-source translator (tree walker)"
	@echo " "
	@echo "                  http://www.antlr.org"
	@echo " "
	@echo "             Trouble reports to tmoog@polhode.com"
	@echo "             Additional PCCTS 1.33 information at"
	@echo "                  http://www.polhode.com"
	@echo
	@echo
	@echo Making executables...

antlr :
	$(MAKE) -C antlr $@

dlg	:
	$(MAKE) -C dlg $@

fini:
	@echo
	@echo "       PCCTS 1.33MR33 build complete"
	@echo "   do \"make install\" as root to install"
	@echo

testcpp:
	$(MAKE) -C testcpp
	$(MAKE) -C testcpp scrub

bin: antlr dlg
	mkdir -p bin
	cp antlr/antlr bin/
	cp dlg/dlg bin/

tests: bin testcpp

clean:
	$(MAKE) -C antlr clean
	$(MAKE) -C dlg clean

scrub:
	$(MAKE) -C antlr scrub
	$(MAKE) -C dlg scrub

manpages: pre
	install -m 644 $(MANFILES) $(MANDIR)/man$(MANEXT)

install: manpages
	$(MAKE) -C antlr PREFIX=$(PREFIX) install
	$(MAKE) -C dlg PREFIX=$(PREFIX) install
	install -d $(INCDIR)
	install -m 644 h/* $(INCDIR)
	install -m 644 hpp/* $(INCDIR)
