/**
 * \file tokens.h
 * \brief List of labelled tokens and stuff
 *
 * Generated from: dlg_p.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */


#ifndef tokens_h
#define tokens_h

#define zzEOF_TOKEN 1
#define L_EOF 4
#define PER_PER 5
#define NAME_PER_PER 6
#define LEXMEMBER 7
#define LEXACTION 8
#define PARSERCLASS 9
#define LEXPREFIX 10
#define ACTION 11
#define GREAT_GREAT 12
#define L_BRACE 13
#define R_BRACE 14
#define L_PAR 15
#define R_PAR 16
#define L_BRACK 17
#define R_BRACK 18
#define ZERO_MORE 19
#define ONE_MORE 20
#define OR 21
#define RANGE 22
#define NOT 23
#define OCTAL_VALUE 24
#define HEX_VALUE 25
#define DEC_VALUE 26
#define TAB 27
#define NL 28
#define CR 29
#define BS 30
#define CONTINUATION 31
#define LIT 32
#define REGCHAR 33

void grammar(char *mode_file); //dlg_p.c
void start_states();
void do_conversion();
void rule_list();
void rule();
void reg_expr();
void and_expr();
void repeat_expr();
void expr();
void atom_list();
void near_atom();
void atom();
void anychar();

#include "antlr.h"

extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType setwd2[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd3[];



#endif

