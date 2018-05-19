/**
 * \file anlr_log.c
 */

#include <stdio.h>
// MaxNumFiles
#include "generic.h"
#include "misc.h"
#include "fset.h"
#include "fset2.h"
// for zzline
#include "scan.h"
#include "logger.h"
#include "antlr_log.h"

// from globals.c and declared in proto.h
extern FILE *DefFile;
extern int CurFile;
extern char *FileStr[MaxNumFiles];/* Ptr to array of file names on command-line */
extern int NumFiles;     /* current grammar file number */


void warn(char *err)
{
    /* back up the file number if we hit an error at the end of the last file */
    if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
    warning(err, FileStr[CurFile], zzline);
}


void err(char *err)
{
    /* back up the file number if we hit an error at the end of the last file */
    if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
    errorFL(err, FileStr[CurFile], zzline);
}


void dlgerror(const char *s)
{
    hdrLog(FileStr[CurFile], zzline);
    fprintf(stderr, " lexical error: %s (text was '%s')\n",
            ((s == NULL) ? "Lexical error" : s), zzlextext);
}

void fatalFL(char *err, char *f, int l )
{
    hdrLog(f, l);
    fprintf(stderr, " %s\n", err);
    cleanUp();
    exit(EXIT_FAILURE);
}


/** TODO : replace fatalFL or remove this function */
void fatal_intern(char *err_, char *f, int l)
{
    hdrLog(f, l);
    fprintf(stderr, " #$%%*&@# internal error: %s\n", err_);
    hdrLog(f, l);
    fprintf(stderr, " [complain to nearest government official\n");
    hdrLog(f, l);
    fprintf(stderr, "  or send hate-mail to parrt@parr-research.com;\n");
    hdrLog(f, l);
    fprintf(stderr, "  please pray to the ``bug'' gods that there is a trival fix.]\n");
    cleanUp();
    exit(EXIT_FAILURE);
}

/**
 * Close 'DefFile' if needed.
 */
void cleanUp(void) {
    if ( DefFile != NULL) fclose( DefFile );
}


            /* M e t h o d  T a b l e s */
/*
 * The following tables are used to fill syntax diagram nodes with the correct
 * function pointers for computing FIRST sets and printing themselves.
 */

/* fpTraverse[node type] == pointer to function that calculates trees
 * representing the FIRST sets for that node (maintains spatial info).
 * We use 'struct _tree' not 'tree' due to a g++ 2.4.3 bug.
 */
Tree *(*fpTraverse[NumNodeTypes+1])() = {
  NULL,
  tJunc,
  tRuleRef,
  tToken,
  tAction
};

/* fpPrint[node type] == pointer to function that knows how to print that node. */
void (*fpPrint[NumNodeTypes+1])() = {
  NULL,
  pJunc,
  pRuleRef,
  pToken,
  pAction
};

/* fpReach[node type] == pointer to function that calculates FIRST set for
 * that node. (r stands for reach).  We use 'struct _set' not 'set'
 * due to a g++ 2.4.3 bug.
 */
set (*fpReach[NumNodeTypes+1])() = {
  NULL,
  rJunc,
  rRuleRef,
  rToken,
  rAction
};
