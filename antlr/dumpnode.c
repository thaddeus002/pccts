#include <stdio.h>
#include <ctype.h>

#include "set.h"
#include "syn.h"
#include "hash.h"
#include "generic.h"

void dumpset1(set s)
{
  if (set_nil(s)) {
    fprintf(stderr,"{}");
  } else {
    s_fprT(stderr,s);
  };
}

void dumpset(set s)
{
  dumpset1(s);
  fprintf(stderr,"\n");
}

int isEndRule(Node * p)
{
  int       result=0;
  if ( p->ntype == nJunction &&
       ( (Junction *) p)->jtype == EndRule) {
    result=1;
  };
  return result;
}

void dumppred1(int depth,Predicate *p)
{
  int       i;
  int       k;

  for (i=0; i<depth ; i++) {
    fprintf(stderr,"  ");
  };
  if (p->expr == PRED_AND_LIST ||
      p->expr == PRED_OR_LIST) {
    fprintf(stderr," %s", (p->expr == NULL ? "null expr" : p->expr));
    if (p->inverted) fprintf(stderr," predicate inverted !");
    if (p->redundant) {
      fprintf(stderr," Redundant!");
    };
    if (p->isConst) fprintf(stderr," const %d !",p->constValue);
    fprintf(stderr,"\n");
  } else {
    fprintf(stderr,"predicate k=%d",p->k);
    k=set_int(p->completionSet);
    if (k >= 0) {
      fprintf(stderr," Incomplete Set=%d !",k);
    };
    k=set_int(p->completionTree);
    if (k >= 0) {
      fprintf(stderr," Incomplete Tree=%d !",k);
    };
    if (p->redundant) {
      fprintf(stderr," Redundant!");
    };
    fprintf(stderr," \"%s\" (%x)", (p->expr == NULL ? "null expr" : p->expr) ,p);
    if (p->source != NULL) {
       fprintf(stderr,"line %d",p->source->line);
    };
    if (p->inverted) fprintf(stderr," predicate inverted !");
    fprintf(stderr,"\n");
    for (i=0; i<depth ; i++) {
      fprintf(stderr,"  ");
    };
    fprintf(stderr,"scontext: ");
    dumpset(p->scontext[1]);
    for (i=0; i<depth ; i++) {
      fprintf(stderr,"  ");
    };
    fprintf(stderr,"tcontext: ");
    preorder(p->tcontext);
    fprintf(stderr,"\n");
  };
  fprintf(stderr,"\n");
  if (p->down != NULL) {
    dumppred1(depth+1,p->down);
  };
  if (p->right != NULL) {
    dumppred1(depth,p->right);
  };
}

void dumppred(Predicate *p)
{
  fprintf(stderr,"---------------------------------\n");
  dumppred1(0,p);
  fprintf(stderr,"\n");
}

void dumppredtree(Predicate *p)
{
  fprintf(stderr,"predicate k=%d \"%s\" line %d\n",p->k,p->expr,p->source->line);
  dumpset(p->scontext[1]);
}

void dumppredexpr(Predicate *p)
{
  fprintf(stderr,"    pred expr \"%s\"\n",p->expr);
}

void dt(Tree *t)
{
  MR_dumpTreeF(stderr,0,t,5);
}

void d(Node * p)
{
  Junction      *j;
  RuleRefNode   *r;
  TokNode       *t;
  ActionNode    *a;

  if (p==NULL) {
    fprintf(stderr,"dumpNode: Node is NULL");
    return;
  };

  switch (p->ntype) {
    case nJunction :
      j = (Junction *) p;
      fprintf(stderr, "Junction (#%d in rule %s line %d) ",j->seq,j->rname,j->line);
      if (j->guess) fprintf(stderr,"guess block ");
      switch (j->jtype ) {
        case aSubBlk :
          fprintf(stderr,"aSubBlk");
          break;
        case aOptBlk :
          fprintf(stderr,"aOptBlk");
          break;
        case aLoopBegin :
          fprintf(stderr,"aLoopBeginBlk");
          break;
        case aLoopBlk :
          fprintf(stderr,"aLoopBlk");
          break;
        case aPlusBlk :
          fprintf(stderr,"aPlusBlk");
          break;
        case EndBlk :
          fprintf(stderr,"EndBlk");
          break;
        case RuleBlk :
          fprintf(stderr,"RuleBlk");
          break;
        case Generic :
          fprintf(stderr,"Generic");
          break;
        case EndRule :
          fprintf(stderr,"EndRule");
          break;
      };
      if (j->halt) fprintf(stderr,"  halt!");
      if (j->p1) fprintf(stderr," p1 valid");
      if (j->p2) {
        if (j->p2->ntype == nJunction) {
           fprintf(stderr," (p2=#%d)",( (Junction *) j->p2)->seq);
        } else {
           fprintf(stderr," (p2 valid)");
        };
      };
    if (j->ignore) fprintf(stderr, " ignore/plus-block-bypass");
      if (j->fset != NULL && set_deg(*j->fset) != 0) {
         fprintf(stderr,"\nfset:\n");
         dumpset(*j->fset);
      };
      if (j->ftree != NULL) {
         fprintf(stderr,"\nftree:\n");
         preorder(j->ftree);
      };
      fprintf(stderr,"\n");
      break;
    case nRuleRef :
       r = (RuleRefNode *) p;
       fprintf(stderr, "RuleRefNode (in rule %s line %d) to rule %s\n", r->rname,r->line,r->text);
       break;
    case nToken :
       t = (TokNode *) p;
       fprintf(stderr, "TokNode (in rule %s line %d) token %s\n",t->rname,t->line,TerminalString(t->token));
       break;
    case nAction :
       a =(ActionNode *) p;
       if (a->is_predicate) {
         fprintf(stderr, "Predicate (in rule %s line %d) %s",a->rname,a->line,a->action);
         if (a->inverted) fprintf(stderr," action inverted !");
         if (a->guardpred != NULL) {
           fprintf(stderr," guarded");
           dumppredexpr(a->guardpred);
           if (a->ampersandPred) {
             fprintf(stderr," \"&&\" style");
           } else {
             fprintf(stderr," \"=>\" style");
           };
         };
         if (a->predEntry != NULL) fprintf(stderr," predEntry \"%s\" ",a->predEntry->str);
         fprintf(stderr,"\n");
       } else if (a->init_action) {
         fprintf(stderr, "Init-Action (in rule %s line %d) %s\n",a->rname,a->line,a->action);
       } else {
         fprintf(stderr, "Action (in rule %s line %d) %s\n",a->rname,a->line,a->action);
       };
       break;
   };
}

Node * dp1(Node * p)
{
  Node  *result=NULL;

  if (p->ntype == nJunction) {
    result=( (Junction *) p )->p1;
    d(result);
  } else {
    fprintf(stderr,"dp1: Not a Junction node");
  };
  return result;
}

Node * dp2(Node * p)
{
  Node  *result=NULL;

  if (p->ntype == nJunction) {
    result=( (Junction *) p )->p2;
    d(result);
  } else {
    fprintf(stderr,"dp2: Not a Junction node");
  };
  return result;
}

Node * dn(Node * p)
{
  Node  *result=NULL;

  if (p->ntype == nRuleRef) {
    result=( (RuleRefNode *)p )->next;
  } else if (p->ntype == nAction) {
    result=( (ActionNode *)p )->next;
  } else if (p->ntype == nToken) {
    result=( (TokNode *)p )->next;
  } else {
    fprintf(stderr,"No next field: Neither a RuleRefNode, ActionNode, nor TokNode");
  };
  if (result != NULL) d(result);
  return result;
}

void df(Node * p)
{
  int       count=0;
  Node      *next;

  fprintf(stderr,"\n#%d ",++count);
  d(p);

  for (next=p; next != NULL && !isEndRule(next) ; ) {
    fprintf(stderr,"#%d ",++count);
    if (next->ntype == nJunction) {
      next=dp1(next);
    } else {
      next=dn(next);
    };
  };
}

Node *dfn(Node * p,int target)
{
  Node      *result=NULL;
  int       count=0;
  Node      *next;

  fprintf(stderr,"#%d ",++count);
  d(p);

  for (next=p; next != NULL && !isEndRule(next) ; ) {
    fprintf(stderr,"#%d ",++count);
    if (next->ntype == nJunction) {
      next=dp1(next);
    } else {
      next=dn(next);
    };
    if (count == target) {
      result=next;
      break;
    };
  };
  return result;
}


static int findnodeMatch;

Junction *findnode1(Node *n)
{
   Node         *next;
   Junction     *j;
   Junction     *match;

   if (n == NULL) return NULL;
   if (n->ntype == nJunction) {
     j=(Junction *) n;
     if (j->seq == findnodeMatch) return j;
     if (j->jtype == EndRule) return NULL;
     if (j->jtype != RuleBlk && j->jtype != EndBlk) {
       if (j->p2 != NULL && !j->ignore) {
          match=findnode1(j->p2);
          if (match != NULL) return match;
       };
     };
   };
   next=MR_advance(n);
   return findnode1(next);
}

Junction *findnode(int match)
{
  Junction  *j;
  Junction  *result=NULL;

  findnodeMatch=match;

  for (j=SynDiag; j != NULL; j=(Junction *)j->p2) {
    require (j->ntype == nJunction && j->jtype == RuleBlk,"Not a rule block");
    result=findnode1( (Node *) j);
    if (result != NULL) break;
  };
  if (result != NULL) {
    d( (Node *) result);
  };
  return result;
}
