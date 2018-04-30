/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file fset.h
 * \brief Compute FIRST and FOLLOW sets.
 */

#ifndef __FSET_H__
#define __FSET_H__

/**
 * What tokens are k tokens away from junction q?
 *
 * Follow both p1 and p2 paths (unless RuleBlk) to collect the tokens k away from this
 * node.
 * We lock the junction according to k--the lookahead.  If we have been at this
 * junction before looking for the same, k, number of lookahead tokens, we will
 * do it again and again...until we blow up the stack.  Locks are only used on aLoopBlk,
 * RuleBlk, aPlusBlk and EndRule junctions to remove/detect infinite recursion from
 * FIRST and FOLLOW calcs.
 *
 * If p->jtype == EndRule we are going to attempt a FOLLOW.  (FOLLOWs are really defined
 * in terms of FIRST's, however).  To proceed with the FOLLOW, p->halt cannot be
 * set.  p->halt is set to indicate that a reference to the current rule is in progress
 * and the FOLLOW is not desirable.
 *
 * If we attempt a FOLLOW and find that there is no FOLLOW or REACHing beyond the EndRule
 * junction yields an empty set, replace the empty set with EOF.  No FOLLOW means that
 * only EOF can follow the current rule.  This normally occurs only on the start symbol
 * since all other rules are referenced by another rule somewhere.
 *
 * Normally, both p1 and p2 are followed.  However, checking p2 on a RuleBlk node is
 * the same as checking the next rule which is clearly incorrect.
 *
 * Cycles in the FOLLOW sense are possible.  e.g. Fo(c) requires Fo(b) which requires
 * Fo(c).  Both Fo(b) and Fo(c) are defined to be Fo(b) union Fo(c).  Let's say
 * Fo(c) is attempted first.  It finds all of the FOLLOW symbols and then attempts
 * to do Fo(b) which finds of its FOLLOW symbols.  So, we have:
 *
 *                  Fo(c)
 *                 /     \
 *              a set    Fo(b)
 *                      /     \
 *                   a set    Fo(c) .....Hmmmm..... Infinite recursion!
 *
 * The 2nd Fo(c) is not attempted and Fo(b) is left deficient, but Fo(c) is now
 * correctly Fo(c) union Fo(b).  We wish to pick up where we left off, so the fact
 * that Fo(b) terminated early means that we lack Fo(c) in the Fo(b) set already
 * laying around.  SOOOOoooo, we track FOLLOW cycles.  All FOLLOW computations are
 * cached in a hash table.  After the sequence of FOLLOWs finish, we reconcile all
 * cycles --> correct all Fo(rule) sets in the cache.
 *
 * Confused? Good! Read my MS thesis [Purdue Technical Report TR90-30].
 * TJP 8/93 -- can now read PhD thesis from Purdue.
 *
 * Also, FIRST sets are cached in the hash table.  Keys are (rulename,Fi/Fo,k).
 * Only FIRST sets, for which the FOLLOW is not included, are stored.
 *
 * SPECIAL CASE of (...)+ blocks:
 * I added an optional alt so that the alts could see what
 * was behind the (...)+ block--thus using enough lookahead
 * to branch out rather than just enough to distinguish
 * between alts in the (...)+.  However, when the FIRST("(...)+") is
 * is needed, must not use this last "optional" alt.  This routine
 * turns off this path by setting a new 'ignore' flag for
 * the alt and then resetting it afterwards.
 */
set rJunc(Junction *p, int k, set *rk);

set rRuleRef( RuleRefNode *p, int k, set *rk_out );

/**
 * Return FIRST sub k ( token_node )
 *
 * TJP 10/11/93 modified this so that token nodes that are actually
 * ranges (T1..T2) work.
 */
set rToken( TokNode *p, int k, set *rk );

set rAction(ActionNode *p, int k, set *rk);


/**
 * Don't analyze alpha block of (alpha)?beta; if (alpha)? then analyze
 * Return the 1st node of the beta block if present else return j.
 */
Junction *analysis_point(Junction *j);

set First(Junction *j, int k, int jtype, int *max_k);


#endif
