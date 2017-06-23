/**
    \file set.h

    \brief The following is a general-purpose set library originally developed
    by Hank Dietz and enhanced by Terence Parr to allow dynamic sets.

    Sets are now structs containing the number of words in the set and
    a pointer to the actual set words.

    1987 by Hank Dietz

    Modified by:
        Terence Parr
        Purdue University
        October 1989

        Added ANSI prototyping Dec. 1992 -- TJP
*/

#ifndef __GATE_SET_H
#define __GATE_SET_H

#include "pcctscfg.h"

#ifdef NOT_USED /* SEE config.h */
/* Define usable bits per unsigned int word */
#ifdef PC
#define WORDSIZE 16
#define LogWordSize 4
#else
#define WORDSIZE 32
#define LogWordSize 5
#endif
#define BytesPerWord    sizeof(unsigned)
#endif

#define SETSIZE(a) ((a).n<<LogWordSize)     /* Maximum items per set */
#define MODWORD(x) ((x) & (WORDSIZE-1))     /* x % WORDSIZE */
#define DIVWORD(x) ((x) >> LogWordSize)     /* x / WORDSIZE */
#define nil (~((unsigned) 0))   /* An impossible set member all bits on (big!) */

/**
 * The set struct, containing various words.
 */
typedef struct _set {
    unsigned int n; /**< Number of words in set */
    unsigned int *setword;
} set;

#define set_init    {0, NULL}
#define set_null(a) ((a).setword==NULL)

#define NumBytes(x)     (((x)>>3)+1)                        /* Num bytes to hold x */
#define NumWords(x)     ((((unsigned)(x))>>LogWordSize)+1)  /* Num words to hold x */


/* M a c r o s */

/* make arg1 a set big enough to hold max elem # of arg2 */
#define set_new(a,_max) \
if (((a).setword=(unsigned *)calloc(NumWords(_max),BytesPerWord))==NULL) \
        fprintf(stderr, "set_new: Cannot allocate set with max of %u\n", _max); \
        (a).n = NumWords(_max);

#define set_free(a)                                 \
    {if ( (a).setword != NULL ) free((char *)((a).setword));    \
    (a) = empty;}


void set_size( unsigned );
unsigned int set_deg( set );
set set_or( set, set );
set set_and( set, set );
set set_dif( set, set );
set set_of( unsigned );
void set_ext( set *, unsigned int );
set set_not( set );
int set_equ( set, set );
int set_sub( set, set );
unsigned set_int( set );
int set_el( unsigned, set );
int set_nil( set );
char * set_str( set );
set set_val( register char * );
void set_orel( unsigned, set * );
void set_orin( set *, set );
void set_andin( set *, set );
void set_rm( unsigned, set );
void set_clr( set );
set set_dup( set );
void set_PDQ( set, register unsigned * );
unsigned *set_pdq( set );
void _set_pdq( set a, register unsigned *q );
unsigned int set_hash( set, register unsigned int );

/** Global variable for an empty set */
set empty;

#endif
