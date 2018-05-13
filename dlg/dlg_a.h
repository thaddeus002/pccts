/*
 * Thaddeus002's pccts
 *
 * Public Domain
 */

/**
 * \file dlg_a.h
 */

#ifndef __DLG_A_H__
#define __DLG_A_H__

#include <stdio.h>


#ifndef zzchar_t
#ifdef ZZWCHAR_T
#define zzchar_t wchar_t
#else
#define zzchar_t char
#endif
#endif


extern int func_action;    /* should actions be turned into functions?*/
extern int firstLexMember;

extern zzchar_t *zzlextext; /* text of most recently matched token */
extern int zzbufsize;  /* number of characters in zzlextext */
extern int zzline; /* line current token is on */

void zzrdstream( FILE *f );
void zzrdfunc( int (*f)(void) );
void zzrdstr(zzchar_t *s);
void zzmode( int m );
void zzgettok();

#endif
