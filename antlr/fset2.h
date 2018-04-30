/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file fset2.h
 * \brief Compute FIRST sets for full LL(k)
 */

#ifndef __FSET2_H__
#define __FSET2_H__

extern set *fset;
extern int ConstrainSearch;
extern int maxk; /* set to initial k upon tree construction request */


/**
 * Do root
 * Then each sibling
 */
void preorder(Tree *tree);

Tree *tnode(int tok);

/** tree duplicate */
Tree *tdup( Tree *t );


Tree *tappend( Tree *t, Tree *u );

/** dealloc all nodes in a tree */
void Tfree( Tree *t );

/**
 * Find all children (alts) of t that require remaining_k nodes to be LL_k
 * tokens long.
 *
 * t-->o
 *     |
 *     a1--a2--...--an    <-- LL(1) tokens
 *     |   |        |
 *     b1  b2  ...  bn    <-- LL(2) tokens
 *     |   |        |
 *     .   .        .
 *     .   .        .
 *     z1  z2  ...  zn    <-- LL(LL_k) tokens
 *
 * We look for all [Ep] needing remaining_k nodes and replace with u.
 * u is not destroyed or actually used by the tree (a copy is made).
 */
Tree *tlink( Tree *t, Tree *u, int remaining_k );

/** remove as many ALT nodes as possible while still maintaining semantics */
Tree *tshrink(Tree *t);

Tree *tflatten(Tree *t);

Tree *tJunc(Junction *p, int k, set *rk);

Tree *tRuleRef(RuleRefNode *p, int k, set *rk_out);

Tree *tToken(TokNode *p, int k, set *rk);

Tree *tAction(ActionNode *p, int k, set *rk);

/** combine (? (A t) ... (A u) ...) into (? (A t u)) */
Tree *tleft_factor(Tree *t);

/** remove the permutation p from t if present */
Tree *trm_perm( Tree *t, Tree *p );

/**
 * Compute LL(k) trees for alts alt1 and alt2 of p.
 * function result is tree of ambiguous input permutations
 *
 * ALGORITHM may change to look for something other than LL_k size
 * trees ==> maxk will have to change.
 */
Tree *VerifyAmbig(Junction *alt1, Junction *alt2, unsigned **ft, set *fs, Tree **t, Tree **u, int *numAmbig);

/**
 * Make a tree from k sets where the degree of the first k-1 sets is 1.
 */
Tree *make_tree_from_sets(set *fset1, set *fset2);

/**
 * Create and return the tree of lookahead k-sequences that are in t, but not
 * in the context of predicates in predicate list p.
 */
Tree *tdif(Tree *ambig_tuples, Predicate *p, set *fset1, set *fset2);

int is_single_tuple(Tree *t);

/**
 * Look at a (...)? generalized-predicate context-guard and compute
 * either a lookahead set (k==1) or a lookahead tree for k>1.  The
 * k level is determined by the guard itself rather than the LL_k
 * variable.  For example, ( A B )? is an LL(2) guard and ( ID )?
 * is an LL(1) guard.  For the moment, you can only have a single
 * tuple in the guard.  Physically, the block must look like this
 *   --o-->TOKEN-->o-->o-->TOKEN-->o-- ... -->o-->TOKEN-->o--
 * An error is printed for any other type.
 */
Predicate *computePredFromContextGuard(Graph blk,int *msgDone);

/* MR13
   When the context guard is originally computed the
   meta-tokens are not known.
*/
void recomputeContextGuard(Predicate *pred);


extern int MR_AmbSourceSearch;
extern int MR_matched_AmbAidRule;


void MR_traceAmbSourceK(Tree *t,Junction *alt1,Junction *alt2);


/* this if for k=1 grammars only

   this is approximate only because of the limitations of linear
   approximation lookahead.  Don't want to do a k=3 search when
   the user only specified a ck=3 grammar
*/
void MR_traceAmbSource(set *matchSets,Junction *alt1, Junction *alt2);

void MR_backTraceDumpItemReset();

void MR_backTraceDumpItem(FILE *f,int skip,Node *n);

void MR_backTraceReport();

void MR_setConstrainPointer(set * newConstrainValue);


#endif
