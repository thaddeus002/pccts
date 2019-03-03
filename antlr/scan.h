/*
 * Thaddeus002's pccts
 *
 * Public Domain
 */

/**
 * \file scan.h
 */

#ifndef __SCAN_H__
#define __SCAN_H__

#include <stdio.h>

// TODO mask this mode in scan.c
#define PARSE_ENUM_FILE 11

struct zzdlg_state {
  FILE *stream;
  int (*func_ptr)();
  char *str;
  int auto_num;
  int add_erase;
  int lookc;
  int char_full;
  int begcol, endcol;
  int line;
  char *lextext, *begexpr, *endexpr;
  int bufsize;
  int bufovf;
  char *nextpos;
  int class_num;
};

extern set AST_nodes_refd_in_actions;
extern int inAlt;
extern set attribsRefdFromAction;
extern int UsedOldStyleAttrib;
extern int UsedNewStyleLabel;
extern int tokenActionActive;

extern char *zzlextext; /* text of most recently matched token */
extern int zzbufsize;  /* number of characters in zzlextext */
extern int zzline; /* line current token is on */

extern void (*zzerr)(const char *);/* pointer to error reporting function */

/* what stream to read from */
void zzrdstream( FILE *f );

/* what function to get char from */
void zzrdfunc( int (*f)(void) );

void zzrdstr( char *s );

/* saves dlg state, but not what feeds dlg (such as file position) */
void zzsave_dlg_state(struct zzdlg_state *state);

void zzrestore_dlg_state(struct zzdlg_state *state);

/* switch to automaton 'm' */
void zzmode(int m);

/* get next token */
void zzgettok();

#endif
