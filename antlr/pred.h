/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file pred.h
 * \brief source for predicate detection, manipulation
 */


extern char *PRED_AND_LIST;
extern char *PRED_OR_LIST;

/**
 * In C mode, return the largest constant integer found as the
 * sole argument to LATEXT(i).
 *
 * In C++ mode, return the largest constant integer found as the
 * sole argument to LT(i) given that the char before is nonalpha.
 */
int predicateLookaheadDepth(ActionNode *a);


/**
 * Look for a predicate;
 *
 * Do not pass anything but Junction nodes; no Actions, Tokens, RuleRefs.
 * This means that a "hoisting distance" of zero is the only distance
 * allowable.  Init actions are ignored.
 *
 * WARNING:
 *    Assumes no (..)? block after predicate for the moment.
 *    Does not check to see if pred is in production that can generate
 *      a sequence contained in the set of ambiguous tuples.
 *
 * Return the predicate found if any.
 */
Predicate *find_predicates(Node *alt);


Predicate *MR_find_predicates_and_supp(Node *alt);


Predicate *new_pred();


/** Walk a list of predicates and return the set of all tokens in scontext[1]'s */
set covered_set(Predicate *p);

/* MR10 predicate_free()
   MR10 Don't free the leaf nodes since they are part of the action node
*/
void predicate_free(Predicate *p);

Predicate * predicate_dup_without_context(Predicate *p);

Predicate * predicate_dup(Predicate *p);
