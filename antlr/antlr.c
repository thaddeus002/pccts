/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   ..\bin\antlr -gh antlr.g
 *
 */

#define ANTLR_VERSION 13333
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "hash.h"
#include "syn.h"
#include "generic.h"
#include "proto.h"
#include "antlr.h"
#include "tokens.h"
#include "constants.h"
#include "logger.h"
#include "antlr_log.h"
#include "build.h"
#include "egman.h"
#include "pred.h"
#include "utils.h"
#include "misc.h"
#include "mrhoist.h"
#include "fset2.h"
#define zzSET_SIZE 20 /* error_handling.h needs this */
#include "error.h" /* ... and this */
#include "scan.h" /* ... and this for zzline */
#include "error_handling.h"

#define MaxClassDeclStuff   256
#define ZZA_STACKSIZE 400

ListNode *CurExGroups=NULL; /* Current list of exception groups for rule/alts */
ListNode *CurElementLabels=NULL;
Junction *CurRuleBlk=NULL;  /* Pointer to current block node for enclosing block */
char *CurPredName=NULL;
Junction *CurAltStart = NULL; /* Junction node that starts the alt */
static int CurAltNum=0;
static int CurBlockID=0;   /* Unique int for each block */
RuleEntry *CurRuleNode=NULL;/* Pointer to current rule node in syntax tree */

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif


#define zzsetmatch(_es,_tokclassErrset)           \
  if ( !_zzsetmatch(_es, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet, _tokclassErrset) ) goto fail;

#define zzmatch(_t)             \
  if ( !_zzmatch(_t, &zzBadText, &zzMissText, &zzMissTok, &zzBadTok, &zzMissSet) ) goto fail;


int zzasp=ZZA_STACKSIZE;
const char *zzStackOvfMsg = "fatal: attrib/AST stack overflow %s(%d)!\n";
Attrib zzaStack[ZZA_STACKSIZE];

#define MAX_BLK_LEVEL 100
int CurBlockID_array[MAX_BLK_LEVEL];
int CurAltNum_array[MAX_BLK_LEVEL];


static void check_overflow() {
    if (zzasp <= 0) {
        fprintf(stderr, zzStackOvfMsg, __FILE__, __LINE__);
        exit(1);
    }
}


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
  check_overflow(); --zzasp;
  return 1;
}


int _zzsetmatch(SetWordType *e, char **zzBadText, char **zzMissText,
      int *zzMissTok, int *zzBadTok,
      SetWordType **zzMissSet,
      SetWordType *zzTokclassErrset)
{
  if ( !zzset_el((unsigned)zztoken, e) ) {
    *zzBadText = zzlextext; *zzMissText=NULL;
    *zzMissTok= 0; *zzBadTok=zztoken;
    *zzMissSet=zzTokclassErrset;
    return 0;
  }
  check_overflow(); --zzasp;
  return 1;
}


typedef struct _zzantlr_state {
      int asp;
      int ast_sp;
      int token;
      char text[ZZLEXBUFSIZE];
} zzantlr_state;


static void zzsave_antlr_state(zzantlr_state *buf)
{
  buf->asp = zzasp;
  buf->token = zztoken;
  strcpy(buf->text, zzlextext);
}

static void zzrestore_antlr_state(zzantlr_state *buf)
{
  zzasp = buf->asp;
  zztoken = buf->token;
  strcpy(zzlextext, buf->text);
}

static void chkToken(char *, char *, char *, int);

static int isDLGmaxToken(char *Token);

static int class_nest_level = 0;

static void zzsyn(char *text, int tok, char *egroup, SetWordType *eset, int etok,
int k, char *bad_text);


#define ANTLRm(st, f, _m) zzbufsize = ZZLEXBUFSIZE; \
            zzmode(_m);       \
            zzenterANTLR(f);      \
            {                                            \
              int zztasp1 = zzasp - 1;                          \
              st;      \
              /* MR20 G. Hobbelt. Kill the top' attribute (+AST stack corr.) */  \
              zzasp=zztasp1 + 1;                 \
            }


static UserAction *newUserAction(char *s)
{
  UserAction *ua = (UserAction *) calloc(1, sizeof(UserAction));
  require(ua!=NULL, "cannot allocate UserAction");

  ua->action = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
  strcpy(ua->action, s);
  return ua;
}


static char *makeAltID(int blockid, int altnum)
{
  static char buf[100];
  char *p;
  sprintf(buf, "_blk%d_alt%d", blockid, altnum);
  p = (char *)malloc(strlen(buf)+1);
  strcpy(p, buf);
  return p;
}

static void class_def();
static void rule();
static void laction();
static void lmember();
static void lprefix();
static void aPred();
static Predicate *predOrExpr();
static Predicate *predAndExpr();
static Predicate *predPrimary();
static void aLexclass();
static void error();
static void tclass();
static void token();
static void block(set *toksrefd, set *rulesrefd);
static void alt(set *toksrefd, set *rulesrefd);
static Node *element(int old_not,int first_on_line,int use_def_MT_handler);
static void default_exception_handler();
static ExceptionGroup *exception_group();
static ExceptionHandler *exception_handler();
static void enum_file(char *fname);
static void defines(char *fname);
static void enum_def(char *fname);


void grammar()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  Graph g;
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;

    for (;;) {
      if (!(setwd1[zztoken]&0x1)) break;
      if (zztoken==94) {
        zzmatch(94); zzgettok();
        zzmatch(Action);

        if ( HdrAction==NULL ) {
          HdrAction = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
          require(HdrAction!=NULL, "rule grammar: cannot allocate header action");
          strcpy(HdrAction, zzlextext);
        }
        else warn("additional #header statement ignored");
        zzgettok();

      } else if (zztoken==95) {
        zzmatch(95); zzgettok();
        zzmatch(Action);

        if ( FirstAction==NULL ) {
          FirstAction = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
          require(FirstAction!=NULL, "rule grammar: cannot allocate #first action");
          strcpy(FirstAction, zzlextext);
        } else {
          warn("additional #first statement ignored");
        }
        zzgettok();

      } else if (zztoken==96) {
        zzmatch(96);
        zzgettok();
        zzmatch(QuotedTerm);

        if ( GenCC ) {
          warn("#parser meta-op incompatible with -CC; ignored");
        } else if ( strcmp(ParserName,"zzparser")==0 ) {
          ParserName=strip_quotes(strdup(zzlextext));
          if ( RulePrefix[0]!='\0' ) {
            warn("#parser meta-op incompatible with '-gp prefix'; '-gp' ignored");
            RulePrefix[0]='\0';
          }
        } else warn("additional #parser statement ignored");

        zzgettok();
      } else if (zztoken==97) {
        zzmatch(97);
        zzgettok();
        zzmatch(QuotedTerm);
        {
          char *fname;
          zzantlr_state st; FILE *f; struct zzdlg_state dst;
          UserTokenDefsFile = strdup(zzlextext);
          zzsave_antlr_state(&st);
          zzsave_dlg_state(&dst);
          fname = strdup(zzlextext);
          f = fopen(strip_quotes(fname), "r");
          if ( f==NULL ) {
            warn(eMsg("cannot open token defs file '%s'", fname));
          } else {
            ANTLRm(enum_file(fname), f, PARSE_ENUM_FILE);
            UserDefdTokens = 1;
          }
          zzrestore_antlr_state(&st);
          zzrestore_dlg_state(&dst);
        }
        zzgettok();
      } else break; /* MR6 code for exiting loop "for sure" */

      zzasp=zztasp2;
    }
    zzasp=zztasp2;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    for (;;) {
      if ( !((setwd1[zztoken]&0x2))) break;
      if ( (zztoken==Action) ) {
        zzmatch(Action);
        {
          UserAction *ua = newUserAction(zzlextext);
          ua->file = action_file; ua->line = action_line;
          if ( class_nest_level>0 ) list_add(&class_before_actions, ua);
          else list_add(&BeforeActions, ua);
        }
        zzgettok();
      } else if (zztoken==108) {
        laction();
      } else if (zztoken==109) {
        lmember();
      } else if (zztoken==110) {
        lprefix();
      } else if (zztoken==116) {
        aLexclass();
      } else if (zztoken==120) {
        token();
      } else if (zztoken==117) {
        error();
      } else if (zztoken==118) {
        tclass();
      } else if (zztoken==111) {
        aPred();
      } else if (zztoken==133) {
        default_exception_handler();
      } else if (zztoken==99) {
        class_def();
      } else if (zztoken==98) {
        zzmatch(98);
        if ( class_nest_level==0 )
          warn("missing class definition for trailing '}'");
        class_nest_level--;
        zzgettok();
      } else break; /* MR6 code for exiting loop "for sure" */
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  rule();
  g=zzaArg(zztasp1,3); SynDiag = (Junction *) zzaArg(zztasp1,3 ).left;
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    for (;;) {
      if ( !((setwd1[zztoken]&0x4))) break;
      if ( (zztoken==NonTerminal) ) {
        rule();
        if ( zzaArg(zztasp2,1 ).left!=NULL ) {
          g.right = NULL;

/* MR21a */             /*  Avoid use of a malformed graph when CannotContinue */
          /* MR21a */             /*  is already set                                     */
          /* MR21a */
          /* MR21a */             if (! (CannotContinue && g.left == NULL)) {
            /* MR21a */               g = Or(g, zzaArg(zztasp2,1));
            /* MR21a */             }
          /* MR21a */         }
      }
      else {
        if (zztoken==116) {
          aLexclass();
        }
        else {
          if (zztoken==120) {
            token();
          }
          else {
            if (zztoken==117) {
              error();
            }
            else {
              if (zztoken==118) {
                tclass();
              }
              else {
                if (zztoken==111) {
                  aPred();
                }
                else {
                  if (zztoken==99) {
                    class_def();
                  }
                  else {
                    if (zztoken==98) {
                      zzmatch(98);

                      if ( class_nest_level==0 )
                        warn("missing class definition for trailing '}'");
                      class_nest_level--;
		      zzgettok();
                    }
                    else break; /* MR6 code for exiting loop "for sure" */
                  }
                }
              }
            }
          }
        }
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    for (;;) {
      if ( !((setwd1[zztoken]&0x8))) break;
      if ( (zztoken==Action) ) {
        zzmatch(Action);
        {
          UserAction *ua = newUserAction(zzlextext);
          ua->file = action_file; ua->line = action_line;
          if ( class_nest_level>0 ) list_add(&class_after_actions, ua);
          else list_add(&AfterActions, ua);
        }
        zzgettok();

      }
      else {
        if (zztoken==108) {
          laction();
        }
        else {
          if (zztoken==109) {
            lmember();
          }
          else {
            if (zztoken==110) {
              lprefix();
            }
            else {
              if (zztoken==117) {
                error();
              }
              else {
                if (zztoken==118) {
                  tclass();
                }
                else {
                  if (zztoken==99) {
                    class_def();
                  }
                  else {
                    if (zztoken==111) {
                      aPred();
                    }
                    else {
                      if (zztoken==98) {
                        zzmatch(98);

                        if ( class_nest_level==0 )
                        warn("missing class definition for trailing '}'");
                        class_nest_level--;
                        zzgettok();
                      }
                      else break; /* MR6 code for exiting loop "for sure" */
                    }
                  }
                }
              }
            }
          }
        }
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(Eof);
  zzgettok();
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

static void class_def()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  int go=1; char name[MaxRuleName+1];
  zzmatch(99);
  zzgettok();
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==NonTerminal) ) {
      zzmatch(NonTerminal);
      if(go) strncpy(name,zzlextext,MaxRuleName);
      zzgettok();
    }
    else {
      if ( (zztoken==TokenTerm) ) {
        zzmatch(TokenTerm);
        if(go) strncpy(name,zzlextext,MaxRuleName);
        zzgettok();
      }
      else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }

  if ( CurrentClassName[0]!='\0' && strcmp(CurrentClassName,name)!=0
  && GenCC ) {
    err("only one grammar class allowed in this release");
    go = 0;
  }
  else strcpy(CurrentClassName, name);
  if ( !GenCC ) { err("class meta-op used without C++ option"); }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while ( (setwd1[zztoken]&0x20) ) {
      zzsetmatch(zzerr2, zzerr3);
      if (ClassDeclStuff == NULL) {
        /* MR10 */                   ClassDeclStuff=(char *)calloc(MaxClassDeclStuff+1,sizeof(char));
        /* MR10 */              };
      /* MR10 */              strncat(ClassDeclStuff," ",MaxClassDeclStuff);
      /* MR10 */              strncat(ClassDeclStuff,zzlextext,MaxClassDeclStuff);
      /* MR22 */              do {
        /* MR22 */                if (0 == strcmp(zzlextext,"public")) break;
        /* MR22 */                if (0 == strcmp(zzlextext,"private")) break;
        /* MR22 */                if (0 == strcmp(zzlextext,"protected")) break;
        /* MR22 */                if (0 == strcmp(zzlextext,"virtual")) break;
        /* MR22 */                if (0 == strcmp(zzlextext,",")) break;
        /* MR22 */                if (0 == strcmp(zzlextext,":")) break;
        /* MR22 */                if (BaseClassName != NULL) break;
        /* MR22 */                BaseClassName=(char *)calloc(strlen(zzlextext)+1,sizeof(char));
        /* MR22 */                require(BaseClassName!=NULL, "rule grammar: cannot allocate base class name");
        /* MR22 */          strcpy(BaseClassName,zzlextext);
        /* MR22 */              } while (0);
      /* MR10 */
      zzgettok();

      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(102);

  no_classes_found = 0;
  if ( class_nest_level>=1 ) {warn("cannot have nested classes");}
  else class_nest_level++;
  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

static void rule()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
    ExceptionGroup *eg;
    RuleEntry *q; Junction *p; Graph r; int f, l; ECnode *e;
    set toksrefd, rulesrefd;
    char *pdecl=NULL, *ret=NULL, *a; CurRetDef = CurParmDef = NULL;
    CurExGroups = NULL;
    CurElementLabels = NULL;
    CurAstLabelsInActions = NULL;
    /* We want a new element label hash table for each rule */
    if ( Elabel!=NULL ) killHashTable(Elabel);
    Elabel = newHashTable();
    attribsRefdFromAction = empty;
    zzmatch(NonTerminal);
    q=NULL;
    if ( hash_get(Rname, zzlextext)!=NULL ) {
      err(eMsg("duplicate rule definition: '%s'",zzlextext));
      CannotContinue=true;
    }
    else
    {
      q = (RuleEntry *)hash_add(Rname,
        zzlextext,
        (Entry *)newRuleEntry(zzlextext));
      CurRule = q->str;
    }
    CurRuleNode = q;
    f = CurFile; l = zzline;
    NumRules++;
    zzgettok();
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;
      {
        if ( (zztoken==103) ) {
          zzmatch(103);
          if ( q!=NULL ) q->noAST = true;
          zzgettok();
        }
        else {
          if ( (setwd1[zztoken]&0x80) ) {
          }
          else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzasp=zztasp2;
      }
    }
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;
      {
        if ( (setwd2[zztoken]&0x1) ) {
        {
          int zztasp3 = zzasp - 1;
          check_overflow(); --zzasp;
          {
            if ( (zztoken==104) ) {
              zzmatch(104); zzgettok();
            }
            else {
              if ( (zztoken==PassAction) ) {
              }
              else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp3;
          }
        }
        zzmatch(PassAction);
        pdecl = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
        require(pdecl!=NULL, "rule rule: cannot allocate param decl");
        strcpy(pdecl, zzlextext);
        CurParmDef = pdecl;
        zzgettok();
      }
      else {
        if ( (setwd2[zztoken]&0x2) ) {
        }
        else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
      zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==105) ) {
      zzmatch(105);
      zzgettok();
      zzmatch(PassAction);
      ret = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
      require(ret!=NULL, "rule rule: cannot allocate ret type");
      strcpy(ret, zzlextext);
      CurRetDef = ret;
      zzgettok();
    }
    else {
      if ( (setwd2[zztoken]&0x4) ) {
      }
      else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==QuotedTerm) ) {
      zzmatch(QuotedTerm);
      if ( q!=NULL ) q->egroup=strdup(zzlextext);
      zzgettok();
    }
    else {
      if (zztoken==106) {
      }
      else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }

  if ( GenEClasseForRules && q!=NULL ) {
    e = newECnode;
    require(e!=NULL, "cannot allocate error class node");
    if ( q->egroup == NULL ) {a = q->str; a[0] = (char)toupper(a[0]);}
    else a = q->egroup;
    if ( Tnum( a ) == 0 )
    {
      e->tok = addTname( a );
      list_add(&eclasses, (char *)e);
      if ( q->egroup == NULL ) a[0] = (char)tolower(a[0]);
      /* refers to itself */
      list_add(&(e->elist), strdup(q->str));
    }
    else {
      warn(eMsg("default errclass for '%s' would conflict with token/errclass/tokclass",a));
      if ( q->egroup == NULL ) a[0] = (char)tolower(a[0]);
      free((char *)e);
    }
  }
  BlkLevel++;
  if (BlkLevel >= MAX_BLK_LEVEL) antlr_fatal("Blocks nested too deeply");
  CurBlockID_array[BlkLevel] = CurBlockID;
  CurAltNum_array[BlkLevel] = CurAltNum;
  zzmatch(106);
  inAlt=1;
  zzgettok();

  block( &toksrefd, &rulesrefd );
  r = makeBlk(zzaArg(zztasp1,7),0, NULL /* pFirstSetSymbol */ );
  CurRuleBlk = (Junction *)r.left;
  CurRuleBlk->blockid = CurBlockID;
  CurRuleBlk->jtype = RuleBlk;
  if ( q!=NULL ) CurRuleBlk->rname = q->str;
  CurRuleBlk->file = f;
  CurRuleBlk->line = l;
  CurRuleBlk->pdecl = pdecl;
  CurRuleBlk->ret = ret;
  CurRuleBlk->lock = makelocks();
  CurRuleBlk->pred_lock = makelocks();
  CurRuleBlk->tokrefs = toksrefd;
  CurRuleBlk->rulerefs = rulesrefd;
  p = newJunction();  /* add EndRule Node */
  ((Junction *)r.right)->p1 = (Node *)p;
  r.right = (Node *) p;
  p->jtype = EndRule;
  p->lock = makelocks();
  p->pred_lock = makelocks();
  CurRuleBlk->end = p;
  if ( q!=NULL ) q->rulenum = NumRules;
  zzaArg(zztasp1,7) = r;

  CurBlockID_array[BlkLevel] = (-1);
  CurAltNum_array[BlkLevel] = (-1);
  --BlkLevel;
  altFixup();leFixup();egFixup();
  zzmatch(107);
  inAlt=0;
  zzgettok();

  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==Action) ) {
      zzmatch(Action);
      a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
      require(a!=NULL, "rule rule: cannot allocate error action");
      strcpy(a, zzlextext);
      CurRuleBlk->erraction = a;
      zzgettok();
    }
    else {
      if ( (setwd2[zztoken]&0x8) ) {
      }
      else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while (zztoken==133) {
       eg  = exception_group();

      if ( eg!=NULL ) {
        list_add(&CurExGroups, (void *)eg);
        if (eg->label == NULL || *eg->label=='\0' ) q->has_rule_exception = 1;
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  if ( q==NULL ) zzaArg(zztasp1,0 ).left = NULL; else zzaArg(zztasp1,0) = zzaArg(zztasp1,7);
  CurRuleBlk->exceptions = CurExGroups;
  CurRuleBlk->el_labels = CurElementLabels;
  CurRuleNode->ast_labels_in_actions = CurAstLabelsInActions;
  CurRuleNode = NULL;
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

static void laction()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *a;
  zzmatch(108);
  zzgettok();
  zzmatch(Action);

  a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
  require(a!=NULL, "rule laction: cannot allocate action");
  strcpy(a, zzlextext);
  list_add(&LexActions, a);
  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

static void lmember()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *a;
  zzmatch(109);
  zzgettok();
  zzmatch(Action);

  if (! GenCC) {
    err("Use #lexmember only in C++ mode (to insert code in DLG class header");
  } else {
    a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
    require(a!=NULL, "rule lmember: cannot allocate action");
    strcpy(a, zzlextext);
    list_add(&LexMemberActions, a);
  };

  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

static void lprefix()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *a;
  zzmatch(110);
  zzgettok();
  zzmatch(Action);

  if (! GenCC) {
    err("Use #lexprefix only in C++ mode (to insert code in DLG class header");
  } else {
    a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
    require(a!=NULL, "rule lprefix: cannot allocate action");
    strcpy(a, zzlextext);
    list_add(&LexPrefixActions, a);
  };

  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

static void aPred()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  PredEntry     *predEntry=NULL;
  char          *name=NULL;
  Predicate     *predExpr=NULL;
  char          *predLiteral=NULL;
  int           save_file;
  int           save_line;
  int           predExprPresent=0;
  zzmatch(111);

  MR_usingPredNames=1; /* will need to use -mrhoist version of genPredTree */
  zzgettok();

  zzmatch(TokenTerm);
  name=strdup(zzlextext);
  zzgettok();

  /* don't free - referenced in predicates */

  CurPredName=(char *)calloc(1,strlen(name) + 10);
  strcat(CurPredName,"#pred ");
  strcat(CurPredName,name);

  predEntry=(PredEntry *) hash_get(Pname,name);
  if (predEntry != NULL) {
    warning("#pred %s previously defined - ignored",
            FileStr[action_file], action_line, name);
    name=NULL;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==Pred) ) {
      zzmatch(Pred);
      predLiteral=strdup(zzlextext);
      save_line=action_line;
      save_file=action_file;
      zzgettok();

      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        {
        if ( (setwd3[zztoken]&0x1) ) {
	  predExpr  = predOrExpr();
          predExprPresent=1;
        }
        else {
          if ( (setwd3[zztoken]&0x2) ) {
          }
          else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzasp=zztasp3;
        }
      }
      if (predLiteral != NULL && name != NULL) {

       /*
        *  predExpr may be NULL due to syntax errors
        *    or simply omitted by the user
        */

        predEntry=newPredEntry(name);
        predEntry->file=save_file;
        predEntry->line=save_line;
        predExpr=MR_predFlatten(predExpr);
        predEntry->predLiteral=predLiteral;
        if (! predExprPresent || predExpr == NULL) {
          predExpr=new_pred();
          predExpr->expr=predLiteral;
          predExpr->source=newActionNode();
          predExpr->source->action=predExpr->expr;
          predExpr->source->rname=CurPredName;
          predExpr->source->line=action_line;
          predExpr->source->file=action_file;
          predExpr->source->is_predicate=1;
          predExpr->k=predicateLookaheadDepth(predExpr->source);
        };
        predEntry->pred=predExpr;
        hash_add(Pname,name,(Entry *)predEntry);
        predExpr=NULL;
      };
      predicate_free(predExpr);
    }
    else {
      if ( (setwd3[zztoken]&0x4) ) {
        save_line=zzline; save_file=CurFile;
         predExpr  = predOrExpr();

        if (predExpr != NULL && name != NULL) {
          predEntry=newPredEntry(name);
          predEntry->file=CurFile;
          predEntry->line=zzline;
          predExpr=MR_predFlatten(predExpr);
          predEntry->pred=predExpr;
          hash_add(Pname,name,(Entry *)predEntry);
          predExpr=NULL;
        };
        predicate_free(predExpr);
      }
      else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if (zztoken==107) {
      zzmatch(107);
      zzgettok();
    }
    else {
      if (setwd3[zztoken]&0x8) {
      }
      else {zzFAIL(1,zzerr12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  predicate_free(predExpr);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

static Predicate *predOrExpr()
{
  Predicate *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(Predicate *  ))
  check_overflow(); --zzasp;
  {
  Predicate     *ORnode;
  Predicate     *predExpr;
  Predicate     **tail=NULL;
  predExpr  = predAndExpr();


  ORnode=new_pred();
  ORnode->expr=PRED_OR_LIST;
  if (predExpr != NULL) {
    ORnode->down=predExpr;
    tail=&predExpr->right;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while ( (zztoken==112) ) {
      zzmatch(112);
      zzgettok();
      predExpr  = predAndExpr();


      if (predExpr != NULL) {
        *tail=predExpr;
        tail=&predExpr->right;
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }

  _retv=ORnode;
  ORnode=NULL;
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  predicate_free(ORnode);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  return _retv;
  }
}

static Predicate *predAndExpr()
{
  Predicate *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(Predicate *  ))
  check_overflow(); --zzasp;
  {
  Predicate     *ANDnode;
  Predicate     *predExpr;
  Predicate     **tail=NULL;
  predExpr  = predPrimary();


  ANDnode=new_pred();
  ANDnode->expr=PRED_AND_LIST;
  if (predExpr != NULL) {
    ANDnode->down=predExpr;
    tail=&predExpr->right;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while (zztoken==113) {
      zzmatch(113);
      zzgettok();
      predExpr  = predPrimary();

      if (predExpr != NULL) {
        *tail=predExpr;
        tail=&predExpr->right;
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }

  _retv=ANDnode;
  ANDnode=NULL;
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  predicate_free(ANDnode);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  return _retv;
  }
}

static Predicate *predPrimary()
{
  Predicate *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(Predicate *  ))
  check_overflow(); --zzasp;
  {

  char          *name=NULL;
  PredEntry     *predEntry=NULL;
  Predicate     *predExpr=NULL;
  if ( (zztoken==TokenTerm) ) {
    zzmatch(TokenTerm);
    name=strdup(zzlextext);
    zzgettok();
    predEntry=(PredEntry *) hash_get(Pname,name);
    if (predEntry == NULL) {
      warning("no previously defined #pred with name \"%s\"",
              FileStr[CurFile], zzline, name);
      name=NULL;
      _retv=NULL;
    } else {
      predExpr=predicate_dup(predEntry->pred);
      predExpr->predEntry=predEntry;
      _retv=predExpr;
    }
  }
  else {
    if (zztoken==114) {
      zzmatch(114);
      zzgettok();
      predExpr  = predOrExpr();

      zzmatch(115);
      _retv=predExpr;
      zzgettok();
    }
    else {
      if (zztoken==103) {
        zzmatch(103);
	zzgettok();
        predExpr  = predPrimary();

        predExpr->inverted=!predExpr->inverted;
        _retv=predExpr;
      }
      else {zzFAIL(1,zzerr13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;

  predicate_free(predExpr);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x80);
  return _retv;
  }
}

static void aLexclass()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  zzmatch(116);
  zzgettok();
  zzmatch(TokenTerm);
  lexclass(strdup(zzlextext));
  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x1);
  }
}

static void error()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *t=NULL; ECnode *e; int go=1; TermEntry *p;
  zzmatch(117);
  zzgettok();
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if (zztoken==TokenTerm) {
      zzmatch(TokenTerm);
      t=strdup(zzlextext);
      zzgettok();
    }
    else {
      if ( (zztoken==QuotedTerm) ) {
        zzmatch(QuotedTerm);
        t=strdup(zzlextext);
        zzgettok();

      }
      else {zzFAIL(1,zzerr14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  e = newECnode;
  require(e!=NULL, "cannot allocate error class node");
  e->lexclass = CurrentLexClass;
  if ( Tnum( (t=strip_quotes(t)) ) == 0 )
  {
    if ( hash_get(Texpr, t) != NULL )
      warn(eMsg("errclass name conflicts with regular expression  '%s'",t));
    e->tok = addTname( t );
    set_orel(e->tok, &imag_tokens);
    require((p=(TermEntry *)hash_get(Tname, t)) != NULL,
    "hash table mechanism is broken");
    p->classname = 1; /* entry is errclass name, not token */
    list_add(&eclasses, (char *)e);
  }
  else
  {
    warn(eMsg("redefinition of errclass or conflict w/token or tokclass '%s'; ignored",t));
    free( (char *)e );
    go=0;
  }
  zzmatch(102);
  zzgettok();
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==NonTerminal) ) {
      zzmatch(NonTerminal);
      if ( go ) t=strdup(zzlextext);
      zzgettok();
    }
    else {
      if ( (zztoken==TokenTerm) ) {
        zzmatch(TokenTerm);
        if ( go ) t=strdup(zzlextext);
        zzgettok();
      }
      else {
        if ( (zztoken==QuotedTerm) ) {
          zzmatch(QuotedTerm);
          if ( go ) t=strdup(zzlextext);
          zzgettok();
        }
        else {zzFAIL(1,zzerr15,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzasp=zztasp2;
    }
  }
  if ( go ) list_add(&(e->elist), t);
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while ( (setwd4[zztoken]&0x2) ) {
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;

        if ( (zztoken==NonTerminal) ) {
          zzmatch(NonTerminal);
          if ( go ) t=strdup(zzlextext);
          zzgettok();
        } else {
          if ( (zztoken==TokenTerm) ) {
            zzmatch(TokenTerm);
            if ( go ) t=strdup(zzlextext);
            zzgettok();
          } else {
            if ( (zztoken==QuotedTerm) ) {
              zzmatch(QuotedTerm);
              if ( go ) t=strdup(zzlextext);
              zzgettok();
            } else {
              zzFAIL(1,zzerr16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
              goto fail;
            }
          }
        }
        zzasp=zztasp3;
      }
      if ( go ) list_add(&(e->elist), t);
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(98);
  zzgettok();
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x4);
  }
}

static void tclass()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *t=NULL; TCnode *e; int go=1,tok,totok; TermEntry *p, *term, *toterm;
  char *akaString=NULL; int save_file; int save_line;
  char *totext=NULL;
  zzmatch(118);
  zzgettok();
  zzmatch(TokenTerm);
  t=strdup(zzlextext);
  zzgettok();

  e = newTCnode;
  require(e!=NULL, "cannot allocate token class node");
  e->lexclass = CurrentLexClass;
  if ( Tnum( t ) == 0 )
  {
    e->tok = addTname( t );
    set_orel(e->tok, &imag_tokens);
    set_orel(e->tok, &tokclasses);
    require((p=(TermEntry *)hash_get(Tname, t)) != NULL,
    "hash table mechanism is broken");
    p->classname = 1; /* entry is class name, not token */
    p->tclass = e;    /* save ptr to this tclass def */
    list_add(&tclasses, (char *)e);
  }
  else
  {
    warn(eMsg("redefinition of tokclass or conflict w/token '%s'; ignored",t));
    free( (char *)e );
    go=0;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==114) ) {
      zzmatch(114); zzgettok();
      zzmatch(QuotedTerm);
      akaString=strdup(strip_quotes(zzlextext));
      save_file=CurFile;save_line=zzline;
      zzgettok();

      zzmatch(115); zzgettok();
    }
    else {
      if (zztoken==102) {
      }
      else {zzFAIL(1,zzerr17,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }

  /* MR23 */         if (p!= NULL && akaString != NULL) {
    /* MR23 */           if (p->akaString != NULL) {
      /* MR23 */             if (strcmp(p->akaString,akaString) != 0) {
        /* MR23 */                warning("this #tokclass statment conflicts with a previous #tokclass %s(\"%s\") statement", FileStr[save_file], save_line, t, p->akaString);
        /* MR23 */             };
      /* MR23 */            } else {
      /* MR23 */              p->akaString=akaString;
      /* MR23 */            };
    /* MR23 */          };
  /* MR23 */
  zzmatch(102); zzgettok();
  {
    int zztasp2 = zzasp - 1;
    int zzcnt=1;
    check_overflow(); --zzasp;
    {
     do {
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        {
        if ( (zztoken==TokenTerm) ) {
          zzmatch(TokenTerm);
          if ( go ) {
            term = (TermEntry *) hash_get(Tname, zzlextext);
            if ( term==NULL && UserDefdTokens ) {
              err("implicit token definition not allowed with #tokdefs");
              go = 0;
            }
            else {t=strdup(zzlextext); tok=addTname(zzlextext);}
          }
          zzgettok();

          {
            int zztasp4 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==119) {
              zzmatch(119);
	      zzgettok();
              zzmatch(TokenTerm);
              if ( go ) {
                toterm = (TermEntry *) hash_get(Tname, zzlextext);
                if ( toterm==NULL && UserDefdTokens ) {
                  err("implicit token definition not allowed with #tokdefs");
                  go = 0;
                } else {
                  totext=strdup(zzlextext); totok=addTname(zzlextext);
                }
              }
              zzgettok();
            }
            else {
              if ( (setwd4[zztoken]&0x8) ) {
              }
              else {zzFAIL(1,zzerr18,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp4;
            }
          }
        }
        else {
          if ( (zztoken==QuotedTerm) ) {
            zzmatch(QuotedTerm);
            if ( go ) {
              term = (TermEntry *) hash_get(Texpr, zzlextext);
              if ( term==NULL && UserDefdTokens ) {
                err("implicit token definition not allowed with #tokdefs");
                go = 0;
              }
              else {t=strdup(zzlextext); tok=addTexpr(zzlextext);}
            }
            zzgettok();

          }
          else {zzFAIL(1,zzerr19,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzasp=zztasp3;
        }
      }
      if ( go ) {
        if (totext == NULL) {
          list_add(&(e->tlist), t);
        } else {
          list_add(&(e->tlist),"..");
          list_add(&(e->tlist),t);
          list_add(&(e->tlist),totext);
        }
        totext=NULL;
      }
      zzasp=zztasp2;
    } while (setwd4[zztoken]&0x10);
    zzasp=zztasp2;
    }
  }
  zzmatch(98);
  zzgettok();
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x20);
  }
}

static void token()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  char *t=NULL, *e=NULL, *a=NULL; int tnum=0;
  char *akaString=NULL; TermEntry *te;int save_file=0,save_line=0;
  zzmatch(120);
  tokenActionActive=1;
  zzgettok();

  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==TokenTerm) ) {
      zzmatch(TokenTerm);
      t=strdup(zzlextext);
      zzgettok();

      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;

        if (zztoken==114) {
          zzmatch(114);
          zzgettok();
          zzmatch(QuotedTerm);
          akaString=strdup(strip_quotes(zzlextext));
          save_file=CurFile;
          save_line=zzline;
          zzgettok();

          zzmatch(115);
          zzgettok();
        }
        else if ( !(setwd4[zztoken]&0x40) ) {
            zzFAIL(1,zzerr20,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
            goto fail;
        }
        zzasp=zztasp3;
      }
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;

        if (zztoken==121) {
          zzmatch(121);
          zzgettok();
          zzmatch(122);
          tnum = atoi(zzlextext);
          zzgettok();
        }
        else if ( !(setwd4[zztoken]&0x80) ) {
          zzFAIL(1,zzerr21,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
        }
        zzasp=zztasp3;
      }
    }
    else if ( !(setwd5[zztoken]&0x1) ) {
      zzFAIL(1,zzerr22,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
      goto fail;
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;

    if ( (zztoken==QuotedTerm) ) {
      zzmatch(QuotedTerm);
      e=strdup(zzlextext);
      zzgettok();
    } else if ( !(setwd5[zztoken]&0x2) ) {
      zzFAIL(1,zzerr23,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
      goto fail;
    }

    zzasp=zztasp2;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;

    if ( (zztoken==Action) ) {
      zzmatch(Action);

      a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
      require(a!=NULL, "rule token: cannot allocate action");
      strcpy(a, zzlextext);
      zzgettok();
    }
    else if ( !(setwd5[zztoken]&0x4) ) {
      zzFAIL(1,zzerr24,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
      goto fail;
    }
    zzasp=zztasp2;
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;

    if (zztoken==107) {
      zzmatch(107);
      zzgettok();
    } else if ( !(setwd5[zztoken]&0x8) ) {
      zzFAIL(1,zzerr25,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
      goto fail;
    }
    zzasp=zztasp2;
  }
  chkToken(t, e, a, tnum);
  if (t != NULL) {
    te=(TermEntry *)hash_get(Tname,t);
    if (te != NULL && akaString != NULL) {
      if (te->akaString != NULL) {
        if (strcmp(te->akaString,akaString) != 0) {
          warning("this #token statment conflicts with a previous #token %s(\"%s\") statement",
                FileStr[save_file], save_line, t, te->akaString);
        };
      } else {
        te->akaString=akaString;
      };
    };
  };
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x10);
  }
}

static void block(set *toksrefd, set *rulesrefd)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {

  Graph g, b;
  set saveblah;
  int saveinalt = inAlt;
  ExceptionGroup *eg;
  *toksrefd = empty;
  *rulesrefd = empty;
  set_clr(AST_nodes_refd_in_actions);
  CurBlockID++;
  CurBlockID_array[BlkLevel] = CurBlockID;
  CurAltNum = 1;
  CurAltNum_array[BlkLevel] = CurAltNum;
  saveblah = attribsRefdFromAction;
  attribsRefdFromAction = empty;
  alt( toksrefd,rulesrefd );
  b = g = zzaArg(zztasp1,1);

  if ( ((Junction *)g.left)->p1->ntype == nAction )
  {
    ActionNode *actionNode=(ActionNode *)
    ( ( (Junction *)g.left) ->p1);
    if (!actionNode->is_predicate )
    {
      actionNode->init_action = true;
        if (actionNode->noHoist) {
          errorFL("<<nohoist>> appears as init-action - use <<>> <<nohoist>>",
                    FileStr[actionNode->file],actionNode->line);
        }
    }
  }
  ((Junction *)g.left)->blockid = CurBlockID;
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while ( (zztoken==133) ) {
       eg  = exception_group();


      if ( eg!=NULL ) {
        /* MR7 *****        eg->altID = makeAltID(CurBlockID,CurAltNum);        *****/
        /* MR7 *****    CurAltStart->exception_label = eg->altID;           *****/
        list_add(&CurExGroups, (void *)eg);
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  CurAltNum++;
  CurAltNum_array[BlkLevel] = CurAltNum;
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while (zztoken==123) {
      zzmatch(123);
      inAlt=1;
      zzgettok();

      alt( toksrefd,rulesrefd );
      g = Or(g, zzaArg(zztasp2,2));

      ((Junction *)g.left)->blockid = CurBlockID;
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        {
        while (zztoken==133) {
           eg  = exception_group();


          if ( eg!=NULL ) {
            /* MR7 *****        eg->altID = makeAltID(CurBlockID,CurAltNum);        *****/
            /* MR7 *****    CurAltStart->exception_label = eg->altID;           *****/
            list_add(&CurExGroups, (void *)eg);
          }
          zzasp=zztasp3;
        }
        zzasp=zztasp3;
        }
      }
      CurAltNum++;
      CurAltNum_array[BlkLevel] = CurAltNum;
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzaArg(zztasp1,0) = b;
  attribsRefdFromAction = saveblah; inAlt = saveinalt;
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x20);
  }
}

static void alt(set * toksrefd,set * rulesrefd)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  int n=0; Graph g; int e_num=0, old_not=0; Node *node; set elems, dif;
  int first_on_line = 1, use_def_MT_handler = 0;
  g.left=NULL; g.right=NULL;

      CurAltStart = NULL;
  elems = empty;
  inAlt = 1;
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if ( (zztoken==88) ) {
      zzmatch(88);
      use_def_MT_handler = 1;
      zzgettok();
    }
    else {
      if ( (setwd5[zztoken]&0x40) ) {
      }
      else {zzFAIL(1,zzerr26,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while ( (setwd5[zztoken]&0x80) ) {
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        {
        old_not=0;
        if ( (zztoken==124) ) {
          zzmatch(124);
          old_not=1;
          zzgettok();
        }
        else {
          if ( (setwd6[zztoken]&0x1) ) {
          }
          else {zzFAIL(1,zzerr27,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzasp=zztasp3;
        }
      }
       node  = element( old_not, first_on_line, use_def_MT_handler );

      if ( node!=NULL && node->ntype!=nAction ) first_on_line = 0;

      if ( zzaArg(zztasp2,2 ).left!=NULL ) {
        g = Cat(g, zzaArg(zztasp2,2));
        n++;
        if ( node!=NULL ) {
          if ( node->ntype!=nAction ) e_num++;
          /* record record number of all rule and token refs */
          if ( node->ntype==nToken ) {
            TokNode *tk = (TokNode *)((Junction *)zzaArg(zztasp2,2 ).left)->p1;
            tk->elnum = e_num;
            set_orel(e_num, &elems);
          }
          else if ( node->ntype==nRuleRef ) {
            RuleRefNode *rn = (RuleRefNode *)((Junction *)zzaArg(zztasp2,2 ).left)->p1;
            rn->elnum = e_num;
            set_orel(e_num,  rulesrefd);
          }
        }
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  if ( n == 0 ) g = emptyAlt();
  zzaArg(zztasp1,0) = g;
  /* We want to reduce number of LT(i) calls and the number of
  * local attribute variables in C++ mode (for moment, later we'll
  * do for C also).  However, if trees are being built, they
  * require most of the attrib variables to create the tree nodes
  * with; therefore, we gen a token ptr for each token ref in C++
  */
  if ( GenCC && !GenAST )
  {
  /* This now free's the temp set -ATG 5/6/95 */
  set temp;
  temp = set_and(elems, attribsRefdFromAction);
  set_orin( toksrefd, temp);
  set_free(temp);
}
else set_orin( toksrefd, elems);
if ( GenCC ) {
  dif = set_dif(attribsRefdFromAction, elems);
  if ( set_deg(dif)>0 )
  err("one or more $i in action(s) refer to non-token elements");
  set_free(dif);
}
set_free(elems);
set_free(attribsRefdFromAction);
inAlt = 0;
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd6, 0x2);
  }
}

static LabelEntry *element_label()
{
  LabelEntry *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(LabelEntry *  ))
  check_overflow(); --zzasp;
  {
  TermEntry *t=NULL; LabelEntry *l=NULL; RuleEntry *r=NULL; char *lab;
  zzmatch(LABEL);
  lab = strdup(zzlextext);
  zzgettok();


  UsedNewStyleLabel = 1;
  if ( UsedOldStyleAttrib ) err("cannot mix with new-style labels with old-style $i");
  t = (TermEntry *) hash_get(Tname, lab);
  if ( t==NULL ) t = (TermEntry *) hash_get(Texpr, lab);
  if ( t==NULL ) r = (RuleEntry *) hash_get(Rname, lab);
  if ( t!=NULL ) {
    err(eMsg("label definition clashes with token/tokclass definition: '%s'", lab));
    _retv = NULL;
  }
  else if ( r!=NULL ) {
    err(eMsg("label definition clashes with rule definition: '%s'", lab));
    _retv = NULL;
  }
  else {
    /* we don't clash with anybody else */
    l = (LabelEntry *) hash_get(Elabel, lab);
    if ( l==NULL ) {  /* ok to add new element label */
    l = (LabelEntry *)hash_add(Elabel,
    lab,
    (Entry *)newLabelEntry(lab));
    /* add to list of element labels for this rule */
    list_add(&CurElementLabels, (void *)lab);
    /* MR7 */       leAdd(l);  /* list of labels waiting for exception group definitions */
    _retv = l;
  }
  else {
  err(eMsg("label definitions must be unique per rule: '%s'", lab));
  _retv = NULL;
}
}
  zzmatch(106);
  zzgettok();
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd6, 0x4);
  return _retv;
  }
}

static Node *element(int old_not,int first_on_line,int use_def_MT_handler)
{
  Node *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(Node *  ))
  check_overflow(); --zzasp;
  {

  Attrib blk;
  Predicate *pred = NULL;
  int local_use_def_MT_handler=0;
  ActionNode *act;
  RuleRefNode *rr;
  set toksrefd, rulesrefd;
  TermEntry *term;
  TokNode *p=NULL; RuleRefNode *q; int approx=0;
  LabelEntry *label=NULL;
  int predMsgDone=0;
  int semDepth=0;
  int ampersandStyle;
  int height;
  int equal_height;

  char* pFirstSetSymbol = NULL;

  _retv = NULL;
  if ( (setwd6[zztoken]&0x8) ) {
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;
      {
      if ( (zztoken==LABEL) ) {
         label  = element_label();

      }
      else {
        if ( (setwd6[zztoken]&0x10) ) {
        }
        else {zzFAIL(1,zzerr28,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
      zzasp=zztasp2;
      }
    }
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;
      {
      if ( (zztoken==TokenTerm) ) {
        zzmatch(TokenTerm);

        term = (TermEntry *) hash_get(Tname, zzlextext);
        if ( term==NULL && UserDefdTokens ) {
          err("implicit token definition not allowed with #tokdefs");
          zzaRet.left = zzaRet.right = NULL;
        }
        else {
          zzaRet = buildToken(zzlextext);
          p=((TokNode *)((Junction *)zzaRet.left)->p1);
          term = (TermEntry *) hash_get(Tname, zzlextext);
          require( term!= NULL, "hash table mechanism is broken");
          p->tclass = term->tclass;
          p->complement =  old_not;
          if ( label!=NULL ) {
            p->el_label = label->str;
            label->elem = (Node *)p;
          }
        }
        zzgettok();

        {
          int zztasp3 = zzasp - 1;
          check_overflow(); --zzasp;
          {
          if (zztoken==119) {
            zzmatch(119);
	    zzgettok();
            {
              int zztasp4 = zzasp - 1;
              check_overflow(); --zzasp;
              {
              if ( (zztoken==QuotedTerm) ) {
                zzmatch(QuotedTerm);
                if ( p!=NULL ) setUpperRange(p, zzlextext);
                zzgettok();
              }
              else {
                if ( (zztoken==TokenTerm) ) {
                  zzmatch(TokenTerm);
                  if ( p!=NULL ) setUpperRange(p, zzlextext);
                  zzgettok();
                }
                else {zzFAIL(1,zzerr29,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
              zzasp=zztasp4;
              }
            }
          }
          else {
            if ( (setwd6[zztoken]&0x20) ) {
            }
            else {zzFAIL(1,zzerr30,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzasp=zztasp3;
          }
        }

        if ( p!=NULL && (p->upper_range!=0 || p->tclass ||  old_not) )
        list_add(&MetaTokenNodes, (void *)p);
        {
          int zztasp3 = zzasp - 1;
          check_overflow(); --zzasp;
          {
          if (zztoken==125) {
            zzmatch(125);
            if ( p!=NULL ) p->astnode=ASTroot;
	    zzgettok(); 
          }
          else {
            if (setwd6[zztoken]&0x40) {
              if ( p!=NULL ) p->astnode=ASTchild;
            }
            else {
              if (zztoken==103) {
                zzmatch(103);
                if ( p!=NULL ) p->astnode=ASTexclude;
                zzgettok();
              }
              else {zzFAIL(1,zzerr31,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
          zzasp=zztasp3;
          }
        }
        {
          int zztasp3 = zzasp - 1;
          check_overflow(); --zzasp;
          {
          if (zztoken==88) {
            zzmatch(88);
            local_use_def_MT_handler = 1;
            zzgettok();
          }
          else {
            if (setwd6[zztoken]&0x80) {
            }
            else {zzFAIL(1,zzerr32,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzasp=zztasp3;
          }
        }

        if ( p!=NULL &&  first_on_line ) {
          CurAltStart = (Junction *)zzaRet.left;
          altAdd(CurAltStart);
          p->altstart = CurAltStart;
        }
        if ( p!=NULL )
        p->use_def_MT_handler =  use_def_MT_handler || local_use_def_MT_handler;
        _retv = (Node *)p;
      }
      else {
        if ( (zztoken==QuotedTerm) ) {
          zzmatch(QuotedTerm);

          term = (TermEntry *) hash_get(Texpr, zzlextext);
          if ( term==NULL && UserDefdTokens ) {
            err("implicit token definition not allowed with #tokdefs");
            zzaRet.left = zzaRet.right = NULL;
          }
          else {
            zzaRet = buildToken(zzlextext); p=((TokNode *)((Junction *)zzaRet.left)->p1);
            p->complement =  old_not;
            if ( label!=NULL ) {
              p->el_label = label->str;
              label->elem = (Node *)p;
            }
          }
          zzgettok();
          {
            int zztasp3 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==119) {
              zzmatch(119);
	      zzgettok();
              {
                int zztasp4 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if (zztoken==QuotedTerm) {
                  zzmatch(QuotedTerm);
                  if ( p!=NULL ) setUpperRange(p, zzlextext);
                  zzgettok();
                }
                else {
                  if (zztoken==TokenTerm) {
                    zzmatch(TokenTerm);
                    if ( p!=NULL ) setUpperRange(p, zzlextext);
                    zzgettok();
                  }
                  else {zzFAIL(1,zzerr33,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
                zzasp=zztasp4;
                }
              }
            }
            else {
              if (setwd7[zztoken]&0x1) {
              }
              else {zzFAIL(1,zzerr34,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp3;
            }
          }
          {
            int zztasp3 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==125) {
              zzmatch(125);
              if ( p!=NULL ) p->astnode=ASTroot;
              zzgettok();
            }
            else {
              if (setwd7[zztoken]&0x2) {
                if ( p!=NULL ) p->astnode=ASTchild;
              }
              else {
                if (zztoken==103) {
                  zzmatch(103);
                  if ( p!=NULL ) p->astnode=ASTexclude;
                  zzgettok();

                }
                else {zzFAIL(1,zzerr35,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
            }
            zzasp=zztasp3;
            }
          }
          {
            int zztasp3 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==88) {
              zzmatch(88);
              local_use_def_MT_handler = 1;
              zzgettok();
            }
            else {
              if (setwd7[zztoken]&0x4) {
              }
              else {zzFAIL(1,zzerr36,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp3;
            }
          }

          if ( p!=NULL && (p->upper_range!=0 || p->tclass ||  old_not) )
          list_add(&MetaTokenNodes, (void *)p);

          if (  first_on_line ) {
            CurAltStart = (Junction *)zzaRet.left;
            altAdd(CurAltStart);
            p->altstart = CurAltStart;
          }
          if ( p!=NULL )
          p->use_def_MT_handler =  use_def_MT_handler || local_use_def_MT_handler;
          _retv = (Node *)p;
        }
        else {
          if ( (zztoken==WildCard) ) {
            if (  old_not ) warn("~ WILDCARD is an undefined operation (implies 'nothing')");
            zzmatch(WildCard);
            zzaRet = buildWildCard(zzlextext); p=((TokNode *)((Junction *)zzaRet.left)->p1);
            zzgettok();

            {
              int zztasp3 = zzasp - 1;
              check_overflow(); --zzasp;
              {
              if (zztoken==125) {
                zzmatch(125);
                p->astnode=ASTroot;
                zzgettok();
              }
              else {
                if ( (setwd7[zztoken]&0x8) ) {
                  p->astnode=ASTchild;
                }
                else {
                  if (zztoken==103) {
                    zzmatch(103);
                    p->astnode=ASTexclude;
                    zzgettok();
                  }
                  else {zzFAIL(1,zzerr37,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
              }
              zzasp=zztasp3;
              }
            }
            list_add(&MetaTokenNodes, (void *)p);

            if (  first_on_line ) {
              CurAltStart = (Junction *)zzaRet.left;
              altAdd(CurAltStart);                                 /* MR7 */
              p->altstart = CurAltStart;
              if ( label!=NULL ) {
                p->el_label = label->str;
                label->elem = (Node *)p;
              }
            }
            _retv = (Node *)p;
          }
          else {
            if (zztoken==NonTerminal) {
              if (  old_not ) warn("~ NONTERMINAL is an undefined operation");
              zzmatch(NonTerminal);
              zzaRet = buildRuleRef(zzlextext);
              zzgettok();

              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if (zztoken==103) {
                  zzmatch(103);
                  q = (RuleRefNode *) ((Junction *)zzaRet.left)->p1;
                  q->astnode=ASTexclude;
		  zzgettok();

                }
                else {
                  if (setwd7[zztoken]&0x10) {
                  }
                  else {zzFAIL(1,zzerr38,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
                zzasp=zztasp3;
                }
              }
              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if (setwd7[zztoken]&0x20) {
                  {
                    int zztasp4 = zzasp - 1;
                    check_overflow(); --zzasp;
                    {
                    if (zztoken==104) {
                      zzmatch(104);
		      zzgettok();
                    }
                    else {
                      if ( (zztoken==PassAction) ) {
                      }
                      else {zzFAIL(1,zzerr39,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                    }
                    zzasp=zztasp4;
                    }
                  }
                  zzmatch(PassAction);
                  addParm(((Junction *)zzaRet.left)->p1, zzlextext);
                  zzgettok();
                }
                else {
                  if (setwd7[zztoken]&0x40) {
                  }
                  else {zzFAIL(1,zzerr40,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
                zzasp=zztasp3;
                }
              }
              rr=(RuleRefNode *) ((Junction *)zzaRet.left)->p1;
              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                char *a;
                if (zztoken==105) {
                  zzmatch(105);
		  zzgettok();
                  zzmatch(PassAction);

                  a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
                  require(a!=NULL, "rule element: cannot allocate assignment");
                  strcpy(a, zzlextext);
                  rr->assign = a;
                  zzgettok();
                }
                else {
                  if (setwd7[zztoken]&0x80) {
                  }
                  else {zzFAIL(1,zzerr41,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
                zzasp=zztasp3;
                }
              }

              if ( label!=NULL ) {
                rr->el_label = label->str;
                label->elem = (Node *)rr;
              }
              if (  first_on_line ) {
                CurAltStart = (Junction *)zzaRet.left;
                altAdd(CurAltStart);                                 /* MR7 */
                ((RuleRefNode *)((Junction *)zzaRet.left)->p1)->altstart = CurAltStart;
              }
              _retv = (Node *)rr;
            }
            else {zzFAIL(1,zzerr42,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
      zzasp=zztasp2;
      }
    }
  }
  else {
    if (zztoken==Action) {
      if (  old_not ) warn("~ ACTION is an undefined operation");
      zzmatch(Action);
      zzaArg(zztasp1,0) = buildAction(zzlextext,action_file,action_line, 0);
      zzgettok();

      if (  first_on_line ) {
        CurAltStart = (Junction *)zzaArg(zztasp1,0 ).left;                   /* MR7 */
        altAdd(CurAltStart);
      }
      _retv = (Node *) ((Junction *)zzaArg(zztasp1,0 ).left)->p1;
    }
    else {
      if (zztoken==Pred) {
        if (  old_not ) warn("~ SEMANTIC-PREDICATE is an undefined operation");
        zzmatch(Pred);
        zzaArg(zztasp1,0) = buildAction(zzlextext,action_file,action_line, 1);
	zzgettok();

        act = (ActionNode *) ((Junction *)zzaArg(zztasp1,0 ).left)->p1;
        if (numericActionLabel) {
          list_add(&NumericPredLabels,act);
          numericActionLabel=0;
        }
        {
          int zztasp2 = zzasp - 1;
          check_overflow(); --zzasp;
          {
          char *a;
          if (zztoken==PassAction) {
            zzmatch(PassAction);

            a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
            require(a!=NULL, "rule element: cannot allocate predicate fail action");
            strcpy(a, zzlextext);
            act->pred_fail = a;
            zzgettok();
          }
          else {
            if ( (setwd8[zztoken]&0x1) ) {
            }
            else {zzFAIL(1,zzerr43,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzasp=zztasp2;
          }
        }
        if (  first_on_line ) {                                /* MR7 */
          CurAltStart = (Junction *)zzaArg(zztasp1,0 ).left;                   /* MR7 */
          altAdd(CurAltStart);                                 /* MR7 */
        };
        _retv = (Node *)act;
      }
      else {
        if ( (setwd8[zztoken]&0x2) ) {
          if (  old_not ) warn("~ BLOCK is an undefined operation");
          BlkLevel++;
          if (BlkLevel >= MAX_BLK_LEVEL) antlr_fatal("Blocks nested too deeply");
          CurBlockID_array[BlkLevel] = CurBlockID;
          CurAltNum_array[BlkLevel] = CurAltNum;
          {
            int zztasp2 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==Pragma) {
              zzmatch(Pragma);
	      zzgettok();
              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if (zztoken==126) {
                  zzmatch(126);
                  approx=LL_k;
                  zzgettok();
                }
                else {
                  if (zztoken==127) {
                    zzmatch(127);
                    approx = 1;
                    zzgettok();
                  }
                  else {
                    if (zztoken==128) {
                      zzmatch(128);
                      approx = 2;
                      zzgettok();
                    }
                    else {zzFAIL(1,zzerr44,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                  }
                }
                zzasp=zztasp3;
                }
              }
            }
            else {
              if (setwd8[zztoken]&0x4) {
              }
              else {zzFAIL(1,zzerr45,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp2;
            }
          }
          {
            int zztasp2 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==FirstSetSymbol) {
              zzmatch(FirstSetSymbol);
	      zzgettok();
              zzmatch(114);
	      zzgettok();
              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if ( (zztoken==NonTerminal) ) {
                  zzmatch(NonTerminal);

                  pFirstSetSymbol = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
                  require(pFirstSetSymbol!=NULL, "cannot allocate first set name");
                  strcpy(pFirstSetSymbol, zzlextext);

                  zzgettok();
                }
                else {
                  if ( (zztoken==TokenTerm) ) {
                    zzmatch(TokenTerm);

                    pFirstSetSymbol = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
                    require(pFirstSetSymbol!=NULL, "cannot allocate first set name");
                    strcpy(pFirstSetSymbol, zzlextext);

                    zzgettok();
                  }
                  else {zzFAIL(1,zzerr46,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
                zzasp=zztasp3;
                }
              }
              zzmatch(115);
	      zzgettok();
            }
            else {
              if ( (setwd8[zztoken]&0x8) ) {
              }
              else {zzFAIL(1,zzerr47,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp2;
            }
          }
          {
            int zztasp2 = zzasp - 1;
            check_overflow(); --zzasp;
            {
            if (zztoken==114) {
              zzmatch(114);
	      zzgettok();
              block( &toksrefd,&rulesrefd );
              zzmatch(115);
              blk = zzaRet = zzaArg(zztasp2,2);
              CurBlockID_array[BlkLevel] = (-1);
              CurAltNum_array[BlkLevel] = (-1);
              --BlkLevel;
	      zzgettok();
              {
                int zztasp3 = zzasp - 1;
                check_overflow(); --zzasp;
                {
                if (zztoken==129) {
                  zzmatch(129);
                  zzaRet = makeLoop(zzaRet,approx,pFirstSetSymbol);
                  zzgettok();
                }
                else {
                  if (zztoken==130) {
                    zzmatch(130);
                    zzaRet = makePlus(zzaRet,approx,pFirstSetSymbol);
                    zzgettok();
                  }
                  else {
                    if (zztoken==131) {
                      zzmatch(131);
		      zzgettok();
                      {
                        int zztasp4 = zzasp - 1;
                        check_overflow(); --zzasp;
                        {
                        if ( (setwd8[zztoken]&0x10) ) {
                          {
                            int zztasp5 = zzasp - 1;
                            check_overflow(); --zzasp;
                            {
                            if (zztoken==132) {
                              zzmatch(132);
                              ampersandStyle=0;
                              zzgettok();
                            }
                            else {
                              if (zztoken==113) {
                                zzmatch(113);
                                ampersandStyle=1;
                                zzgettok();
                              }
                              else {zzFAIL(1,zzerr48,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                            }
                            zzasp=zztasp5;
                            }
                          }
                          zzmatch(Pred);
                          zzaRet = buildAction(zzlextext,action_file,action_line,1);
                          zzgettok();

                          act = (ActionNode *) ((Junction *)zzaRet.left)->p1;
                          semDepth=predicateLookaheadDepth(act);
                          if (numericActionLabel) {
                            list_add(&NumericPredLabels,act);
                            numericActionLabel=0;
                          }
                          {
                            int zztasp5 = zzasp - 1;
                            check_overflow(); --zzasp;
                            {
                            char *a;
                            if ( (zztoken==PassAction) ) {
                              zzmatch(PassAction);

                              a = (char *)calloc(strlen(zzlextext)+1, sizeof(char));
                              require(a!=NULL, "rule element: cannot allocate predicate fail action");
                              strcpy(a, zzlextext);
                              act->pred_fail = a;
                              zzgettok();
                            }
                            else {
                              if ( (setwd8[zztoken]&0x20) ) {
                              }
                              else {zzFAIL(1,zzerr49,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                            }
                            zzasp=zztasp5;
                            }
                          }
                          if ( first_on_line) {
                            CurAltStart=(Junction *)zzaRet.left;
                            altAdd(CurAltStart);
                          }
                          _retv = (Node *)act;

                          pred = computePredFromContextGuard(blk,&predMsgDone);           /* MR10 */
                          if ( pred==NULL) {                                              /* MR10 */
                            if ( !predMsgDone) err("invalid or missing context guard");   /* MR10 */
                            predMsgDone=1;                                                /* MR10 */
                          } else {                                                        /* MR10 */
                            act->guardNodes=(Junction *)blk.left;                       /* MR11 */
                            pred->expr = act->action;
                            pred->source = act;
                            /* MR10 */                  pred->ampersandStyle = ampersandStyle;  /* 0 means (g)? => ... 1 means (g)? && ... */
                            /* MR13 */                  if (pred->tcontext != NULL) {
                              /* MR13 */                    height=MR_max_height_of_tree(pred->tcontext);
                              /* MR13 */                    equal_height=MR_all_leaves_same_height(pred->tcontext,height);
                              /* MR13 */                    if (! equal_height) {
                                /* MR13 */                       errorFL("in guarded predicates all tokens in the guard must be at the same height",
                                /* MR13 */                              FileStr[act->file],act->line);
                                /* MR13 */                    };
                              /* MR13 */                  }
                            /* MR10 */                  if (ampersandStyle) {
                              /* MR10 */              act->ampersandPred = pred;
                              /* MR11 */                    if (! HoistPredicateContext) {
                                /* MR11 */                      errorFL("without \"-prc on\" (guard)? && <<pred>>? ... doesn't make sense",
                                /* MR11 */                              FileStr[act->file],act->line);
                                /* MR11 */                    };
                              /* MR10 */                  } else {
                              /* MR10 */              act->guardpred = pred;
                              /* MR10 */                  };
                            /* MR10 */                  if (pred->k != semDepth) {
                              /* MR10 */                     warn(eMsg("length of guard (%d) does not match the length of semantic predicate (%d)",
                              /* MR10 */                                  pred->k,semDepth));
                              /* MR10 */                  };
                          }
                        }
                        else {
                          if ( (setwd8[zztoken]&0x40) ) {
                            zzaRet = makeBlk(zzaRet,approx,pFirstSetSymbol);
                            FoundGuessBlk = 1;
                            ((Junction *) ((Junction *)zzaRet.left)->p1)->guess=1;
                            if ( ! first_on_line ) {
                              err("(...)? predicate must be first element of production");
                            }
                          }
                          else {zzFAIL(1,zzerr50,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                        }
                        zzasp=zztasp4;
                        }
                      }
                    }
                    else {
                      if ( (setwd8[zztoken]&0x80) ) {
                        zzaRet = makeBlk(zzaRet,approx,pFirstSetSymbol);
                      }
                      else {zzFAIL(1,zzerr51,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                    }
                  }
                }
                zzasp=zztasp3;
                }
              }

              if ( pred==NULL && !predMsgDone) {                                      /* MR10 */
                ((Junction *)((Junction *)zzaRet.left)->p1)->blockid = CurBlockID;
                ((Junction *)((Junction *)zzaRet.left)->p1)->tokrefs = toksrefd;
                ((Junction *)((Junction *)zzaRet.left)->p1)->rulerefs = rulesrefd;
                if (  first_on_line ) {                         /* MR7 */
                  CurAltStart = (Junction *)((Junction *)((Junction *)zzaRet.left)->p1);  /* MR7 */
                  altAdd(CurAltStart);                         /* MR7 */
                };                                              /* MR7 */
                _retv = (Node *) ((Junction *)zzaRet.left)->p1;
              }
            }
            else {
              if ( (zztoken==102) ) {
                zzmatch(102);
		zzgettok();
                block( &toksrefd,&rulesrefd );
                zzaRet = makeOpt(zzaArg(zztasp2,2),approx,pFirstSetSymbol);
                CurBlockID_array[BlkLevel] = (-1);
                CurAltNum_array[BlkLevel] = (-1);
                --BlkLevel;
                zzmatch(98);

                ((Junction *)((Junction *)zzaRet.left)->p1)->blockid = CurBlockID;
                ((Junction *)((Junction *)zzaRet.left)->p1)->tokrefs = toksrefd;
                ((Junction *)((Junction *)zzaRet.left)->p1)->rulerefs = rulesrefd;
		zzgettok();

                if (  first_on_line ) {                            /* MR7 */
                  CurAltStart = (Junction *) ((Junction *)((Junction *)zzaRet.left)->p1);  /* MR7 */
                  altAdd(CurAltStart);                             /* MR7 */
                };
                _retv = (Node *) ((Junction *)zzaRet.left)->p1;
              }
              else {zzFAIL(1,zzerr52,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
            zzasp=zztasp2;
            }
          }
        }
        else {
          if (zztoken==129) {
            zzmatch(129);
            warn("don't you want a ')' with that '*'?"); CannotContinue=true;
            zzgettok();
          }
          else {
            if (zztoken==130) {
              zzmatch(130);
              warn("don't you want a ')' with that '+'?"); CannotContinue=true;
              zzgettok();
            }
            else {
              if (zztoken==105) {
                zzmatch(105);
                warn("'>' can only appear after a nonterminal"); CannotContinue=true;
                zzgettok();
              }
              else {
                if (zztoken==PassAction) {
                  zzmatch(PassAction);
                  warn("[...] out of context 'rule > [...]'");
                  CannotContinue=true;
		  zzgettok();
                }
                else {zzFAIL(1,zzerr53,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
            }
          }
        }
      }
    }
  }
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd9, 0x1);
  return _retv;
  }
}

static void default_exception_handler()
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
   DefaultExGroup  = exception_group();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd9, 0x2);
  }
}

static ExceptionGroup *exception_group()
{
  ExceptionGroup *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(ExceptionGroup *  ))
  check_overflow(); --zzasp;
  {
  ExceptionHandler *h; LabelEntry *label=NULL;
  FoundException = 1; FoundExceptionGroup = 1;
  zzmatch(133);
  _retv = (ExceptionGroup *)calloc(1, sizeof(ExceptionGroup));
  zzgettok();

  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    char *p;
    if (zztoken==PassAction) {
      zzmatch(PassAction);

      p = zzlextext+1;
      p[strlen(p)-1] = '\0';    /* kill trailing space */
      label = (LabelEntry *) hash_get(Elabel, zzlextext+1);
      if ( label==NULL )
      {
        err(eMsg("unknown label in exception handler: '%s'", zzlextext+1));
      }
      zzgettok();

    }
    else {
      if ( (setwd9[zztoken]&0x4) ) {
      }
      else {zzFAIL(1,zzerr54,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while (zztoken==135) {
       h  = exception_handler();

      list_add(&(_retv->handlers), (void *)h);
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if (zztoken==134) {
      zzmatch(134);
      zzgettok();
      zzmatch(106);
      zzgettok();
      zzmatch(Action);
      {
        ExceptionHandler *eh = (ExceptionHandler *)
        calloc(1, sizeof(ExceptionHandler));
        char *a = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
        require(eh!=NULL, "exception: cannot allocate handler");
        require(a!=NULL, "exception: cannot allocate action");
        strcpy(a, zzlextext);
        eh->action = a;
        eh->signalname = (char *) calloc(strlen("default")+1, sizeof(char));
        require(eh->signalname!=NULL, "exception: cannot allocate sig name");
        strcpy(eh->signalname, "default");
        list_add(&(_retv->handlers), (void *)eh);
      }
      zzgettok();

    }
    else {
      if (setwd9[zztoken]&0x8) {
      }
      else {zzFAIL(1,zzerr55,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }

  if ( label!=NULL ) {
    /* Record ex group in sym tab for this label */
    if ( label->ex_group!=NULL ) {
      err(eMsg("duplicate exception handler for label '%s'",label->str));
    } else {
      label->ex_group = _retv;
      /* Label the exception group itself */
      _retv->label = label->str;
      /* Make the labelled element pt to the exception also */
      /* MR6 */   if (label->elem == NULL) {
        /* MR6 */      err(eMsg("reference in exception handler to undefined label '%s'",label->str));
        /* MR6 */   } else {
        switch ( label->elem->ntype ) {
          case nRuleRef :
          {
            RuleRefNode *r = (RuleRefNode *)label->elem;
            r->ex_group = _retv;
            break;
          }
          case nToken :
          {
            TokNode *t = (TokNode *)label->elem;
            t->ex_group = _retv;
            break;
          }
        } /* end switch */
        /* MR6 */   }; /* end test on label->elem */
    } /* end test on label->ex_group */

    } /* end test on exception label */

  /* MR7 */   if (BlkLevel == 1 && label == NULL) {
    /* MR7 */     _retv->forRule=1;
    /* MR7 */   } else if (label == NULL) {
    /* MR7 */     _retv->altID = makeAltID(CurBlockID_array[BlkLevel], CurAltNum_array[BlkLevel]);
    /* MR7 */     egAdd(_retv);
    /* MR7 */   } else {
    /* MR7 */     _retv->labelEntry=label;
    /* MR7 */   };

  /* MR7 */     /* You may want to remove this exc from the rule list  */
  /* MR7 */   /* and handle at the labeled element site.             */

  /* MR7 */   if (label != NULL) {
    /* MR7 */     _retv = NULL;
    /* MR7 */   };
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd9, 0x10);
  return _retv;
  }
}

static ExceptionHandler *exception_handler()
{
  ExceptionHandler *   _retv;
  zzRULE;
  int zztasp1 = zzasp - 1;
  PCCTS_PURIFY(_retv,sizeof(ExceptionHandler *  ))
  check_overflow(); --zzasp;
  {
  zzmatch(135);

  _retv = (ExceptionHandler *)calloc(1, sizeof(ExceptionHandler));
  require(_retv!=NULL, "exception: cannot allocate handler");
  zzgettok();

  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    if (zztoken==NonTerminal) {
      zzmatch(NonTerminal);

      _retv->signalname = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
      require(_retv->signalname!=NULL, "exception: cannot allocate sig name");
      strcpy(_retv->signalname, zzlextext);
      zzgettok();
    }
    else {
      if (zztoken==TokenTerm) {
        zzmatch(TokenTerm);

        _retv->signalname = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
        require(_retv->signalname!=NULL, "exception: cannot allocate sig name");
        strcpy(_retv->signalname, zzlextext);
        zzgettok();
      }
      else {zzFAIL(1,zzerr56,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(106);
  zzgettok();
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    _retv->action = NULL;
    if (zztoken==Action) {
      zzmatch(Action);

      _retv->action = (char *) calloc(strlen(zzlextext)+1, sizeof(char));
      require(_retv->action!=NULL, "exception: cannot allocate action");
      strcpy(_retv->action, zzlextext);
      zzgettok();

    }
    else {
      if (setwd9[zztoken]&0x20) {
      }
      else {zzFAIL(1,zzerr57,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzasp=zztasp2;
    }
  }
  zzasp=zztasp1;
  return _retv;
fail:
  zzasp=zztasp1;
  CannotContinue=true;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd9, 0x40);
  return _retv;
  }
}

static void enum_file(char *fname)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow();
  --zzasp;

  if (setwd9[zztoken]&0x80) {
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;
      {
      if (zztoken==143) {
        zzmatch(143);
        zzgettok();
        zzmatch(ID);
        zzgettok();

        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        if (zztoken==149) {
          zzmatch(149);
          zzgettok();
          zzmatch(ID);
          zzgettok();
        } else if (!(setwd10[zztoken]&0x1)) {
          zzFAIL(1,zzerr58,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
        }
        zzasp=zztasp3;

      } else if (!(setwd10[zztoken]&0x2)) {
          zzFAIL(1,zzerr59,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
      }
      zzasp=zztasp2;
      }
    }
    {
      int zztasp2 = zzasp - 1;
      check_overflow(); --zzasp;

      if (zztoken==151) {
        int zztasp3 = zzasp - 1;
        int zzcnt=1;
        check_overflow(); --zzasp;
        do {
          enum_def(  fname );
          zzasp=zztasp3;
        } while (zztoken==151);
        zzasp=zztasp3;
      } else if (zztoken==149) {
          defines(  fname );
      } else {
          zzFAIL(1,zzerr60,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
      }

      zzasp=zztasp2;
    }
  } else if (zztoken!=Eof) {
    zzFAIL(1,zzerr61,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
    goto fail; 
  }
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd10, 0x4);
}


static void defines(char * fname)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  int v; int maxt=(-1); char *t;

  {
    int zztasp2 = zzasp - 1;
    int zzcnt=1;
    check_overflow(); --zzasp;
    {
    do {
      zzmatch(149);
      zzgettok();
      zzmatch(ID);
      t = strdup(zzlextext);
      zzgettok();

      zzmatch(INT);

      v = atoi(zzlextext);

      if ( ! isDLGmaxToken(t)) {
        TokenNum = v;
        if ( v>maxt ) maxt=v;
        if ( Tnum( t ) == 0 ) {
          addForcedTname( t, v );
        } else {
          warning("redefinition of token %s; ignored", fname, zzline, t);
        }
      }
      zzgettok();

      zzasp=zztasp2;
    } while (zztoken==149);
      zzasp=zztasp2;
    }
  }
  TokenNum = maxt + 1;
  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd10, 0x8);
}

static void enum_def(char * fname)
{
  zzRULE;
  int zztasp1 = zzasp - 1;
  check_overflow(); --zzasp;
  {
  int v= 0; int maxt=(-1); char *t;
  zzmatch(151);
  zzgettok();
  zzmatch(ID);
  zzgettok();
  zzmatch(152);
  zzgettok();
  zzmatch(ID);
  t = strdup(zzlextext);
  zzgettok();

  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;

    if (zztoken==153) {
      zzmatch(153);
      zzgettok();
      zzmatch(INT);
      v=atoi(zzlextext);
      zzgettok();
    } else if (setwd10[zztoken]&0x10) {
      v++;
    } else {
      zzFAIL(1,zzerr62,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
      goto fail;
    }

    zzasp=zztasp2;
  }

  TokenNum = v;
  if ( v>maxt ) maxt=v;
  if ( Tnum( t ) == 0 ) addForcedTname( t, v );
  else {
    warning("redefinition of token %s; ignored", fname, zzline, t);
  }
  {
    int zztasp2 = zzasp - 1;
    check_overflow(); --zzasp;
    {
    while (zztoken==154) {
      zzmatch(154);
      zzgettok();
      {
        int zztasp3 = zzasp - 1;
        check_overflow(); --zzasp;
        {
        if ( (zztoken==ID)&&(isDLGmaxToken(zzlextext)) ) {
          if (!(isDLGmaxToken(zzlextext))) {
            zzfailed_pred("  isDLGmaxToken(zzlextext)",0 /* report */, { 0; /* no user action */ } );
          }
          zzmatch(ID);
	      zzgettok();
          {
            int zztasp4 = zzasp - 1;
            check_overflow(); --zzasp;

            if (zztoken==153) {
              zzmatch(153);
              zzgettok();
              zzmatch(INT);
              zzgettok();
            } else if (!setwd10[zztoken]&0x20) {
              zzFAIL(1,zzerr63,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
              goto fail;
            }
            
            zzasp=zztasp4;
          }
        } else if (zztoken==ID) {
          zzmatch(ID);
          t = strdup(zzlextext);
          zzgettok();

          {
            int zztasp4 = zzasp - 1;
            check_overflow(); --zzasp;

            if (zztoken==153) {
              zzmatch(153);
              zzgettok();
              zzmatch(INT);
              v=atoi(zzlextext);
              zzgettok();
            } else if (setwd10[zztoken]&0x40) {
              v++;
            } else {
              zzFAIL(1,zzerr64,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
              goto fail;
            }

            zzasp=zztasp4;
          }

          TokenNum = v;
          if ( v>maxt ) maxt=v;
          if ( Tnum( t ) == 0 ) addForcedTname( t, v );
          else {
            warning("redefinition of token %s; ignored", fname, zzline, t);
          }
        } else if (!setwd10[zztoken]&0x80) {
          zzFAIL(1,zzerr65,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk);
          goto fail;
        }
        
        zzasp=zztasp3;
        }
      }
      zzasp=zztasp2;
    }
    zzasp=zztasp2;
    }
  }
  zzmatch(155);
  zzgettok();
  zzmatch(156);
  TokenNum = maxt + 1;
  zzgettok();

  zzasp=zztasp1;
  return;
fail:
  zzasp=zztasp1;
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd11, 0x1);
  }
}


/* MR2 Andreas Magnusson (Andreas.Magnusson@mailbox.swipnet.se) */
/* MR2 Fix to bug introduced by 1.33MR1 for #tokdefs            */
/* MR2 Don't let #tokdefs be confused by      */
/* MR2   DLGminToken and DLGmaxToken          */

/* semantic check on DLGminToken and DLGmaxmaxToken in #tokdefs */

static int isDLGmaxToken(char *Token)
{
    static char checkStr1[] = "DLGmaxToken";
    static char checkStr2[] = "DLGminToken";

    if (strcmp(Token, checkStr1) == 0)
        return 1;
    else if (strcmp(Token, checkStr2) == 0)
        return 1;
    else
        return 0;
}

/* semantics of #token */
static void chkToken(char *t, char *e, char *a, int tnum)
{
    TermEntry *p;

    /* check to see that they don't try to redefine a token as a token class */
    if ( t!=NULL ) {
        p = (TermEntry *) hash_get(Tname, t);
        if ( p!=NULL && p->classname ) {
            err(eMsg("redefinition of #tokclass '%s' to #token not allowed; ignored",t));
            if ( a!=NULL ) free((char *)a);
            return;
        }
    }

    if ( t==NULL && e==NULL ) {     /* none found */
        err("#token requires at least token name or rexpr");
    }
    else if ( t!=NULL && e!=NULL ) {  /* both found */
        if ( UserDefdTokens ) {     /* if #tokdefs, must not define new */
            p = (TermEntry *) hash_get(Tname, t);
            if ( p == NULL) {
                err(eMsg("new token definition '%s' not allowed - only #token with name already defined by #tokdefs file allowed",t));
                return;
            };
        }
        Tklink(t, e);
        if ( a!=NULL ) {
            if ( hasAction(e) ) {
                err(eMsg("redefinition of action for %s; ignored",e));
            }
            else setHasAction(e, a);
        }
    }
    else if ( t!=NULL ) {       /* only one found */
        if ( UserDefdTokens ) {
            p = (TermEntry *) hash_get(Tname, t);
            if (p == NULL) {
                err(eMsg("new token definition '%s' not allowed - only #token with name already defined by #tokdefs file allowed",t));
            };
            return;
        }
        if ( Tnum( t ) == 0 ) addTname( t );
else {
  err(eMsg("redefinition of token %s; ignored",t));
}
if ( a!=NULL ) {
  err(eMsg("action cannot be attached to a token name (%s); ignored",t));
  free((char *)a);
}
}
else if ( e!=NULL ) {
if ( Tnum( e ) == 0 ) addTexpr( e );
else {
  if ( hasAction(e) ) {
    err(eMsg("redefinition of action for expr %s; ignored",e));
  }
  else if ( a==NULL ) {
    err(eMsg("redefinition of expr %s; ignored",e));
  }
}
if ( a!=NULL ) setHasAction(e, a);
}

  /* if a token type number was specified, then add the token ID and 'tnum'
* pair to the ForcedTokens list.  (only applies if an id was given)
*/
if ( t!=NULL && tnum>0 )
{
if ( set_el(tnum, reserved_positions) )
{
  err(eMsg("a token has already been forced to token number %d; ignored", tnum));
}
else
{
  list_add(&ForcedTokens, newForcedToken(t,tnum));
  set_orel(tnum, &reserved_positions);
}
}
}

static int match_token(char *s, char **nxt)
{
  if ( !(*s>='A' && *s<='Z') ) return 0;
  s++;
  while ( (*s>='a' && *s<='z') ||
  (*s>='A' && *s<='Z') ||
  (*s>='0' && *s<='9') ||
  *s=='_' )
  {
    s++;
  }
  if ( *s!=' ' && *s!='}' ) return 0;
  *nxt = s;
  return 1;
}

static int match_rexpr(char *s, char **nxt)
{
  if ( *s!='"' ) return 0;
  s++;
  while ( *s!='"' )
  {
    if ( *s=='\n' || *s=='\r' )
      warn("eoln found in regular expression");
    if ( *s=='\\' ) s++;
    s++;
  }
  *nxt = s+1;
  return 1;
}

/**
 * Walk a string "{ A .. Z }" where A..Z is a space separated list
 * of token references (either labels or reg exprs).  Return a
 * string "inlineX_set" for some unique integer X.  Basically,
 * we pretend as if we had seen "#tokclass inlineX { A .. Z }"
 * on the input stream outside of an action.
 */
char *inline_set(char *s)
{
  char *nxt;
  fprintf(stderr, "found consumeUntil( {...} )\n");
  while ( *s==' ' || *s=='\t' || *s=='\n' || *s=='\r' ) {s++;}
  if ( *s!='{' )
  {
    err("malformed consumeUntil( {...} ); missing '{'");
    return "bad_set";
  }
  s++;
  while ( *s==' ' || *s=='\t' || *s=='\n' || *s=='\r' ) {s++;}
  while ( *s!='}' )
  {
    if ( match_token(s,&nxt) ) fprintf(stderr, "found token %s\n", s);
    else if ( match_rexpr(s,&nxt) ) fprintf(stderr, "found rexpr %s\n", s);
    else {
      err("invalid element in consumeUntil( {...} )");
      return "bad_set";
    }
    s = nxt;
    while ( *s==' ' || *s=='\t' || *s=='\n' || *s=='\r' ) {s++;}
  }
  return "inlineX_set";
}


static int zzset_deg(SetWordType *a)
{
  /* Fast compute degree of a set... the number
     of elements present in the set.  Assumes
     that all word bits are used in the set
  */
  register SetWordType *p = a;
  register SetWordType *endp = &(a[zzSET_SIZE]);
  register int degree = 0;

  if ( a == NULL ) return 0;
  while ( p < endp )
  {
    register SetWordType t = *p;
    register SetWordType *b = &(bitmask[0]);
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


/**
 * ANTLR/DLG-specific syntax error message generator
 */
static void zzsyn(char *text, int tok, char *egroup, SetWordType *eset, int etok,
int k, char *bad_text)
{
    hdrLog(FileStr[CurFile]!=NULL?FileStr[CurFile]:"stdin", zzline);

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
