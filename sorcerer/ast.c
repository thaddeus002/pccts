/* Abstract syntax tree manipulation functions
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-2000
 */

#include <stdarg.h>

/* ensure that tree manipulation variables are current after a rule
 * reference
 */

void zzlink(AST **_root, AST **_sibling, AST **_tail)
{
  if ( *_sibling == NULL ) return;
  if ( *_root == NULL ) *_root = *_sibling;
  else if ( *_root != *_sibling ) (*_root)->down = *_sibling;
  if ( *_tail==NULL ) *_tail = *_sibling;
  while ( (*_tail)->right != NULL ) *_tail = (*_tail)->right;
}

AST *zzastnew()
{
  AST *p = (AST *) calloc(1, sizeof(AST));
  if ( p == NULL ) fprintf(stderr,"%s(%d): cannot allocate AST node\n",__FILE__,__LINE__);
  return p;
}

/** Add a child node to the current sibling list */
void zzsubchild(AST **_root, AST **_sibling, AST **_tail)
{
  AST *n;
  zzNON_GUESS_MODE {
  n = zzastnew();
  zzcr_ast(n, &(zzaCur), LA(1), LATEXT(1));
  zzastPush( n );
  if ( *_tail != NULL ) (*_tail)->right = n;
  else {
    *_sibling = n;
    if ( *_root != NULL ) (*_root)->down = *_sibling;
  }
  *_tail = n;
  if ( *_root == NULL ) *_root = *_sibling;
  }
}

/**
 * Make a new AST node. Make the newly-created
 * node the root for the current sibling list. If a root node already
 * exists, make the newly-created node the root of the current root.
 */
void zzsubroot(AST **_root, AST **_sibling, AST **_tail)
{
  AST *n;
  zzNON_GUESS_MODE {
  n = zzastnew();
  zzcr_ast(n, &(zzaCur), LA(1), LATEXT(1));
  zzastPush( n );
  if ( *_root != NULL )
    if ( (*_root)->down == *_sibling ) *_sibling = *_tail = *_root;
  *_root = n;
  (*_root)->down = *_sibling;
  }
}

/* Apply function to root then each sibling
 * example: print tree in child-sibling LISP-format (AST has token field)
 *
 *  void show(tree)
 *  AST *tree;
 *  {
 *    if ( tree == NULL ) return;
 *    printf(" %s", zztokens[tree->token]);
 *  }
 *
 *  void before() { printf(" ("); }
 *  void after()  { printf(" )"); }
 *
 *  LISPdump() { zzpre_ast(tree, show, before, after); }
 *
 */
void zzpre_ast(
    AST *tree,
    void (*func)(AST *),   /* apply this to each tree node */
    void (*before)(AST *), /* apply this to root of subtree before preordering it */
    void (*after)(AST *))  /* apply this to root of subtree after preordering it */
{
  while ( tree!= NULL )
  {
    if ( tree->down != NULL ) (*before)(tree);
    (*func)(tree);
    zzpre_ast(tree->down, func, before, after);
    if ( tree->down != NULL ) (*after)(tree);
    tree = tree->right;
  }
}


/*
   MR19 Optimize freeing of the following structure to limit recursion
   SAKAI Kiyotaka (ksakai@isr.co.jp)
*/

/*
         NULL o
             / \
           NULL o
               / \
            NULL NULL
*/

/*
   MR21 Another refinement to replace recursion with iteration
   NAKAJIMA Mutsuki (muc@isr.co.jp).
*/

void zzfree_ast(AST *tree)
{

    AST *otree;

    if (tree == NULL) return;

    while (tree->down == NULL || tree->right == NULL) {

        if (tree->down == NULL && tree->right == NULL) {
            zztfree(tree);
            return;
        }

        otree = tree;
        if (tree->down == NULL) {
            tree = tree->right;
        } else {
            tree = tree->down;
        }
        zztfree( otree );
    }

    while (tree != NULL) {
        zzfree_ast(tree->down);
        otree = tree;
        tree = otree->right;
        zztfree(otree);
    }
}

/* build a tree (root child1 child2 ... NULL)
 * If root is NULL, simply make the children siblings and return ptr
 * to 1st sibling (child1).  If root is not single node, return NULL.
 *
 * Siblings that are actually siblins lists themselves are handled
 * correctly.  For example #( NULL, #( NULL, A, B, C), D) results
 * in the tree ( NULL A B C D ).
 *
 * Requires at least two parameters with the last one being NULL.  If
 * both are NULL, return NULL.
 */
AST *zztmake(AST *rt, ...)
{
  va_list ap;
  register AST *child, *sibling=NULL, *tail=NULL /* MR20 */, *w;
  AST *root;

  va_start(ap, rt);
  root = rt;

  if ( root != NULL )
    if ( root->down != NULL ) return NULL;
  child = va_arg(ap, AST *);
  while ( child != NULL )
  {
    for (w=child; w->right!=NULL; w=w->right) {;} /* find end of child */
    if ( sibling == NULL ) {sibling = child; tail = w;}
    else {tail->right = child; tail = w;}
    child = va_arg(ap, AST *);
  }
  if ( root==NULL ) root = sibling;
  else root->down = sibling;
  va_end(ap);
  return root;
}

/* tree duplicate */
AST *zzdup_ast(AST *t)
{
  AST *u;

  if ( t == NULL ) return NULL;
  u = zzastnew();
  *u = *t;
#ifdef zzAST_DOUBLE
  u->up = NULL;   /* set by calling invocation */
  u->left = NULL;
#endif
  u->right = zzdup_ast(t->right);
  u->down = zzdup_ast(t->down);
#ifdef zzAST_DOUBLE
  if ( u->right!=NULL ) u->right->left = u;
  if ( u->down!=NULL ) u->down->up = u;
#endif
  return u;
}

void zztfree(AST *t)
{
#ifdef zzd_ast
  zzd_ast( t );
#endif
  free( t );
}

#ifdef zzAST_DOUBLE
/**
 * Set the 'up', and 'left' pointers of all nodes in 't'.
 * Initial call is double_link(your_tree, NULL, NULL).
 */
void zzdouble_link(AST *t, AST *left, AST *up)
{
  if ( t==NULL ) return;
  t->left = left;
  t->up = up;
  zzdouble_link(t->down, NULL, t);
  zzdouble_link(t->right, t, up);
}
#endif
