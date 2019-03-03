/*
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-2000
 */

/**
 * \file antlr.h
 *
 * Define all of the stack setup and manipulation of $i, #i variables.
 */


#ifndef ANTLR_H
#define ANTLR_H


typedef int ANTLRTokenType;
typedef unsigned char SetWordType;
typedef char ANTLRChar;

void grammar();

            /* G u e s s  S t u f f */


/* can make this a power of 2 for more efficient lookup */
/* Allow user to override default ZZLEXBUFSIZE  */
#ifndef ZZLEXBUFSIZE
#define ZZLEXBUFSIZE 32000
#endif

        /* G l o b a l  V a r i a b l e s */

/* Define a parser; user should do a "#parser myname" in their grammar file */

extern int zztoken;
extern int zzasp;

#endif
