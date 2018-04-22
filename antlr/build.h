/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file build.h
 * \brief functions associated with building syntax diagrams.
 */


/**
 * Add the parameter string 'parm' to the parms field of a block-type junction
 * g.left points to the sentinel node on a block.  i.e. g.left->p1 points to
 * the actual junction with its jtype == some block-type.
 */
void addParm(Node *p, char *parm);


/**
 * Build an action node for the syntax diagram
 *
 * buildAction(ACTION) ::= --o-->ACTION-->o--
 *
 * Where o is a junction node.
 */
Graph buildAction(char *action, int file, int line, int is_predicate);


/**
 * Build a token node for the syntax diagram
 *
 * buildToken(TOKEN) ::= --o-->TOKEN-->o--
 *
 * Where o is a junction node.
 */
Graph buildToken(char *text);


/**
 * Build a wild-card node for the syntax diagram
 *
 * buildToken(TOKEN) ::= --o-->'.'-->o--
 *
 * Where o is a junction node.
 */
Graph buildWildCard(char *text);


void setUpperRange(TokNode *t, char *text);


/**
 * Build a rule reference node of the syntax diagram
 *
 * buildRuleRef(RULE) ::= --o-->RULE-->o--
 *
 * Where o is a junction node.
 *
 * If rule 'text' has been defined already, don't alloc new space to store string.
 * Set r->text to point to old copy in string table.
 */
Graph buildRuleRef(char *text);


/**
 * Or two subgraphs into one graph via:
 *
 * Or(G1, G2) ::= --o-G1-o--
 *                  |    ^
 *          v    |
 *                  o-G2-o
 *
 * Set the altnum of junction starting G2 to 1 + altnum of junction starting G1.
 * If, however, the G1 altnum is 0, make it 1 and then
 * make G2 altnum = G1 altnum + 1.
 */
Graph Or(Graph g1, Graph g2);


/**
 * Catenate two subgraphs
 *
 * Cat(G1, G2) ::= --o-G1-o-->o-G2-o--
 * Cat(NULL,G2)::= --o-G2-o--
 * Cat(G1,NULL)::= --o-G1-o--
 */
Graph Cat(Graph g1, Graph g2);


/**
 * Make a subgraph an optional block
 *
 * makeOpt(G) ::= --o-->o-G-o-->o--
 *                      |       ^
 *            v       |
 *              o-------o
 *
 * Note that this constructs {A|B|...|Z} as if (A|B|...|Z|) was found.
 *
 * The node on the far right is added so that every block owns its own
 * EndBlk node.
 */
Graph makeOpt(Graph g1, int approx, char * pFirstSetSymbol);


/**
 * Make a graph into subblock
 *
 * makeBlk(G) ::= --o-->o-G-o-->o--
 *
 * The node on the far right is added so that every block owns its own
 * EndBlk node.
 */
Graph makeBlk(Graph g1, int approx, char * pFirstSetSymbol);


/**
 * Make a subgraph into a loop (closure) block -- (...)*
 *
 * makeLoop(G) ::=       |---|
 *               v   |
 *         --o-->o-->o-G-o-->o--
 *                   |           ^
 *                   v           |
 *           o-----------o
 *
 * After making loop, always place generic node out front.  It becomes
 * the start of enclosing block.  The aLoopBlk is the target of the loop.
 *
 * Loop blks have TWO EndBlk nodes--the far right and the node that loops back
 * to the aLoopBlk node.  Node with which we can branch past loop == aLoopBegin and
 * one which is loop target == aLoopBlk.
 * The branch-past (initial) aLoopBegin node has end
 * pointing to the last EndBlk node.  The loop-target node has end==NULL.
 *
 * Loop blocks have a set of locks (from 1..CLL_k) on the aLoopBlk node.
 */
Graph makeLoop(Graph g1, int approx, char * pFirstSetSymbol);


/**
 * Make a subgraph into a plus block -- (...)+ -- 1 or more times
 *
 * makePlus(G) ::=   |---|
 *           v   |
 *         --o-->o-G-o-->o--
 *
 * After making loop, always place generic node out front.  It becomes
 * the start of enclosing block.  The aPlusBlk is the target of the loop.
 *
 * Plus blks have TWO EndBlk nodes--the far right and the node that loops back
 * to the aPlusBlk node.
 *
 * Plus blocks have a set of locks (from 1..CLL_k) on the aPlusBlk node.
 */
Graph makePlus( Graph g1, int approx, char * pFirstSetSymbol);


/**
 * Return an optional path:  --o-->o--
 */
Graph emptyAlt();


ActionNode *newActionNode();


/**
 * allocate the array of locks (1..CLL_k) used to inhibit infinite recursion.
 * Infinite recursion can occur in (..)* blocks, FIRST calcs and FOLLOW calcs.
 * Therefore, we need locks on aLoopBlk, RuleBlk, EndRule nodes.
 *
 * if ( lock[k]==TRUE ) then we have been here before looking for k tokens
 * of lookahead.
 */
char *makelocks();
