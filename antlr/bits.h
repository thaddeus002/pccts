/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */


/**
 * \file bits.h
 * \brief manage creation and output of bit sets used by the parser.
 */

#ifndef __BITS_H__
#define __BITS_H__


/**
 * char is only thing that is pretty much always known == 8 bits
 * This allows output of antlr (set stuff, anyway) to be androgynous (portable)
 */
typedef unsigned char SetWordType;


extern int setnum;
extern int wordnum;
extern int esetnum;


/** Create a new setwd (ignoring [Ep] token on end) */
void NewSetWd();

void DumpSetWd();

/**
 * Make a new set.
 * Dump old setwd and create new setwd if current setwd is full
 */
void NewSet();

/**
 * Turn on bit at each token position in set 'setnum'.
 * \param s a set of tokens
 */
void FillSet(set s);

void ComputeErrorSets();

void ComputeTokSets();

void DumpRemainingTokSets();

int DefErrSet1(int nilOK, set *f, int subst, char *name);

int DefErrSet(set *f, int subst, char *name);

int DefErrSetWithSuffix(int nilOK, set *f, int subst, char *name, const char* suffix);

void GenParser_c_Hdr();

void GenParser_h_Hdr();

/* Currently, this is only used in !GenCC mode */
void GenErrHdr();


#endif
