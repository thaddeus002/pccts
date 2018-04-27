/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file lex.h
 * \brief Generate all of the lexical type files: parser.dlg tokens.h
 */

#ifndef __LEX_H__
#define __LEX_H__


/** Generate a complete lexical description of the lexemes found in the grammar */
void genLexDescr();


/**
 * Generate a list of #defines && list of struct definitions for
 * aggregate retv's
 */
void genDefFile(void);


void GenRemapFile(void);


/**
 * Define all rules in the class.h file; generate any required
 * struct definitions first, however.
 */
void GenRuleMemberDeclarationsForCC(FILE *f, Junction *q);


/**
 * given a list of parameters or return values, dump the next
 * name to output.  Return 1 if last one just printed, 0 if more to go.
 */
int DumpNextNameInDef(char **q, FILE *output);


/** Take in a type definition (type + symbol) and print out type only */
void DumpType(char *s, FILE *f);


#endif
