/*
 * Thaddeus002's pccts
 *
 * Public Domain
 */

/**
 * \file mrhoist.h
 */

#ifndef __MR_HOIST_H__
#define __MR_HOIST_H__


/**
 * Try to determine whether predicate "first" is true for
 * all cases where "second" is true.  Comparison takes place
 * without regard to context.
 * Assumes that predicate symbols have been expanded.
 * Assumes that there are no NAND or NOR nodes
 */
int MR_secondPredicateUnreachable(Predicate *first,Predicate *second);

void MR_dumpTokenSet(FILE *f,int depth,set s);

void MR_dumpPred1(int depth,Predicate *p,int withContext);

void MR_dumpPred(Predicate *p,int withContext);

Tree *MR_make_tree_from_set(set s);

void MR_check_pred_too_long(Predicate *p,set completion);

int MR_predicate_context_completed(Predicate *p);

/* the predicates which are ANDed together have a common
   context:  they must all have common roots.  Thus the
   AND operation is more like an OR operation because
   branches which are longer are grafted onto shorter
   branches of the AND tree.  For instance combining
   (A B C) with (A B C D) gives (A B C D).  There
   should never be a case of (A B C) and (A B D) because
   they have the same context.

   Actually, this may not be true once one throws in
   guard predicates which are defined by the user, not
   the context.
*/

/** requires input trees to be in "canonical" format */
Tree *MR_computeTreeAND(Tree *l,Tree *r);

void MR_pred_depth(Predicate *p,int *maxDepth);

/** this computes the OR of all the contexts */
set MR_compute_pred_set(Predicate *p);

void MR_cleanup_pred_trees(Predicate *p);

int MR_tree_equ(Tree *big, Tree *small);

Predicate *MR_find_in_aSubBlk(Junction *alt);

void MR_predContextPresent(Predicate *p,int *allHaveContext,int *noneHaveContext);

int MR_pointerStackPush(PointerStack *ps,void *dataPointer);

void *MR_pointerStackPop(PointerStack *ps);

void MR_pointerStackReset(PointerStack *ps);

Junction *MR_nameToRuleBlk(char *name);

/** pretty much ignores things with the inverted bit set */
Predicate *MR_predFlatten(Predicate *p);

Predicate *MR_predSimplifyALLX(Predicate *p,int skipPass3);

Predicate *MR_predSimplifyALL(Predicate *p);

void MR_releaseResourcesUsedInRule(Node *n);

int MR_allPredLeaves(Predicate *p);

char *MR_ruleNamePlusOffset(Node *n);

int MR_max_height_of_tree(Tree *t);

int MR_all_leaves_same_height(Tree *t,int depth);

int MR_comparePredicates(Predicate *a, Predicate *b);

/**
 *  action->inverted can be set only when a predicate symbol appears in
 *      a rule:  "rule : <<!XXX>>? X".  It cannot be set under any
 *      other circumstances.  In particular it cannot be set by
 *      "#pred NotA !A" or by "#pred Nota <<!A>>?".  The first case
 *      creates a predEntry and the predicate expression of that predEntry
 *      has inverted set.  In the second case, the code for handling "!"
 *      is only present in buildAction, which is not called by the #pred
 *      semantic routines, only when a <<...>>? is recognized as part of
 *      a rule definition.
 *
 *  predicate->inverted can only be set by a predicate created by a #pred
 *      expression, such as "#pred NotA !A" or "#pred NotXY ! (X && Y) or
 *      "#pred XbarY !(X && Y)".  In particular, it cannot be set by any
 *      predicate expression occurring under any other circumstances.
 *      The #pred predicate expresssions are stored with in predEntry->pred
 *      and do not normally appear anywhere else until the predicates are
 *      "unfolded" in order to recognize redundancies, conflicts, and
 *      tautologies.
 *
 *  The unfold routine expands all references to #pred expressions.
 *
 *  The simplifyInvert goes through and propagates the invert bit so that
 *      all OR and AND nodes are un-inverted.
 *
 *  Note that !(A and B) => (!A or !B)
 *            !(A or B)  => (!A and !B)
 *
 *  MR_unfold() is called to expand predicate symbols by replacing predicates
 *    that reference predicate entries with the copies of the predicate entries.
 *    Each reference receives a duplicate of the original.  This is necessary
 *    because the next phase involves simplification and removal of redundant
 *    predicate nodes.  Anyway, the point I'm making is that predicate->invert
 *    should not be set in any predicate until it has been expanded.
 *
 *    This is a recursive structure, but there is no need for "recursive expansion"
 *    by which I mean a predicate symbol refers to other predicate symbols which
 *    must also be expanded.
 *
 *    Recursive expansion is *not* performed by this routine because it is not
 *    necessary.  Expansion of references is performed by predPrimary when
 *    a new predicate symbol is created by referring to others in the pred expr.
 */
Predicate *MR_unfold(Predicate *pred);

/**
 * this should be called immediately after MR_unfold() and
 * at no other times.
 * TODO make it static and call it in MR_unfold()
 */
void MR_simplifyInverted(Predicate *pred,int inverted);

/** only remove it from AND and OR nodes, not leaves */
void MR_clearPredEntry(Predicate *p);


void MR_orphanRules(FILE *f);


Tree *MR_compute_pred_tree_context(Predicate *p);

void MR_guardPred_plainSet(ActionNode *anode,Predicate *pred);

/*******************************************************************************/

extern int MR_SuppressSearch;


Predicate *MR_suppressK(Node *j,Predicate *p);

void MR_suppressSearchReport();

void MR_alphaBetaTraceReport();

void MR_dumpRuleSet(set s);

#endif
