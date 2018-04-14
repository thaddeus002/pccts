/*
 * errsupport.c -- error support code for SORCERER output
 *
 * Define your own or compile and link this in.
 *
 * Terence Parr
 * U of MN, AHPCRC
 * February 1994
 */
#include "sorcerer.h"

void
mismatched_range( STreeParser *_parser, int looking_for, int upper_token, SORAST *found )
{
    if ( found!=NULL ) {
        fprintf(stderr,
                "parse error: expected token range %d..%d found token %d\n",
                looking_for, upper_token,
                found->token);
    }
    else {
        fprintf(stderr,
                "parse error: expected token range %d..%d found NULL tree\n",
                looking_for, upper_token);
    }
}

void
missing_wildcard(STreeParser *_parser)
{
    fprintf(stderr, "parse error: expected any token/tree found found NULL tree\n");
}

void
mismatched_token( STreeParser *_parser, int looking_for, SORAST *found )
{
    if ( found!=NULL ) {
        fprintf(stderr,
                "parse error: expected token %d found token %d\n",
                looking_for,
                found->token);
    }
    else {
        fprintf(stderr,
                "parse error: expected token %d found NULL tree\n",
                looking_for);
    }
}

void
no_viable_alt( STreeParser *_parser, char *rulename, SORAST *root )
{
    if ( root==NULL )
        fprintf(stderr,
                "parse error: in rule %s, no viable alternative for NULL tree\n",
                rulename);
    else
        fprintf(stderr,
                "parse error: in rule %s, no viable alternative for tree\n",
                rulename);
}

void
sorcerer_panic(char *err)
{
    fprintf(stderr, "panic: %s\n", err);
    exit(-1);
}
