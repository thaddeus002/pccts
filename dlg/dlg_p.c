/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   ..\bin\antlr dlg_p.g -gh
 *
 */

#include "constants.h"
#include <stdio.h>
#include <stdbool.h>

#include <ctype.h>
#include "dlg_p.h"
#include "dlg_a.h" // for zzline
#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "support.h"
#include "relabel.h"
#include "automata.h"
#include "constants.h"
#include "output.h"
#include "logger.h"
#include "error.h" // needed by error_handling.h
#define zzEOF_TOKEN 1  /* needed by error_handling.h */
#include "error_handling.h"

#define NFA_MIN   64  /* minimum nfa_array size */

#define zzOvfChk                            \
    if ( zzasp <= 0 )                                           \
    {                                                           \
        fprintf(stderr, zzStackOvfMsg, __FILE__, __LINE__);   \
        exit(1);                                               \
    }


#define zzmatch(_t)             \
  if ( !_zzmatch(_t, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet) ) goto fail;


#define zzRULE    Attrib *zzaRetPtr = &(zzaStack[zzasp-1]); \
          int zzBadTok=0; char *zzBadText="";   \
          int zzErrk=1,zzpf=0;                \
          SetWordType *zzMissSet=NULL; int zzMissTok=0; char *zzMissText=""


#define ZZA_STACKSIZE 400

int zzasp=ZZA_STACKSIZE;
char zzStackOvfMsg[]="fatal: attrib/AST stack overflow %s(%d)!\n";
Attrib zzaStack[ZZA_STACKSIZE];

int _zzmatch(int _t, char **zzBadText, char **zzMissText,
    int *zzMissTok, int *zzBadTok,
    SetWordType **zzMissSet)
{
  if ( zztoken!=_t ) {
    *zzBadText = *zzMissText= zzlextext;
    *zzMissTok= _t; *zzBadTok=zztoken;
    *zzMissSet=NULL;
    return 0;
  }
  zzOvfChk;
  --zzasp;
  zzcr_attr(&(zzaStack[zzasp]),zztoken,zzlextext);
  return 1;
}


static void zzsyn(char *text, int tok, char *egroup, SetWordType *eset, int etok, int k, char *bad_text);
extern void zzresynch(SetWordType *, SetWordType);

extern int case_insensitive;/* ignore case of input spec. */
extern int  dfa_basep[];  /* start of each group of dfa */ // output.c
extern int  dfa_class_nop[];/* number of transistion arcs in */ //output.c
        /* each dfa in each mode */


int action_no = 0;     /* keep track of actions outputed */
int nfa_allocated = 0; /* keeps track of number of nfa nodes */
nfa_node **nfa_array = NULL;/* root of binary tree that stores nfa array */
nfa_node nfa_model_node;   /* model to initialize new nodes */
set used_chars;    /* used to label trans. arcs */
set used_classes;    /* classes or chars used to label trans. arcs */
set normal_chars;    /* mask to get rid elements that aren't used in set */
int flag_paren = false;
int flag_brace = false;
int mode_counter = 0;  /* keep track of number of %%names */


static nfa_node *new_nfa_node();

/** stuff that needs to be reset when a new automaton is being built */
static void new_automaton_mode()
{
  set_free(used_chars);
  clear_hash();
}

static void start_states();

void grammar(char *version, char *mode_file)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  p_head(version, mode_file);
  p_class_hdr(version);
  func_action = false;
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    while ( (setwd1[zztoken]&0x1) ) {
      {
        int zztasp3 = zzasp - 1;
        zzOvfChk; --zzasp;
        {
        if ( (zztoken==LEXACTION) ) {
          zzmatch(zztoken); zzgettok();
        } else if ( (zztoken==LEXMEMBER) ) {
          zzmatch(zztoken); zzgettok();
        } else if ( (zztoken==LEXPREFIX) ) {
          zzmatch(zztoken); zzgettok();
        } else if ( (zztoken==PARSERCLASS) ) {
          zzmatch(zztoken); zzgettok();
        } else if ( (zztoken==ACTION) ) {
        } else {
          zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
        }

        zzasp=zztasp3;
        }
      }
      zzmatch(ACTION); zzgettok();
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  if ( gen_cpp ) p_includes();
  start_states();
  func_action = false; p_tables(); p_tail();
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    while ( (zztoken==ACTION) ) {
      zzmatch(ACTION); zzgettok();
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(1);
  if (firstLexMember != 0) p_class_def1();
  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

static void do_conversion();

static void start_states()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    if ( (zztoken==PER_PER) ) {
      zzmatch(PER_PER); zzgettok();
      do_conversion();
    }
    else {
      if ( (zztoken==NAME_PER_PER) ) {
        zzmatch(NAME_PER_PER); zzgettok();
        do_conversion();
        {
          int zztasp3 = zzasp - 1;
          zzOvfChk; --zzasp;
          {
          while ( (zztoken==NAME_PER_PER) ) {
            zzmatch(NAME_PER_PER); zzgettok();
            do_conversion();
            zzasp=zztasp3;
          }
          zzasp=zztasp3;
          }
        }
      }
      else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(PER_PER); zzgettok();
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

static void rule();
static void rule_list();

static void do_conversion()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  new_automaton_mode(); func_action = true;
  rule_list();

  dfa_class_nop[mode_counter] =
  relabel(zzaStack[zztasp1-1].l,comp_level);
  if (comp_level)
  p_shift_table(mode_counter);
  dfa_basep[mode_counter] = dfa_allocated+1;
  make_dfa_model_node(dfa_class_nop[mode_counter]);
  nfa_to_dfa(zzaStack[zztasp1-1].l);
  ++mode_counter;
  func_action = false;
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x8);
  }
}

static void rule_list()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  if ( (setwd1[zztoken]&0x10) ) {
    rule();
    (*zzaRetPtr).l=zzaStack[zztasp1-1 ].l; (*zzaRetPtr).r=zzaStack[zztasp1-1 ].r;
    {
      int zztasp2 = zzasp - 1;
      zzOvfChk; --zzasp;
      {
      while ( (setwd1[zztoken]&0x20) ) {
        rule();
        {nfa_node *t1;
          t1 = new_nfa_node();
          (t1)->trans[0]=(*zzaRetPtr).l;
          (t1)->trans[1]=zzaStack[zztasp2-1 ].l;
          /* all accept nodes "dead ends" */
          (*zzaRetPtr).l=t1; (*zzaRetPtr).r=NULL;
        }
        zzasp=zztasp2;
      }
      zzasp=zztasp2;
      }
    }
  }
  else {
    if ( (setwd1[zztoken]&0x40) ) {
      (*zzaRetPtr).l = new_nfa_node(); (*zzaRetPtr).r = NULL;
      warning("no regular expressions", file_str[0] ? file_str[0] : "stdin", zzline);

    }
    else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

static void reg_expr();
static void and_expr();
static void repeat_expr();
static void expr();

static void rule()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  if ( (setwd2[zztoken]&0x1) ) {
    reg_expr();
    zzmatch(ACTION);
    if (zzaStack[zztasp1-1 ].r != NULL) {
      (*zzaRetPtr).l=zzaStack[zztasp1-1 ].l; (*zzaRetPtr).r=zzaStack[zztasp1-1 ].r; (zzaStack[zztasp1-1 ].r)->accept=action_no;
    }
 zzgettok();

  }
  else {
    if ( (zztoken==ACTION) ) {
      zzmatch(ACTION);
      (*zzaRetPtr).l = NULL; (*zzaRetPtr).r = NULL;
      error("no expression for action  ", zzline);
      zzgettok();
    }
    else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

static void reg_expr()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  and_expr();
  (*zzaRetPtr).l=zzaStack[zztasp1-1 ].l; (*zzaRetPtr).r=zzaStack[zztasp1-1 ].r;
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    while ( (zztoken==OR) ) {
      zzmatch(OR); zzgettok();
      and_expr();
      {nfa_node *t1, *t2;
        t1 = new_nfa_node(); t2 = new_nfa_node();
        (t1)->trans[0]=(*zzaRetPtr).l;
        (t1)->trans[1]=zzaStack[zztasp2-2].l;
        if ((*zzaRetPtr).r != NULL) ((*zzaRetPtr).r)->trans[1]=t2;
        if (zzaStack[zztasp2-2].r) {
          (zzaStack[zztasp2-2].r)->trans[1]=t2;
        }
        (*zzaRetPtr).l=t1; (*zzaRetPtr).r=t2;
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

static void and_expr()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  repeat_expr();

  (*zzaRetPtr).l=zzaStack[zztasp1-1].l; (*zzaRetPtr).r=zzaStack[zztasp1-1].r;
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    while ( (setwd2[zztoken]&0x8) ) {
      repeat_expr();
      if ((*zzaRetPtr).r != NULL) {
        ((*zzaRetPtr).r)->trans[1]=zzaStack[zztasp2-1].l;
        (*zzaRetPtr).r=zzaStack[zztasp2-1].r;
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return;
}

static void repeat_expr()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  if ( (setwd2[zztoken]&0x20) ) {
    expr();
    (*zzaRetPtr).l=zzaStack[zztasp1-1].l; (*zzaRetPtr).r=zzaStack[zztasp1-1].r;
    {
      int zztasp2 = zzasp - 1;
      zzOvfChk; --zzasp;
      {
      if ( (zztoken==ZERO_MORE) ) {
        zzmatch(ZERO_MORE);
        { nfa_node *t1,*t2;
          /* MR23 */    if ((*zzaRetPtr).r != NULL) ((*zzaRetPtr).r)->trans[0] = (*zzaRetPtr).l;
          t1 = new_nfa_node(); t2 = new_nfa_node();
          t1->trans[0]=(*zzaRetPtr).l;
          t1->trans[1]=t2;
          /* MR23 */    if ((*zzaRetPtr).r != NULL) ((*zzaRetPtr).r)->trans[1]=t2;
          (*zzaRetPtr).l=t1;(*zzaRetPtr).r=t2;
        }
        zzgettok();

      }
      else {
        if ( (zztoken==ONE_MORE) ) {
          zzmatch(ONE_MORE);
          if ((*zzaRetPtr).r != NULL) ((*zzaRetPtr).r)->trans[0] = (*zzaRetPtr).l;
          zzgettok();

        }
        else {
          if ( (setwd2[zztoken]&0x40) ) {
          }
          else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      zzasp=zztasp2;
      }
    }
  }
  else {
    if ( (zztoken==ZERO_MORE) ) {
      zzmatch(ZERO_MORE);
      error("no expression for *", zzline);
 zzgettok();

    }
    else {
      if ( (zztoken==ONE_MORE) ) {
        zzmatch(ONE_MORE);
        error("no expression for +", zzline);
 zzgettok();

      }
      else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

static void atom_list();
static void atom();

static void expr()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  (*zzaRetPtr).l = new_nfa_node();
  (*zzaRetPtr).r = new_nfa_node();
  if ( (zztoken==L_BRACK) ) {
    zzmatch(L_BRACK); zzgettok();
    atom_list();
    zzmatch(R_BRACK);

    if ((*zzaRetPtr).l != NULL) {
      ((*zzaRetPtr).l)->trans[0] = (*zzaRetPtr).r;
      ((*zzaRetPtr).l)->label = set_dup(zzaStack[zztasp1-2].label);
      set_orin(&used_chars,((*zzaRetPtr).l)->label);
    }
    zzgettok();

  } else if ( (zztoken==NOT) ) {
    zzmatch(NOT); zzgettok();
    zzmatch(L_BRACK); zzgettok();
    atom_list();
    zzmatch(R_BRACK);

    if ((*zzaRetPtr).l != NULL) {
      ((*zzaRetPtr).l)->trans[0] = (*zzaRetPtr).r;
      ((*zzaRetPtr).l)->label = set_dif(normal_chars,zzaStack[zztasp1-3].label);
      set_orin(&used_chars,((*zzaRetPtr).l)->label);
    }
    zzgettok();

  } else if ( (zztoken==L_PAR) ) {
    zzmatch(L_PAR); zzgettok();
    reg_expr();
    zzmatch(R_PAR);

    if ((*zzaRetPtr).l != NULL) {
      ((*zzaRetPtr).l)->trans[0] = zzaStack[zztasp1-2].l;
      if (zzaStack[zztasp1-2].r) {
        (zzaStack[zztasp1-2].r)->trans[1] = (*zzaRetPtr).r;
      }
    }
    zzgettok();

  } else if ( (zztoken==L_BRACE) ) {
    zzmatch(L_BRACE); zzgettok();
    reg_expr();
    zzmatch(R_BRACE);

    if ((*zzaRetPtr).l != NULL) {
      ((*zzaRetPtr).l)->trans[0] = zzaStack[zztasp1-2].l;
      ((*zzaRetPtr).l)->trans[1] = (*zzaRetPtr).r;
      if (zzaStack[zztasp1-2].r) {
        (zzaStack[zztasp1-2].r)->trans[1] = (*zzaRetPtr).r;
      }
    }
    zzgettok();

  } else if ( (setwd3[zztoken]&0x1) ) {
    atom();

    if ((*zzaRetPtr).l != NULL) {
      ((*zzaRetPtr).l)->trans[0] = (*zzaRetPtr).r;
      ((*zzaRetPtr).l)->label = set_dup(zzaStack[zztasp1-1].label);
      set_orin(&used_chars,((*zzaRetPtr).l)->label);
    }
  } else {
    zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
    goto fail;
  }

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

static void near_atom();

static void atom_list()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  set_free((*zzaRetPtr).label);
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    while ( (setwd3[zztoken]&0x4) ) {
      near_atom();
      set_orin(&((*zzaRetPtr).label),zzaStack[zztasp2-1].label);
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

static void anychar();

static void near_atom()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  int i;
  int i_prime;
  anychar();
  (*zzaRetPtr).letter=zzaStack[zztasp1-1].letter; (*zzaRetPtr).label=set_of(zzaStack[zztasp1-1].letter);
  i_prime = zzaStack[zztasp1-1].letter + MIN_CHAR;
  if (case_insensitive && islower(i_prime))
  set_orel(toupper(i_prime)-MIN_CHAR,
  &((*zzaRetPtr).label));
  if (case_insensitive && isupper(i_prime))
  set_orel(tolower(i_prime)-MIN_CHAR,
  &((*zzaRetPtr).label));
  {
    int zztasp2 = zzasp - 1;
    zzOvfChk; --zzasp;
    {
    if ( (zztoken==RANGE) ) {
      zzmatch(RANGE); zzgettok();
      anychar();
      if (case_insensitive){
        i_prime = (*zzaRetPtr).letter+MIN_CHAR;
        (*zzaRetPtr).letter = (islower(i_prime) ?
        toupper(i_prime) : i_prime)-MIN_CHAR;
        i_prime = zzaStack[zztasp2-2].letter+MIN_CHAR;
        zzaStack[zztasp2-2].letter = (islower(i_prime) ?
        toupper(i_prime) : i_prime)-MIN_CHAR;
      }
      /* check to see if range okay */
      {
        int debugLetter1 = (*zzaRetPtr).letter;
        int debugLetter2 = zzaStack[zztasp2-2].letter;
      }
      if ((*zzaRetPtr).letter > zzaStack[zztasp2-2].letter
      && zzaStack[zztasp2-2].letter != 0xff){
        error("invalid range  ", zzline);
      }
      for (i=(*zzaRetPtr).letter; i<= (int)zzaStack[zztasp2-2].letter; ++i){
        set_orel(i,&((*zzaRetPtr).label));
        i_prime = i+MIN_CHAR;
        if (case_insensitive && islower(i_prime))
        set_orel(toupper(i_prime)-MIN_CHAR,
        &((*zzaRetPtr).label));
        if (case_insensitive && isupper(i_prime))
        set_orel(tolower(i_prime)-MIN_CHAR,
        &((*zzaRetPtr).label));
      }
    } else if ( !(setwd3[zztoken]&0x10) ) {
      zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;
    }

    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

static void atom()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {
  register int i_prime;
  anychar();
  (*zzaRetPtr).label = set_of(zzaStack[zztasp1-1].letter);
  i_prime = zzaStack[zztasp1-1].letter + MIN_CHAR;
  if (case_insensitive && islower(i_prime))
  set_orel(toupper(i_prime)-MIN_CHAR,
  &((*zzaRetPtr).label));
  if (case_insensitive && isupper(i_prime))
  set_orel(tolower(i_prime)-MIN_CHAR,
  &((*zzaRetPtr).label));
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}

static void anychar()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  zzOvfChk; --zzasp;
  {

  switch(zztoken) {
  case REGCHAR:
  case OCTAL_VALUE:
  case HEX_VALUE:
  case DEC_VALUE:
  case TAB:
  case NL:
  case CR:
  case BS:
  case LIT:
    zzmatch(zztoken);
    (*zzaRetPtr).letter = zzaStack[zztasp1-1].letter - MIN_CHAR;
    break;
  case L_EOF:
    zzmatch(zztoken);
    (*zzaRetPtr).letter = 0;
    break;
  default:
    zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;
  }

  zzgettok();
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  /* empty action */
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x80);
  }
}

/** adds a new nfa to the binary tree and returns a pointer to it */
static nfa_node * new_nfa_node()
{
  register nfa_node *t;
  static int nfa_size=0;  /* elements nfa_array[] can hold */

  ++nfa_allocated;
  if (nfa_size<=nfa_allocated){
    /* need to redo array */
    if (!nfa_array){
      /* need some to do inital allocation */
      nfa_size=nfa_allocated+NFA_MIN;
      nfa_array=(nfa_node **) malloc(sizeof(nfa_node*)*
      nfa_size);
    }else{
      /* need more space */
      nfa_size=2*(nfa_allocated+1);
      nfa_array=(nfa_node **) realloc(nfa_array,
      sizeof(nfa_node*)*nfa_size);
    }
  }
  /* fill out entry in array */
  t = (nfa_node*) malloc(sizeof(nfa_node));
  nfa_array[nfa_allocated] = t;
  *t = nfa_model_node;
  t->node_no = nfa_allocated;
  return t;
}


/** initialize the model node used to fill in newly made nfa_nodes */
void make_nfa_model_node()
{
  nfa_model_node.node_no = -1; /* impossible value for real nfa node */
  nfa_model_node.nfa_set = 0;
  nfa_model_node.accept = 0;   /* error state default*/
  nfa_model_node.trans[0] = NULL;
  nfa_model_node.trans[1] = NULL;
  nfa_model_node.label = empty;
}

#if defined(DEBUG) || defined(_DEBUG)

/* print out the pointer value and the node_number */
void fprint_dfa_pair(FILE *f, nfa_node *p)
{
  if (p){
    fprintf(f, "%x (%d)", p, p->node_no);
  }else{
    fprintf(f, "(nil)");
  }
}

/* print out interest information on a set */
void fprint_set(FILE *f, set s)
{
  unsigned int *x;

  fprintf(f, "n = %d,", s.n);
  if (s.setword){
    fprintf(f, "setword = %x,   ", s.setword);
    /* print out all the elements in the set */
    x = set_pdq(s);
    while (*x!=nil){
      fprintf(f, "%d ", *x);
      ++x;
    }
  }else{
    fprintf(f, "setword = (nil)");
  }
}

/* code to be able to dump out the nfas
return 0 if okay dump
return 1 if screwed up
*/
int dump_nfas(int first_node, int last_node)
{
  register int i;
  nfa_node *t;

  for (i=first_node; i<=last_node; ++i){
    t = NFA(i);
    if (!t) break;
    fprintf(stderr, "nfa_node %d {\n", t->node_no);
    fprintf(stderr, "\n\tnfa_set = %d\n", t->nfa_set);
    fprintf(stderr, "\taccept\t=\t%d\n", t->accept);
    fprintf(stderr, "\ttrans\t=\t(");
    fprint_dfa_pair(stderr, t->trans[0]);
    fprintf(stderr, ",");
    fprint_dfa_pair(stderr, t->trans[1]);
    fprintf(stderr, ")\n");
    fprintf(stderr, "\tlabel\t=\t{ ");
    fprint_set(stderr, t->label);
    fprintf(stderr, "\t}\n");
    fprintf(stderr, "}\n\n");
  }
  return 0;
}
#endif


static int zzset_deg(SetWordType *a)
{
  /* Fast compute degree of a set... the number
     of elements present in the set.  Assumes
     that all word bits are used in the set
  */
  SetWordType *p = a;
  SetWordType *endp = &(a[zzSET_SIZE]);
  int degree = 0;

  if ( a == NULL ) return 0;
  while ( p < endp )
  {
    SetWordType t = *p;
    SetWordType *b = &(bitmask[0]);
    do {
      if (t & *b) ++degree;
    } while (++b < &(bitmask[sizeof(SetWordType)*8]));
    p++;
  }

  return(degree);
}


static void zzedecode(SetWordType *a)
{
  register SetWordType *p = a;
  register SetWordType *endp = &(p[zzSET_SIZE]);
  register unsigned e = 0;

  if ( zzset_deg(a)>1 ) fprintf(stderr, " {");
  do {
    register SetWordType t = *p;
    register SetWordType *b = &(bitmask[0]);
    do {
      if ( t & *b ) fprintf(stderr, " %s", zztokens[e]);
      e++;
    } while (++b < &(bitmask[sizeof(SetWordType)*8]));
  } while (++p < endp);
  if ( zzset_deg(a)>1 ) fprintf(stderr, " }");
}


/*
 * DLG-specific syntax error message generator
 */
static void zzsyn(char *text, int tok, char *egroup, SetWordType *eset, int etok, int k, char *bad_text)
{
    hdrLog(file_str[0]!=NULL?file_str[0]:"stdin" , zzline);

    fprintf(stderr, " syntax error at \"%s\"", (tok==zzEOF_TOKEN)?"EOF":text);

    if ( !etok && !eset ) {fprintf(stderr, "\n"); return;}
    if ( k==1 ) fprintf(stderr, " missing");
    else
    {
        fprintf(stderr, "; \"%s\" not", bad_text);
        if ( zzset_deg(eset)>1 ) fprintf(stderr, " in");
    }
    if ( zzset_deg(eset)>0 ) zzedecode(eset);
    else fprintf(stderr, " %s", zztokens[etok]);
    if ( strlen(egroup) > (size_t)0 ) fprintf(stderr, " in %s", egroup);
    fprintf(stderr, "\n");
}
