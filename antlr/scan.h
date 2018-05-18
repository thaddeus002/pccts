/*
 * Thaddeus002's pccts
 *
 * Public Domain
 */

/**
 * \file scan.c
 */

extern set AST_nodes_refd_in_actions;
extern int inAlt;
extern set attribsRefdFromAction;
extern int UsedOldStyleAttrib;
extern int UsedNewStyleLabel;
extern int tokenActionActive;

extern zzchar_t *zzlextext; /* text of most recently matched token */
extern int zzbufsize;  /* number of characters in zzlextext */
extern int zzline; /* line current token is on */

extern void (*zzerr)(const char *);/* pointer to error reporting function */


void zzrdstream( FILE *f );

void zzrdfunc( int (*f)(void) );

void zzrdstr( zzchar_t *s );

/* saves dlg state, but not what feeds dlg (such as file position) */
void zzsave_dlg_state(struct zzdlg_state *state);

void zzrestore_dlg_state(struct zzdlg_state *state);

void zzmode(int m);

void zzgettok();
