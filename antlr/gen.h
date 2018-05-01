/*
 * Thaddeus002's pccts.
 *
 * Public domain.
 */

/**
 * \file gen.h
 * \brief Generate C code (ANSI, K&R, C++)
 */

#ifndef __GEN_H__
#define __GEN_H__



          /* T r a n s l a t i o n  T a b l e s */

/** C_Trans[node type] == pointer to function that knows how to translate that node. */
extern void (*C_Trans[])();

/**
 * C_JTrans[Junction type] == pointer to function that knows how to translate that
 * kind of junction node.
 */
extern void (*C_JTrans[])();

void freeBlkFsets( Junction *q );

/** See description of first_item_is_guess_block for background */
Junction *first_item_is_guess_block_extra(Junction *q );

/**
 * return NULL if 1st item of alt is NOT (...)? block; else return ptr to aSubBlk node
 * of (...)?;  This function ignores actions and predicates.
 */
Junction *first_item_is_guess_block( Junction *q );

void DumpANSIFunctionArgDef(FILE *f, Junction *q, int bInitializer);

void genStdPCCTSIncludeFile( FILE *f,char *gate );

/**
 * dump action 's' to file 'output' starting at "local" tab 'tabs'
 * Dump line information in front of action if GenLineInfo is set
 * If file == -1 then GenLineInfo is ignored.
 * The user may redefine the LineInfoFormatStr to his/her liking
 * most compilers will like the default, however.
 *
 * June '93; changed so that empty lines are left alone so that
 * line information is correct for the compiler/debuggers.
 */
void dumpAction( char *s, FILE *output, int tabs, int file, int line, int final_newline );

#endif
