#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

// define ErrHdr "%s, line %d:"
#include "constants.h"
// MaxNumFiles
#include "generic.h"
// for zzline
#include "dlgdef.h"

// from globals.c
extern int CurFile;
extern char *FileStr[MaxNumFiles];/* Ptr to array of file names on command-line */
extern int NumFiles;     /* current grammar file number */



void warnNoFL(char *err)
{
  fprintf(stderr, "warning: %s\n", err);
}

/**
 * \param f filename
 * \param l line number
 */
void warnFL(char *err,char *f,int l)
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " warning: %s\n", err);
}

void warn(char *err)
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " warning: %s\n", err);
}

void warnNoCR( char *err )
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " warning: %s", err);
}

void errNoFL(char *err)
{
  fprintf(stderr, "error: %s\n", err);
}

void errFL(char *err,char *f,int l)
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " error: %s\n", err);
}

void err(char *err)
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " error: %s\n", err);
}

void errNoCR( char *err )
{
  /* back up the file number if we hit an error at the end of the last file */
  if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
  fprintf(stderr, ErrHdr, FileStr[CurFile], zzline);
  fprintf(stderr, " error: %s", err);
}


void fatalFL( char *err_, char *f, int l )
{
  fprintf(stderr, ErrHdr, f, l);
  fprintf(stderr, " %s\n", err_);
  cleanUp();
  exit(EXIT_FAILURE);
}
