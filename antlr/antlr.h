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

#include <string.h>
#include "syn.h" // for Graph


typedef int ANTLRTokenType;
typedef unsigned char SetWordType;
typedef char ANTLRChar;
typedef Graph Attrib;

void grammar();

            /* G u e s s  S t u f f */


/* can make this a power of 2 for more efficient lookup */
/* Allow user to override default ZZLEXBUFSIZE  */
#ifndef ZZLEXBUFSIZE
#define ZZLEXBUFSIZE 32000
#endif



/*  MR23            Provide more control over failed predicate action
                    without any need for user to worry about guessing internals.
                    _hasuseraction == 0 => no user specified error action
                    _hasuseraction == 1 => user specified error action
*/

#define zzfailed_pred(_p,_hasuseraction,_useraction) \
    if (_hasuseraction) { _useraction } \
    else { fprintf(stderr, "semantic error; failed predicate: '%s'\n",_p); }

            /* S t a t e  S t u f f */

#define zzGUESS_BLOCK
#define zzGUESS
#define zzGUESS_FAIL
#define zzGUESS_DONE
#define zzNON_GUESS_MODE
#define zzGuessData


                 /* I n f i n i t e  L o o k a h e a d */


#define zzenterANTLR(f)             \
    {static char zztoktext[ZZLEXBUFSIZE]; \
    zzlextext = zztoktext; zzrdstream( f ); zzgettok();}


#define ANTLR(st, f)  zzbufsize = ZZLEXBUFSIZE; \
            zzenterANTLR(f);      \
            {                                            \
              zzBLOCK(zztasp1);                          \
              st;                                        \
              zzEXIT_ANTLR(zztasp1 + 1);                 \
            }

#define zztext    zzlextext


          /* A r g u m e n t  A c c e s s */

#define zzaCur      (zzaStack[zzasp])
#define zzaRet      (*zzaRetPtr)
#define zzaArg(v,n)   zzaStack[v-n]
#define zzREL(t)    zzasp=(t);    /* Restore state of stack */

#define zzRULE    Attrib *zzaRetPtr = &(zzaStack[zzasp-1]); \
          int zzBadTok=0; char *zzBadText="";   \
          int zzErrk=1,zzpf=0;                \
          SetWordType *zzMissSet=NULL; int zzMissTok=0; char *zzMissText=""

#define zzBLOCK(i)  int i = zzasp - 1
#define zzEXIT(i) zzREL(i)
#define zzEXIT_ANTLR(i) zzREL(i)           /* [i_a] added as we want this for the ANTLRx() macros */
#define zzLOOP(i) zzREL(i)

#define zzCONSUME zzgettok();
#define NLA     zztoken
#define NLATEXT   zztext
#define LA(i)       zztoken
#define LATEXT(i)   zztext


           /* S t a n d a r d  S i g n a l s */

#define NoSignal      0
#define MismatchedToken   1
#define NoViableAlt     2
#define NoSemViableAlt    3

/* MR7  Allow more control over signalling                                  */
/*        by adding "Unwind" and "zzsetSignal"                              */

#define Unwind              4
#define zzsetSignal(newValue) *_retsignal=_signal=(newValue)
#define zzsuppressSignal *_retsignal=_signal=0
#define zzexportSignal    *_retsignal=_signal

           /* F u n c t i o n  T r a c i n g */

#define zzTRACEIN(r)  zzTracePrevRuleName=zzTraceCurrentRuleName;
#define zzTRACEOUT(r) zzTraceCurrentRuleName=zzTracePrevRuleName;

/* MR19 zzchar_t additions */
#define zzchar_t char


        /* E x t e r n  D e f s */

extern void zzresynch(SetWordType *, SetWordType);
extern void zzfill_inf_look(void);

        /* G l o b a l  V a r i a b l e s */

/* Define a parser; user should do a "#parser myname" in their grammar file */

extern int zztoken;
extern int zzasp;
extern Attrib zzaStack[];

#endif
