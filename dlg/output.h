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
 * DLG 1.33
 * Will Cohen
 * With mods by Terence Parr; AHPCRC, University of Minnesota
 * 1989-2001
 */

/**
 * \file output.h
 * \brief output generator for dlg
 *
 * Output Notes:
 *
 * DfaStates == number of dfa nodes in automaton (just a #define)
 * DfaState == type large enough to index every node in automaton
 *         <256 unsigned char, <65536 unsigned short, etc.
 *
 * Thus, the elements in each of the automaton states (st%d) are type DfaState
 * and are size appropriately, since they must be able to index the next
 * automaton state.
 *
 * dfa[] == a linear array that points to all the automaton states (st%d)
 *         (dfa_base[] should be the same, but isn't right now)
 *
 * accepts[] == Taking a closer look at this one, it probably shouldn't be type
 *         DfaState because there is no real requirement that the number of
 *         accepts states is less than the number of dfa state.  However, if
 *         the number of accept states was more than the number of DFA states
 *         then the lexical specification would be really ambiguous.
 *
 *         Another note. Is that is should be possible to fold accepts[] and
 *         actions[] together.  If this is done, I would suggest get rid of
 *         accept[] and make actions[] have an entry for each state (st%d) in
 *         the automaton.
 *
 * dfa_base[] == starting location for each lexical mode.  This should be
 *         Dfastate type (but isn't right now), since it points to the states
 *         in the automaton.
 *
 * dfa_class_no[] == indicates the number of columns each lexical mode has.
 *
 * b_class_no[] == pointer to the start of the translation array used to
 *         convert from input character to character class.  This could cause
 *         problems if there are more than 256 classes
 *
 * shift%d[] == the actual translation arrays that convert the input character
 *         into the character class.  These will have to change if there are
 *         more than 256 character classes.
 */


#ifndef OUTPUT_H__
#define OUTPUT_H__


#include <stdio.h>


extern int operation_no; /* unique number for each operation */
extern FILE *output_stream; /* where to put the output */
extern FILE *class_stream; /* where to put the scan.h stuff (if gen_cpp) */



void p_class_hdr(char *version);

/* MR1                  */
/* MR1 16-Apr-97  Split printing of class header up into several parts  */
/* MR1        so that #lexprefix <<...>>and #lexmember <<...>>  */
/* MR1        can be inserted in the appropriate spots    */
/* MR1                  */

void p_class_def1();

void p_class_def2();

/**
 * generate required header on output
 * \param version the program's version
 * \param mode_file filename for mode output may be "mode.h"
 */
void p_head(char *version, char *mode_file);

void p_includes();

/** generate code to tie up any loose ends */
void p_tail();


/** output the table of DFA for general use */
void p_tables();


void p_shift_table(int m);


void p_mode_def(char *s, int m);


char *ClassName(char *suffix);

#endif
