/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file misc.h
 *
 * Manage tokens, regular expressions.
 * Print methods for debugging
 * Compute follow lists onto tail ends of rules.
 */


#ifndef __MISC_H__
#define __MISC_H__

#include <stdio.h>


/**
 * switch to lexical class/mode m.  This amounts to creating a new
 * lex mode if one does not already exist and making ExprStr point
 * to the correct char string array.  We must also switch Texpr tables.
 *
 * BTW, we need multiple ExprStr arrays because more than one automaton
 * may have the same label for a token, but with different expressions.
 * We need to track an expr for each automaton.  If we disallowed this
 * feature, only one ExprStr would be required.
 */
void lexclass(char *m);

void lexmode(int i);

int hasAction( char *expr );

void setHasAction( char *expr, char *action );

ForcedToken *newForcedToken(char *token, int tnum);

/**
 * Make a token indirection array that remaps token numbers and then walk
 * the appropriate symbol tables and SynDiag to change token numbers
 */
void RemapForcedTokens();


/**
 * Add a token name.  Return the token number associated with it.  If it already
 * exists, then return the token number assigned to it.
 *
 * Track the order in which tokens are found so that the DLG output maintains
 * that order.  It also lets us map token numbers to strings.
 */
int addTname(char *token);

/**
 * This is the same as addTname except we force the TokenNum to be tnum.
 * We don't have to use the Forced token stuff as no tokens will have
 * been defined with #tokens when this is called.  This is only called
 * when a #tokdefs meta-op is used.
 */
int addForcedTname(char *token, int tnum);

/**
 * Add a token expr.  Return the token number associated with it.  If it already
 * exists, then return the token number assigned to it.
 */
int addTexpr( char *expr );

/** return the token number of 'term'.  Return 0 if no 'term' exists */
int Tnum( char *term );

/**
 * associate a Name with an expr.  If both have been already assigned
 * token numbers, then an error is reported.  Add the token or expr
 * that has not been added if no error.  This 'represents' the #token
 * ANTLR pseudo-op.  If both have not been defined, define them both
 * linked to same token number.
 */
void Tklink( char *token, char *expr );


/**
 * add an element to a list.
 *
 * Any non-empty list has a sentinel node whose 'elem' pointer is really
 * a pointer to the last element.  (i.e. length(list) = #elemIn(list)+1).
 * Elements are appended to the list.
 */
void list_add(ListNode **list, void *e);

/* MR10 list_free() frees the ListNode elements in the list       */
/* MR10   if freeData then free the data elements of the list too */
void list_free(ListNode **list,int freeData);

void list_apply( ListNode *list, void (*f)(void *) );


int list_search_cstring(ListNode *list, char * cstring);

      /* F O L L O W  C y c l e  S t u f f */

/* make a key based upon (rulename, computation, k value).
 * Computation values are 'i'==FIRST, 'o'==FOLLOW.
 */

/* MR10  Make the key all characters so it can be read easily   */
/* MR10    by a simple dump program.  Also, separates           */
/* MR10   'o' and 'i' from rule name                            */

char *Fkey(char *rule, int computation, int k);

/** Push a rule onto the kth FOLLOW stack */
void FoPush(char *rule, int k);

/** Pop one rule off of the FOLLOW stack.  TOS ptr is NULL if empty. */
void FoPop(int k);

/**
 * Compute FOLLOW cycle.
 * Mark all FOLLOW sets for rules in cycle as incomplete.
 * Then, save cycle on the cycle list (Cycles) for later resolution.
 * The Cycle is stored in the form:
 *    (head of cycle==croot, rest of rules in cycle==cyclicDep)
 *
 * e.g. (Fo means "FOLLOW of", "-->" means requires or depends on)
 *
 *    Fo(x)-->Fo(a)-->Fo(b)-->Fo(c)-->Fo(x)
 *                       ^----Infinite recursion (cycle)
 *
 * the cycle would be: x -> {a,b,c} or stored as (x,{a,b,c}).  Fo(x) depends
 * on the FOLLOW of a,b, and c.  The root of a cycle is always complete after
 * Fo(x) finishes.  Fo(a,b,c) however are not.  It turns out that all rules
 * in a FOLLOW cycle have the same FOLLOW set.
 */
void RegisterCycle(char *rule, int k);

/**
 * make all rules in cycle complete
 *
 * while ( some set has changed ) do
 *    for each cycle do
 *      if degree of FOLLOW set for croot > old degree then
 *        update all FOLLOW sets for rules in cyclic dependency
 *        change = TRUE
 *      endif
 *    endfor
 * endwhile
 */
void ResolveFoCycles(int k);


      /* P r i n t i n g  S y n t a x  D i a g r a m s */

/** How to print out a junction */
void pJunc( Junction *q );

/** How to print out a rule reference node */
void pRuleRef( RuleRefNode *p );

/** How to print out a terminal node */
void pToken( TokNode *p );

/** How to print out a terminal node */
void pAction(ActionNode *p);


          /* F i l l  F o l l o w  L i s t s */

/**
 * Search all rules for all rule reference nodes, q to rule, r.
 * Add q->next to follow list dangling off of rule r.
 * i.e.
 *
 *    r: -o-R-o-->o--> Ptr to node following rule r in another rule
 *          |
 *          o--> Ptr to node following another reference to r.
 *
 * This is the data structure employed to avoid FOLLOW set computation.  We
 * simply compute the FIRST (reach) of the EndRule Node which follows the
 * list found at the end of all rules which are referenced elsewhere.  Rules
 * not invoked by other rules have no follow list (r->end->p1==NULL).
 * Generally, only start symbols are not invoked by another rule.
 *
 * Note that this mechanism also gives a free cross-reference mechanism.
 *
 * The entire syntax diagram is layed out like this:
 *
 * SynDiag
 *  |
 *  v
 *  o-->R1--o
 *  |
 *  o-->R2--o
 *  |
 *  ...
 *  |
 *  o-->Rn--o
 *
 */
void FoLink(Node *p);


void GenCrossRef(Junction *p);


/** skip to just past the "=" separating the declaration from the initialization value */
char *getInitializer(char *pStart);


/**
 *  This replaces HasComma() which couldn't distinguish
 *       foo ["a,b"]
 *  from:
 *       foo[a,b]
 */
int hasMultipleOperands(char *pStart);


/**
 * strBetween(pStart, pNext, pStop)
 *
 * Creates a null terminated string by copying the text between two pointers
 * to a work area.  The start of the string is pStart.  The end of the string
 * is the character before pNext, or if pNext is null then the character before
 * pStop.  Trailing spaces are not included in the copy operation.
 *
 * This is used when a string contains several parts.  The pNext part may be
 * optional.  The pStop will stop the scan when the optional part is not present
 * (is a null pointer).
 */
char *strBetween(char *pStart, char *pNext, char *pStop);


/**
   function     Returns pointer to character following separator at
   value        which to continue search for next formal.  If at the
                end of the string a pointer to the null byte at the
                end of the string is returned.

   pStart       Pointer to the starting position of the formal list

                This may be the middle of a longer string, for example
                when looking for the end of formal #3 starting from
                the middle of the complete formal list.

   ppDataType   Returns a pointer to the start of the data type in the
                formal. Example: pointer to "Foo".

   ppSymbol     Returns a pointer to the start of the formal symbol.
                Example: pointer to "f".

   ppEqualSign  Returns a pointer to the equal sign separating the
                formal symbol from the initial value.  If there is
                no "=" then this will be NULL.

   ppValue      Returns a pointer to the initial value part of the
                formal declaration.  Example: pointer to "&farray[1,2]"

   ppSeparator  Returns a pointer to the character which terminated the
                scan.  This should be a pointer to a comma or a null
                byte which terminates the string.

   pNest        Returns the nesting level when a separator was found.
                This is non-zero for any kind of error.  This is zero
                for a successful parse of this portion of the formal
                list.
*/
char *endFormal(char *pStart,
                 char **ppDataType,
                 char **ppSymbol,
                 char **ppEqualSign,
                 char **ppValue,
                 char **ppSeparator,
                 int *pNest);


void s_fprT(FILE *f, set e);


/** Return the token name or regular expression for a token number. */
char *TerminalString(int token);


#endif
