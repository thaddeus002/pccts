/**
 * \file antlr_log.h
 */

#ifndef __ANTLR_LOGGER_H_
#define __ANTLR_LOGGER_H_

#include "syn.h"

          /* E r r o r  M a c r o s */

#define antlr_fatal(err)  fatalFL(err, __FILE__, __LINE__)
#define fatal_internal(err) fatal_intern(err, __FILE__, __LINE__)

        /* S a n i t y  C h e c k i n g */

#define require(expr, err) {if ( !(expr) ) fatal_internal(err);}




void warn(char *err);


void err(char *err);


void dlgerror(const char *s);

void fatalFL(char *err_, char *f, int l);

void fatal_intern(char *err_, char *f, int l);

/**
 * Close 'DefFile' if needed.
 */
void cleanUp(void);


        /* M e s s a g e  P a s s i n g  T o  N o d e s */

/* fpPrint[node type] == pointer to function that knows how to print that node. */
extern void (*fpPrint[NumNodeTypes+1])();
extern struct _set (*fpReach[])();
extern struct _tree *(*fpTraverse[])();


/*
 * assumes a 'Junction *r' exists.  This macro calls a function with
 * the pointer to the node to operate on and a pointer to the rule
 * in which it is enclosed.
 */
#define TRANS(p)  {if ( (p)==NULL ) antlr_fatal("TRANS: NULL object");    \
          if ( (p)->ntype == nJunction ) (*(fpJTrans[((Junction *)(p))->jtype]))( p );\
          else (*(fpTrans[(p)->ntype]))( p );}

#define PRINT(p)  {if ( (p)==NULL ) antlr_fatal("PRINT: NULL object");\
          (*(fpPrint[(p)->ntype]))( p );}

#define REACH(p,k,rk,a) {if ( (p)==NULL ) antlr_fatal("REACH: NULL object");\
          (a) = (*(fpReach[(p)->ntype]))( p, k, rk );}

#define TRAV(p,k,rk,a) {if ( (p)==NULL ) {\
            if ( ContextGuardTRAV ) (a)=NULL; \
            else antlr_fatal("TRAV: NULL object");\
            } \
          else (a) = (*(fpTraverse[(p)->ntype]))( p, k, rk );}




#endif
